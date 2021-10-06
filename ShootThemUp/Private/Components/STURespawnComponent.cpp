// Created by pyskonus.

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 RespawnTime) {
  if (!GetWorld())
    return;
  
  if (RespawnTime == 0) {
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
      return;

    GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    return;
  }

  RespawnCountdown = RespawnTime;
  GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
  /*if (GetOwner()->IsA(ASTUPlayerController))*/
}

void USTURespawnComponent::RespawnTimerUpdate() {
  if (--RespawnCountdown == 0) {
    if (!GetWorld())
      return;
    GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
      return;

    GameMode->RespawnRequest(Cast<AController>(GetOwner()));
  }
}

bool USTURespawnComponent::IsRespawnInProgress() const {
  return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}