// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter {
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USpringArmComponent* SpringArmComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  UCameraComponent* CameraComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USTUHealthComponent* HealthComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  UTextRenderComponent* HealthTextComponent;

  UPROPERTY(EditDefaultsOnly, Category = "Animation")
  UAnimMontage* DeathAnimMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  FVector2D LandedVelocity = FVector2D(900.0f, 1200.0f);

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

  UPROPERTY(EditDefaultsOnly, Category = "Misc")
  float LifeSpanOnDeath = 5.0f;


public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UFUNCTION(BlueprintCallable, Category = "Movement")
  bool IsRunning() const;

  UFUNCTION(BlueprintCallable, Category = "Movement")
  float GetMovementDirection() const;

private:
  bool WantsToRun = false;
  bool IsMovingForward = false;

  void MoveForward(float Amount);
  void MoveRight(float Amount);

  void OnStartRunning();
  void OnStopRunning();

  void OnDeath();
  void OnHealthChange(float Health);

  UFUNCTION()
  void OnGroundLanded(const FHitResult& Hit);
};
