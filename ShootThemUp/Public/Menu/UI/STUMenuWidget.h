// Created by pyskonus.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  UPROPERTY(meta = (BindWidget))
  UButton* StartGameButton;

  UPROPERTY(meta = (BindWidget))
  UButton* QuitGameButton;

  UPROPERTY(meta = (BindWidget))
  UHorizontalBox* LevelItemsBox;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  TSubclassOf<UUserWidget> LevelItemWidgetClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
  USoundCue* StartGameSound;

  virtual void NativeOnInitialized() override;

private:
  UPROPERTY()
  TArray<USTULevelItemWidget*> LevelItemWidgets;

  UFUNCTION()
  void OnStartGame();

  UFUNCTION()
  void OnQuitGame();

  void InitLevelItems();
  void OnLevelSelected(const FLevelData& Data);
  USTUGameInstance* GetSTUGameInstance() const;
};

