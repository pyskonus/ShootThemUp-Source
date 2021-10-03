// Created by pyskonus.

#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUBasePlayerController.h"
#include "AIController.h"
#include "STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase() {
  DefaultPawnClass = ASTUBaseCharacter::StaticClass();
  PlayerControllerClass = ASTUBasePlayerController::StaticClass();
  PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
  Super::StartPlay();

  SpawnBots();
  CreateTeamsInfo();

  CurrentRound = 1;
  StartRound();
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
}

void ASTUGameModeBase::GameTimerUpdate() {
  UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i | Round %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

  if (--RoundCountDown == 0) {
    GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

    if (CurrentRound + 1 <= GameData.RoundsNum) {
      ++CurrentRound;
      StartRound();
    } else {
      UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER ====="))
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