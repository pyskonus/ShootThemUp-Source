// Created by pyskonus.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  UButton* GoToMenuButton;

  virtual void NativeOnInitialized() override;

private:
  UFUNCTION()
  void OnGoToMenu();
};
