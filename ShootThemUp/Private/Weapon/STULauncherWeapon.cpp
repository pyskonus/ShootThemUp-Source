// Created by pyskonus.

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void ASTULauncherWeapon::StartFire() {
  MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
  if (!GetWorld() || IsAmmoEmpty())
    return;

  const auto Player = Cast<ACharacter>(GetOwner());
  if (!Player)
    return;

  const auto Controller = Player->GetController<AController>();
  if (!Controller)
    return;

  FVector ViewLocation;
  FRotator ViewRotation;

  /// get view point
  const auto STUCharacter = Cast<ACharacter>(GetOwner());
  if (!STUCharacter)
    return;
  if (STUCharacter->IsPlayerControlled()) {
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
  } else {
    ViewLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
  }

  const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
  const FVector TraceStart = ViewLocation;              //SocketTransform.GetLocation();
  const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector();
  const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner());
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

  const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
  const FVector Direction = (EndPoint - SocketTransform.GetLocation()).GetSafeNormal();

  /*const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);*/
  const FTransform SpawnTransform(FRotator::ZeroRotator, SocketTransform.GetLocation());

  ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
  if (Projectile) {
    Projectile->SetShotDirection(Direction);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(SpawnTransform);
  }

  DecreaseAmmo();
  SpawnMuzzleFX();
}