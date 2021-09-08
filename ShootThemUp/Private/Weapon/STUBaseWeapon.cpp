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
  CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() {
}

void ASTUBaseWeapon::StopFire() {
}

void ASTUBaseWeapon::MakeShot() {
}

//bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
//  FVector ViewLocation;
//  FRotator ViewRotation;
//  if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
//    return false;
//
//  TraceStart = ViewLocation;
//  const FVector ShootDirection = ViewLocation.Vector();
//  TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
//  return true;
//}

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

void ASTUBaseWeapon::DecreaseAmmo() {
  CurrentAmmo.Bullets--;
  LogAmmo();

  if (IsClipEmpty() && !IsAmmoEmpty()) {
    ChangeClip();
  }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const {
  return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 1 /*first clip is full so not 0 here*/ && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const {
  return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip() {
  CurrentAmmo.Bullets = DefaultAmmo.Bullets;
  if (!CurrentAmmo.Infinite) {
    CurrentAmmo.Clips--;
  }
  UE_LOG(LogBaseWeapon, Display, TEXT("====== Change Clip ======"));
}

void ASTUBaseWeapon::LogAmmo() {
  FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
  AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
  UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}