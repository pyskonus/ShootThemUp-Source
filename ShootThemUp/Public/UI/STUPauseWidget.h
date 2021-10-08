// Created by pyskonus.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPauseWidget.generated.h"

class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  UButton* ClearPauseButton;

  virtual void NativeOnInitialized() override;

private:
  UFUNCTION()
  void OnClearPause();
};
