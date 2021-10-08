// Created by pyskonus.


#include "Menu/STUMenuPlayerController.h"

void ASTUMenuPlayerController::BeginPlay() {
  Super::BeginPlay();

  SetInputMode(FInputModeUIOnly());
  bShowMouseCursor = true;
}