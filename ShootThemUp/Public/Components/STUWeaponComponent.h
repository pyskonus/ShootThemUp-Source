// Created by pyskonus.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USTUWeaponComponent();

  void Fire();

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  TSubclassOf<ASTUBaseWeapon> WeaponClass;

  UPROPERTY(EditDefaultsOnly, Category = "Weapon")
  FName WeaponAttachmentPointName = "WeaponSocket";

  virtual void BeginPlay() override;

private:
  UPROPERTY()
  ASTUBaseWeapon* CurrentWeapon = nullptr;

  void SpawnWeapon();
};
