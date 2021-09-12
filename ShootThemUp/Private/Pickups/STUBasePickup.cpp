// Created by pyskonus.


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

