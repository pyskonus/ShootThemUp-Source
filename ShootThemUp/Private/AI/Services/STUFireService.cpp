// Created by pyskonus.


#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireService::USTUFireService() {
  NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  const auto Controller = OwnerComp.GetAIOwner();
  const auto Blackboard = OwnerComp.GetBlackboardComponent();

  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}