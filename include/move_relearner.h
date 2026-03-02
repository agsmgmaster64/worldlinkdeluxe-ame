#ifndef GUARD_MOVE_RELEARNER_H
#define GUARD_MOVE_RELEARNER_H

#include "constants/move_relearner.h"

void TeachMoveRelearnerMove(void);
void MoveRelearnerShowHideHearts(s32 move);
void MoveRelearnerShowHideCategoryIcon(s32);
void CB2_InitLearnMove(void);
bool32 CanBoxMonRelearnAnyMove(struct BoxPokemon *boxMon);
bool32 CanBoxMonRelearnMoves(struct BoxPokemon *boxMon, enum MoveRelearnerStates state);
u32 GetRelearnerLevelUpMoves(struct BoxPokemon *mon, u16 *moves);
u32 GetRelearnerEggMoves(struct BoxPokemon *mon, u16 *moves);
u32 GetRelearnerTMMoves(struct BoxPokemon *mon, u16 *moves);
u32 GetRelearnerTutorMoves(struct BoxPokemon *mon, u16 *moves);

extern enum MoveRelearnerStates gMoveRelearnerState;
extern enum RelearnMode gRelearnMode;

#endif //GUARD_MOVE_RELEARNER_H
