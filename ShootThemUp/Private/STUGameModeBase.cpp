// Created by pyskonus.

#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUBasePlayerController.h"
#include "AIController.h"
#include "STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 5;

ASTUGameModeBase::ASTUGameModeBase() {
  DefaultPawnClass = ASTUBaseCharacter::StaticClass();
  PlayerControllerClass = ASTUBasePlayerController::StaticClass();
  HUDClass = ASTUGameHUD::StaticClass();
  PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
  Super::StartPlay();

  SpawnBots();
  CreateTeamsInfo();

  CurrentRound = 1;
  StartRound();

  SetMatchState(ESTUMatchState::InProgress);
}

void ASTUGameModeBase::SpawnBots() {
  if (!GetWorld())
    return;

  for (int32 i = 0; i < GameData.PlayersNum - 1; ++i) {
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const auto CurrentAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
    RestartPlayer(CurrentAIController);
  }
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController) {
  if (InController && InController->IsA<AAIController>()) {
    return AIPawnClass;
  }
  return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::StartRound() {
  RoundCountDown = GameData.RoundTime;
  GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);

  ResetPlayers();
}

void ASTUGameModeBase::GameTimerUpdate() {
  UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i | Round %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

  if (--RoundCountDown == 0) {
    GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

    if (CurrentRound + 1 <= GameData.RoundsNum) {
      ++CurrentRound;
      StartRound();
    } else {
      GameOver();
    }
  }
}

void ASTUGameModeBase::ResetPlayers() {
  if (!GetWorld())
    return;

  for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
    ResetOnePlayer(It->Get());
  }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller) {
  if (Controller && Controller->GetPawn())
    Controller->GetPawn()->Reset();
  
  RestartPlayer(Controller);
  SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo() {
  if (!GetWorld())
    return;

  int32 TeamID = 1;     /// 1 through GameData.TeamColors.Num()
  for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
    const auto Controller = It->Get();
    if (!Controller)
      continue;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState)
      continue;

    PlayerState->SetTeamID(TeamID);
    PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
    PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
    SetPlayerColor(Controller);
    /*TeamID = (TeamID == 1) ? 2 : 1*//*GameData.TeamColors.Num() == 0 ? 0 : (TeamID + 1) % GameData.TeamColors.Num()*/;
    TeamID++;
    if (TeamID > GameData.TeamColors.Num())
      TeamID = 1;
  }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const {
  if (TeamID - 1 < GameData.TeamColors.Num())
    return GameData.TeamColors[TeamID - 1];

  UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id %i, set to default %s"), TeamID, *GameData.DefaultTeamColor.ToString());

  return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller) {
  if (!Controller)
    return;

  const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
  if (!Character)
    return;

  const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
  if (!PlayerState)
    return;

  Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController) {
  const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
  const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

  if (KillerPlayerState)
    KillerPlayerState->AddKill();

  if (VictimPlayerState)
    VictimPlayerState->AddDeath();

  StartRespawn(VictimController);
}

void ASTUGameModeBase::LogInfo() {
  if (!GetWorld())
    return;

  for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
    const auto Controller = It->Get();
    if (!Controller)
      continue;
    
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState)
      continue;
    
    PlayerState->LogPlayerInfo();
  }
}

void ASTUGameModeBase::RespawnRequest(AController* Controller) {
  ResetOnePlayer(Controller);
}

void ASTUGameModeBase::StartRespawn(AController* Controller) {
  bool RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
  if (!RespawnAvailable)
    return;

  const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
  if (!RespawnComponent)
    return;

  RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver() {
  UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER ====="))
  LogInfo();

  for (auto Pawn : TActorRange<APawn>(GetWorld())) {
    if (Pawn) {
      Pawn->TurnOff();
      Pawn->DisableInput(nullptr);
    }
  }

  SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State) {
  if (MatchState == State)
    return;

  MatchState = State;
  OnMatchStateChanged.Broadcast(MatchState);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate/* = FCanUnpause()*/) {
  const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
  if (PauseSet)
    SetMatchState(ESTUMatchState::Pause);

  return PauseSet;
}

bool ASTUGameModeBase::ClearPause() {
  const auto PauseCleared = Super::ClearPause();

  if (PauseCleared)
    SetMatchState(ESTUMatchState::InProgress);

  return PauseCleared;
}