// Created by pyskonus.

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup() {
  PrimaryActorTick.bCanEverTick = true;

  CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
  CollisionComponent->InitSphereRadius(50.0f);
  CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  SetRootComponent(CollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay() {
  Super::BeginPlay();

  check(CollisionComponent);
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  FVector NewLocation = GetActorLocation();
  FRotator NewRotation = GetActorRotation();
  float RunningTime = GetGameTimeSinceCreation();
  float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
  NewLocation.Z += DeltaHeight * FluctuationSpeed; //Scale our height by a factor of 20
  float DeltaRotation = DeltaTime * RotationSpeed; //Rotate by 20 degrees per second
  NewRotation.Yaw += DeltaRotation;
  SetActorLocationAndRotation(NewLocation, NewRotation);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor) {
  Super::NotifyActorBeginOverlap(OtherActor);

  const auto Pawn = Cast<APawn>(OtherActor);
  if (GivePickupTo(Pawn))
    PickupWasTaken();
}

void ASTUBasePickup::PickupWasTaken() {
  CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  if (GetRootComponent())
    GetRootComponent()->SetVisibility(false, true);

  FTimerHandle RespawnTimerHandle;
  GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}
void ASTUBasePickup::Respawn() {
  CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  if (GetRootComponent())
    GetRootComponent()->SetVisibility(true, true);
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn) {
  return false;
}
