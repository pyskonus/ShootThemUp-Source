// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon {
  GENERATED_BODY()

public:
  virtual void StartFire() override;
  virtual void StopFire() override;

protected:
  void MakeShot() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
  float TimeBetweenShots = 0.15f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
  float BulletSpread = 0.5f;

private:
  FTimerHandle ShotTimerHandle;
};
