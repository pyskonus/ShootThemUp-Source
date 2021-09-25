// Created by pyskonus.


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit) {
  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
  AIControllerClass = ASTUAIController::StaticClass();

  if (GetCharacterMovement()) {
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
  }
}