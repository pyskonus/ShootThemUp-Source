// Created by pyskonus.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  UButton* StartGameButton;

  UPROPERTY(meta = (BindWidget))
  UButton* QuitGameButton;

  virtual void NativeOnInitialized() override;

private:
  UFUNCTION()
  void OnStartGame();

  UFUNCTION()
  void OnQuitGame();
};
