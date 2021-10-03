// Created by pyskonus.


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) {
  AutoPossessAI = EAutoPossessAI::Disabled;
  AIControllerClass = ASTUAIController::StaticClass();

  if (GetCharacterMovement()) {
    bUseControllerRotationYaw = false;
    /*GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;*/
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
  }
}

void ASTUAICharacter::OnDeath() {
  Super::OnDeath();

  const auto STUController = Cast<AAIController>(Controller);
  if (STUController && STUController->BrainComponent)
    STUController->BrainComponent->Cleanup();
}