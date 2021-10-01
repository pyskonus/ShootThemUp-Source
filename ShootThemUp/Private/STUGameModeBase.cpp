// Created by pyskonus.

#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUBasePlayerController.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase() {
  DefaultPawnClass = ASTUBaseCharacter::StaticClass();
  PlayerControllerClass = ASTUBasePlayerController::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
  Super::StartPlay();

  SpawnBots();

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
  RoundCountDown = GameData.RoundsNum;
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