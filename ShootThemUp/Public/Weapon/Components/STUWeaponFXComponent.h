// Created by pyskonus.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USTUWeaponFXComponent();

  void PlayImpactFX(const FHitResult& Hit);

  protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
  UNiagaraSystem* Effect;
};
