// Created by pyskonus.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
  PrimaryActorTick.bCanEverTick = false;

  WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
  SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
  Super::BeginPlay();

  check(WeaponMesh);
}

void ASTUBaseWeapon::Fire() {
  MakeShot();
}

void ASTUBaseWeapon::MakeShot() {
  if (!GetWorld())
    return;

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
  const FVector TraceStart = ViewLocation;              //SocketTransform.GetLocation();
  const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector();
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
}

bool ASTUBaseWeapon::AngleSharp(const FVector& Muzzle, const FVector& ImpactPoint, const FVector& SocketFwd) {
  FVector ShotDirection = ImpactPoint - Muzzle;
  ShotDirection = ShotDirection.GetSafeNormal();
  const auto dp = FVector::DotProduct(ShotDirection, SocketFwd);
  auto angle = UKismetMathLibrary::Acos(dp);

  return angle < AngleThreshold;
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult) {
  const auto DamagedActor = HitResult.GetActor();
  if (!DamagedActor)
    return;

  const auto Player = Cast<ACharacter>(GetOwner());
  if (!Player)
    return;

  const auto Controller = Player->GetController<APlayerController>();
  if (!Controller)
    return;

  DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), Controller, this);
}