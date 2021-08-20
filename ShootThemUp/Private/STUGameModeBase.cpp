// Created by pyskonus.

#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUBasePlayerController.h"

ASTUGameModeBase::ASTUGameModeBase() {
  DefaultPawnClass = ASTUBaseCharacter::StaticClass();
  PlayerControllerClass = ASTUBasePlayerController::StaticClass();
}
