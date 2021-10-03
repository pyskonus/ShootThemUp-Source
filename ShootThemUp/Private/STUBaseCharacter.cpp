// Created by pyskonus.

#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "STU_CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(Shit, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
  SpringArmComponent->SetupAttachment(GetRootComponent());
  SpringArmComponent->bUsePawnControlRotation = true;
  SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

  CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
  CameraComponent->SetupAttachment(SpringArmComponent);

  HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

  HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
  HealthTextComponent->SetupAttachment(GetRootComponent());
  HealthTextComponent->SetOwnerNoSee(true);

  WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay() {
  Super::BeginPlay();

  check(HealthComponent);
  check(HealthTextComponent);
  check(GetCharacterMovement());
  check(GetMesh());

  OnHealthChange(HealthComponent->GetHealth(), 0.0f);
  HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
  HealthComponent->OnHealthChange.AddUObject(this, &ASTUBaseCharacter::OnHealthChange);

  LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  check(PlayerInputComponent);
  check(WeaponComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
  PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
  PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
  PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
  PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
  PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
  PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
  PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}

bool ASTUBaseCharacter::IsRunning() const {
  return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const {
  if (GetVelocity().IsZero())
    return 0.0f;
  const auto VelocityNormal = GetVelocity().GetSafeNormal();
  const auto AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal)));
  const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

  return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForward(float Amount) {
  AddMovementInput(GetActorForwardVector(), Amount);

  IsMovingForward = Amount > 0.0f;
}

void ASTUBaseCharacter::MoveRight(float Amount) {
  AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRunning() {
  WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning() {
  WantsToRun = false;
}

void ASTUBaseCharacter::OnDeath() {
  // PlayAnimMontage(DeathAnimMontage);

  GetCharacterMovement()->DisableMovement();
  SetLifeSpan(LifeSpanOnDeath);

  if (Controller) {
    Controller->ChangeState(NAME_Spectating);
  }

  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
  WeaponComponent->StopFire();

  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChange(float Health, float HealthDelta) {
  HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) {
  const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
  UE_LOG(Shit, Display, TEXT("Landed velocity: %f"), FallVelocityZ);

  if (FallVelocityZ < LandedVelocity.X)
    return;

  const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocity, LandedDamage, FallVelocityZ);
  UE_LOG(Shit, Display, TEXT("Landed damage: %f"), FinalDamage);
  TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color) {
  const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
  if (!MaterialInst)
    return;

  MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}