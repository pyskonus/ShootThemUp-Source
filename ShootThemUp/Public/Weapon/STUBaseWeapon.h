// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor {
  GENERATED_BODY()

public:
  ASTUBaseWeapon();

  virtual void Fire();

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

  virtual void BeginPlay() override;

  void MakeShot();
  bool AngleSharp(const FVector& Muzzle, const FVector& ImpactPoint, const FVector& SocketFwd);
  void MakeDamage(const FHitResult& HitResult);
};
