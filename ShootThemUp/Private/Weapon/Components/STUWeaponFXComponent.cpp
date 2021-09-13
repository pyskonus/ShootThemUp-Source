// Created by pyskonus.


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) {
  auto Effect = DefaultEffect;

  if (Hit.PhysMaterial.IsValid()) {
    const auto PhysMat = Hit.PhysMaterial.Get();
    if (EffectsMap.Contains(PhysMat))
      Effect = EffectsMap[PhysMat];
  }
  UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}