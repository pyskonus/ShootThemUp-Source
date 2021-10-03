// Created by pyskonus.


#include "STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)

void ASTUPlayerState::LogPlayerInfo() {
  UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i | KillsNum: %i | DeathsNum: %i"), TeamID, KillsNum, DeathsNum)
}
