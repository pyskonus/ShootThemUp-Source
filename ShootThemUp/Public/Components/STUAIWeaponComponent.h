// Created by pyskonus.

#pragma once

#include "Components/STUWeaponComponent.h"
#include "CoreMinimal.h"
#include "STUAIWeaponComponent.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent {
  GENERATED_BODY()

public:
  virtual void StartFire() override;
  virtual void NextWeapon() override;
};
