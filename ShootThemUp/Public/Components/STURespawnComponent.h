// Created by pyskonus.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USTURespawnComponent();

  void Respawn(int32 RespawnTime);
  int32 GetRespawnCountdown() const { return RespawnCountdown; }
  bool IsRespawnInProgress() const;

private:
  FTimerHandle RespawnTimerHandle;
  int RespawnCountdown = 0;

  void RespawnTimerUpdate();
};
