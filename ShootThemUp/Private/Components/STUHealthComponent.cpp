// Created by pyskonus.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;

  AActor* ComponentOwner = GetOwner();

  if (ComponentOwner) {
    ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
  }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
  Health -= Damage;
}
