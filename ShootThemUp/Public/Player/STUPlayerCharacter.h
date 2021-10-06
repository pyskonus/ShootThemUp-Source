// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter {
  GENERATED_BODY()

public:
  ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual bool IsRunning() const override;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  USpringArmComponent* SpringArmComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  UCameraComponent* CameraComponent;

  virtual void OnDeath() override;

private:
  bool WantsToRun = false;
  bool IsMovingForward = false;

  void MoveForward(float Amount);
  void MoveRight(float Amount);

  void OnStartRunning();
  void OnStopRunning();
};
