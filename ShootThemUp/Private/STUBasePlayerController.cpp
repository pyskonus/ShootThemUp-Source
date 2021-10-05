// Created by pyskonus.


#include "STUBasePlayerController.h"
#include "Components/STURespawnComponent.h"

ASTUBasePlayerController::ASTUBasePlayerController() {
  RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}