// Created by pyskonus.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
  PrimaryActorTick.bCanEverTick = false;

  WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
  SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
  Super::BeginPlay();

  check(WeaponMesh);
  checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count must be > 0"));
  checkf(DefaultAmmo.Clips > 0, TEXT("Clips count must be > 0"));
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

  /*const auto Controller = Player->GetController<AController>();
  if (!Controller)
    return;*/

  DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

void ASTUBaseWeapon::DecreaseAmmo() {
  if (CurrentAmmo.Bullets == 0) {
    return;
  }
  CurrentAmmo.Bullets--;

  if (IsClipEmpty() && !IsAmmoEmpty()) {
    StopFire();
    OnClipEmpty.Broadcast(this);
  }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const {
  return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 1 /*first clip is full so not 0 here*/ && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const {
  return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip() {
  if (!CurrentAmmo.Infinite) {
    if (CurrentAmmo.Clips == 0) {
      return;
    }
    CurrentAmmo.Clips--;
  }
  CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::CanReload() const {
  return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo() {
  FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
  AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
  UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoFull() const {
  return CurrentAmmo.Clips == DefaultAmmo.Clips && //
         CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount) {
  if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
    return false;

  if (IsAmmoEmpty()) {
    CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
    OnClipEmpty.Broadcast(this);
  } else if (CurrentAmmo.Clips < DefaultAmmo.Clips) {
    const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
    if (DefaultAmmo.Clips >= NextClipsAmount) {
      CurrentAmmo.Clips = NextClipsAmount;
    } else {
      CurrentAmmo.Clips = DefaultAmmo.Clips;
      CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
  } else {
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
  }

  return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() {
  return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,                      //
                                                      WeaponMesh,                    //
                                                      MuzzleSocketName,              //
                                                      FVector::ZeroVector,           //
                                                      FRotator::ZeroRotator,         //
                                                      EAttachLocation::SnapToTarget, //
                                                      true);
}

AController* ASTUBaseWeapon::GetController() const {
  const auto Pawn = Cast<APawn>(GetOwner());
  return Pawn ? Pawn->GetController() : nullptr;
}