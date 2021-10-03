// Created by pyskonus.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "STUNeedAmmoDecorator.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator {
  GENERATED_BODY()

public:
  USTUNeedAmmoDecorator();

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  TSubclassOf<ASTUBaseWeapon> WeaponType;

  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
