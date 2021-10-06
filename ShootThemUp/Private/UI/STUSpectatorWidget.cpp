// Created by pyskonus.


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const {
  const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());
  if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress())
    return false;

  CountDownTime = RespawnComponent->GetRespawnCountdown();
  return true;
}