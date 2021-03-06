// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class AController;
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

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

  FWeaponUIData GetUIData() const { return UIData; }
  FAmmoData GetAmmoData() const { return CurrentAmmo; }

  bool TryToAddAmmo(int32 ClipsAmount);
  bool IsAmmoEmpty() const;
  bool IsAmmoFull() const;
  
  virtual void Zoom(bool Enable) {}

  AController* GetController() const;

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

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
  FWeaponUIData UIData;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
  UNiagaraSystem* MuzzleFX;

  virtual void BeginPlay() override;

  virtual void MakeShot();
  bool AngleSharp(const FVector& Muzzle, const FVector& ImpactPoint, const FVector& SocketFwd);
  /*bool GetTraceData(FVector& TraceStart, FVector TraceEnd);*/
  void MakeDamage(const FHitResult& HitResult);

  void DecreaseAmmo();
  bool IsClipEmpty() const;
  void LogAmmo();

  UNiagaraComponent* SpawnMuzzleFX();

private:
  FAmmoData CurrentAmmo;
};
