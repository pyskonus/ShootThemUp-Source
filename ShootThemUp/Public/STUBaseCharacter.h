// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
  virtual void BeginPlay() override;
  virtual void OnDeath();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USTUHealthComponent* HealthComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USTUWeaponComponent* WeaponComponent;

  UPROPERTY(EditDefaultsOnly, Category = "Animation")
  UAnimMontage* DeathAnimMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  FVector2D LandedVelocity = FVector2D(900.0f, 1200.0f);

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  float LifeSpanOnDeath = 5.0f;

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  FName MaterialColorName = "Paint Color";


public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Movement")
  virtual bool IsRunning() const;

  UFUNCTION(BlueprintCallable, Category = "Movement")
  float GetMovementDirection() const;

  void SetPlayerColor(const FLinearColor& Color);

private:
  void OnHealthChange(float Health, float HealthDelta);

  UFUNCTION()
  void OnGroundLanded(const FHitResult& Hit);
};
