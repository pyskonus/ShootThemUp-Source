// Created by pyskonus.

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "BrainComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/STUHealthBarWidget.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) {
  AutoPossessAI = EAutoPossessAI::Disabled;
  AIControllerClass = ASTUAIController::StaticClass();

  if (GetCharacterMovement()) {
    bUseControllerRotationYaw = false;
    /*GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;*/
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
  }

  HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
  HealthWidgetComponent->SetupAttachment(GetRootComponent());
  HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
  HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::OnDeath() {
  Super::OnDeath();

  const auto STUController = Cast<AAIController>(Controller);
  if (STUController && STUController->BrainComponent)
    STUController->BrainComponent->Cleanup();
}

void ASTUAICharacter::BeginPlay() {
  Super::BeginPlay();

  check(HealthWidgetComponent);
}

void ASTUAICharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::OnHealthChange(float Health, float HealthDelta) {
  Super::OnHealthChange(Health, HealthDelta);

  const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
  if (!HealthBarWidget)
    return;

  HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAICharacter::UpdateHealthWidgetVisibility() {
  if (!GetWorld() ||                             //
      !GetWorld()->GetFirstPlayerController() || //
      !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
    return;
  const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
  const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
  HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}