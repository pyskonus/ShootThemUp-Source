// Created by pyskonus.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance {
  GENERATED_BODY()

public:
  FString TestString = "Hello Game!";
};
