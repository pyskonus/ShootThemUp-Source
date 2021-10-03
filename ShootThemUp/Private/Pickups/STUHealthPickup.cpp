// Created by pyskonus.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"


bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn) {
  const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
  if (!HealthComponent)
    return false;

  return HealthComponent->TryToAddHealth(HealthAmount);
}