 // Created by pyskonus.


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
  
    InitAnimations();

    SpawnWeapons();

    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
  CurrentWeapon = nullptr;
  for (auto Weapon : Weapons) {
    Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    Weapon->Destroy();
  }
  Weapons.Empty();
  Super::EndPlay(EndPlayReason);
};

void USTUWeaponComponent::SpawnWeapons() {
  ACharacter* Character = Cast<ACharacter>(GetOwner());
  if (!Character || !GetWorld())
    return;

  for (auto OneWeaponData : WeaponData) {
    auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
    if (!Weapon)
      continue;

    Weapon->SetOwner(Character);
    Weapons.Add(Weapon);
    AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
  }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) {
  if (!Weapon || !SceneComponent)
    return;
  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
  Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) {
  if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) {
    UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
    return;
  }
  ACharacter* Character = Cast<ACharacter>(GetOwner());
  if (!Character)
    return;

  if (CurrentWeapon) {
    CurrentWeapon->StopFire();
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
  }

  CurrentWeapon = Weapons[WeaponIndex];
  //CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
  const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
  CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

  AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

  EquipAnimInProgress = true;
  PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire() {
  if (!CanFire())
    return;

  CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire() {
  if (!CurrentWeapon)
    return;

  CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() {
  if (!CanEquip())
    return;

  CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
  EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
  ACharacter* Character = Cast<ACharacter>(GetOwner());
  if (!Character)
    return;

  Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations() {
  if (!EquipAnimMontage)
    return;

  const auto NotifyEvents = EquipAnimMontage->Notifies;

  for (auto NotifyEvent : NotifyEvents) {
    auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);
    if (EquipFinishedNotify) {
      EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
      break;
    }
  }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
  ACharacter* Character = Cast<ACharacter>(GetOwner());
  if (!Character || Character->GetMesh() != MeshComponent)
    return;

  EquipAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() {
  return (CurrentWeapon && !EquipAnimInProgress);
}

bool USTUWeaponComponent::CanEquip() {
  return !EquipAnimInProgress;
}

void USTUWeaponComponent::Reload() {
  PlayAnimMontage(CurrentReloadAnimMontage);
}