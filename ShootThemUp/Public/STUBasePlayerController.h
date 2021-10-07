// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUBasePlayerController.generated.h"

class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePlayerController : public APlayerController {
  GENERATED_BODY()
public:
  ASTUBasePlayerController();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USTURespawnComponent* RespawnComponent;

  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

private:
  void OnPauseGame();
  void OnMatchStateChanged(ESTUMatchState State);
};
