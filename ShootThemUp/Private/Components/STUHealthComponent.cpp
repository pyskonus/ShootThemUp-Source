// Created by pyskonus.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHC, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;
  OnHealthChange.Broadcast(Health);

  AActor* ComponentOwner = GetOwner();

  if (ComponentOwner) {
    ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
  }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
  if (Damage <= 0.0f || IsDead())
    return;

  Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
  OnHealthChange.Broadcast(Health);

  if (IsDead()) {
    OnDeath.Broadcast();
  }
}
