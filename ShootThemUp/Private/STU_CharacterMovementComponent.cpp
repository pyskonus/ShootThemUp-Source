// Created by pyskonus.


#include "STU_CharacterMovementComponent.h"
#include "STUBaseCharacter.h"

float USTU_CharacterMovementComponent::GetMaxSpeed() const {
  const float MaxSpeed = Super::GetMaxSpeed();
  const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
  return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
