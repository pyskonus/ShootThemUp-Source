// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor {
  GENERATED_BODY()

public:
  ASTUBasePickup();

protected:
  UPROPERTY(VisibleAnywhere, Category = "Pickup")
  USphereComponent* CollisionComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
  float RespawnTime = 5.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
  float FluctuationSpeed = 40.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
  float RotationSpeed = 40.0f;

  virtual void BeginPlay() override;
  virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
  virtual void Tick(float DeltaTime) override;

private:
  virtual bool GivePickupTo(APawn* PlayerPawn);
  void PickupWasTaken();
  void Respawn();
};
