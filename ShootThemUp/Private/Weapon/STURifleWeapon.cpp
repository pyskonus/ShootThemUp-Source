// Created by pyskonus.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

void ASTURifleWeapon::StartFire() {
  GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
  MakeShot();
}

void ASTURifleWeapon::StopFire() {
  GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot() {
  if (!GetWorld() || IsAmmoEmpty()) {
    StopFire();
    return;
  }

  const auto Player = Cast<ACharacter>(GetOwner());
  if (!Player)
    return;

  const auto Controller = Player->GetController<APlayerController>();
  if (!Controller)
    return;

  FVector ViewLocation;
  FRotator ViewRotation;
  Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

  const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
  const FVector TraceStart = ViewLocation; //SocketTransform.GetLocation();
  const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
  const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); //SocketTransform.GetRotation().GetForwardVector();
  const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner());
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

  if (HitResult.bBlockingHit && AngleSharp(SocketTransform.GetLocation(), HitResult.ImpactPoint, SocketTransform.GetRotation().GetForwardVector())) {
    DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
    DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

    MakeDamage(HitResult);
  } else {
    DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
  }

  DecreaseAmmo();
}