// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD {
  GENERATED_BODY()

public:
  virtual void DrawHUD() override;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI");
  TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

  virtual void BeginPlay() override;

private:
  void DrawCrosshair();
  void OnMatchStateChanged(ESTUMatchState State);
};
