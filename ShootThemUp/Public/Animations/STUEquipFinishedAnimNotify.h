// Created by pyskonus.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "STUEquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class SHOOTTHEMUP_API USTUEquipFinishedAnimNotify : public UAnimNotify {
  GENERATED_BODY()
public:
  virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

  FOnNotifiedSignature OnNotified;
};
