// Created by pyskonus.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() {
  MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
  const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
  const FTransform SpawnTransform(FRotator::ZeroRotator, SocketTransform.GetLocation());

  auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
  // TODO: set projectile params

  UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}