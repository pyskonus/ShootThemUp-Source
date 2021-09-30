// Created by pyskonus.

#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUBasePlayerController.h"
#include "AIController.h"

ASTUGameModeBase::ASTUGameModeBase() {
  DefaultPawnClass = ASTUBaseCharacter::StaticClass();
  PlayerControllerClass = ASTUBasePlayerController::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
  Super::StartPlay();

  SpawnBots();
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