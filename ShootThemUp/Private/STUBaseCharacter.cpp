// Created by pyskonus.

#include "STUBaseCharacter.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "STU_CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
  PrimaryActorTick.bCanEverTick = true;

  HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

  WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay() {
  Super::BeginPlay();

  check(HealthComponent);
  check(GetCharacterMovement());
  check(GetMesh());

  OnHealthChange(HealthComponent->GetHealth(), 0.0f);
  HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
  HealthComponent->OnHealthChange.AddUObject(this, &ASTUBaseCharacter::OnHealthChange);

  LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::IsRunning() const {
  return false;
}

float ASTUBaseCharacter::GetMovementDirection() const {
  if (GetVelocity().IsZero())
    return 0.0f;
  const auto VelocityNormal = GetVelocity().GetSafeNormal();
  const auto AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal)));
  const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

  return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath() {
  // PlayAnimMontage(DeathAnimMontage);

  GetCharacterMovement()->DisableMovement();
  SetLifeSpan(LifeSpanOnDeath);

  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
  WeaponComponent->StopFire();

  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChange(float Health, float HealthDelta) {
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) {
  const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

  if (FallVelocityZ < LandedVelocity.X)
    return;

  const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocity, LandedDamage, FallVelocityZ);
  TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color) {
  const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
  if (!MaterialInst)
    return;

  MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}