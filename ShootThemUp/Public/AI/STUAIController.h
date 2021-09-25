// Created by pyskonus.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController {
  GENERATED_BODY()

protected:
  virtual void OnPossess(APawn* InPawn) override;
};
