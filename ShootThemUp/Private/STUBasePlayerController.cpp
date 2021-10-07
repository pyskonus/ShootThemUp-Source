// Created by pyskonus.


#include "STUBasePlayerController.h"
#include "Components/STURespawnComponent.h"
#include "GameFramework/GameModeBase.h"

ASTUBasePlayerController::ASTUBasePlayerController() {
  RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUBasePlayerController::SetupInputComponent() {
  Super::SetupInputComponent();
  if (!InputComponent)
    return;

  InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUBasePlayerController::OnPauseGame);
}

void ASTUBasePlayerController::OnPauseGame() {
  if (!GetWorld() || !GetWorld()->GetAuthGameMode())
    return;

  GetWorld()->GetAuthGameMode()->SetPause(this);
}