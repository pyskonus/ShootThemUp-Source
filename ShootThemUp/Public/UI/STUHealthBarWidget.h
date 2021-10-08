// Created by pyskonus.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget {
  GENERATED_BODY()

public:
  void SetHealthPercent(float Percent);

protected:
  UPROPERTY(meta = (BindWidget))
  UProgressBar* HealthProgressBar;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  float PercentVisibilityThreshold = 0.8;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  float PercentColorThreshold = 0.3;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  FLinearColor GoodColor = FLinearColor::White;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  FLinearColor BadColor = FLinearColor::Red;
};
