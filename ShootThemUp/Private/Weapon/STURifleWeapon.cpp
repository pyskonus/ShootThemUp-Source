// Created by pyskonus.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"

ASTURifleWeapon::ASTURifleWeapon() {
  WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay() {
  Super::BeginPlay();
  check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire() {
  InitMuzzleFX();
  GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
  MakeShot();
}

void ASTURifleWeapon::StopFire() {
  GetWorldTimerManager().ClearTimer(ShotTimerHandle);
  SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot() {
  if (!GetWorld() || IsAmmoEmpty()) {
    StopFire();
    return;
  }

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
  const FVector TraceStart = ViewLocation; //SocketTransform.GetLocation();
  const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
  const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); //SocketTransform.GetRotation().GetForwardVector();
  const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner());
  CollisionParams.bReturnPhysicalMaterial = true;
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

  if (HitResult.bBlockingHit && AngleSharp(SocketTransform.GetLocation(), HitResult.ImpactPoint, SocketTransform.GetRotation().GetForwardVector())) {
    // DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
    // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    WeaponFXComponent->PlayImpactFX(HitResult);

    MakeDamage(HitResult);
  } else {
    DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
  }

  DecreaseAmmo();
}

void ASTURifleWeapon::InitMuzzleFX() {
  if (!MuzzleFXComponent)
    MuzzleFXComponent = SpawnMuzzleFX();

  SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible) {
  if (MuzzleFXComponent) {
    MuzzleFXComponent->SetPaused(!Visible);
    MuzzleFXComponent->SetVisibility(Visible, true);
  }
}

void ASTURifleWeapon::Zoom(bool Enabled) {
  const auto Controller = Cast<APlayerController>(GetController());
  if (!Controller || !Controller->PlayerCameraManager) return;

  const TInterval<float> FOV(50.0f, 90.0f);
  Controller->PlayerCameraManager->SetFOV(Enabled ? FOV.Min : FOV.Max);
}