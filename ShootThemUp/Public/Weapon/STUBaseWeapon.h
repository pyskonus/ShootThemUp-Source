// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor {
  GENERATED_BODY()

public:
  ASTUBaseWeapon();

  FOnClipEmptySignature OnClipEmpty;

  virtual void StartFire();
  virtual void StopFire();

  void ChangeClip();
  bool CanReload() const;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USkeletalMeshComponent* WeaponMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Misc")
  FName MuzzleSocketName = "MuzzleSocket";

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
  float TraceMaxDistance = 1500.0f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Misc")
  float AngleThreshold = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
  float DamageAmount = 10.0f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Misc")
  FAmmoData DefaultAmmo{15, 10, false};

  virtual void BeginPlay() override;

  virtual void MakeShot();
  bool AngleSharp(const FVector& Muzzle, const FVector& ImpactPoint, const FVector& SocketFwd);
  /*bool GetTraceData(FVector& TraceStart, FVector TraceEnd);*/
  void MakeDamage(const FHitResult& HitResult);

  void DecreaseAmmo();
  bool IsAmmoEmpty() const;
  bool IsClipEmpty() const;
  void LogAmmo();

private:
  FAmmoData CurrentAmmo;
};
