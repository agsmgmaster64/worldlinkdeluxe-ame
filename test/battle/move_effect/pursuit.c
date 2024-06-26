#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_PURSUIT].effect == EFFECT_PURSUIT);
}

SINGLE_BATTLE_TEST("Pursuited mon correctly switches out after it got hit and activated ability Tangling Hair")
{
    GIVEN {
        PLAYER(SPECIES_GOLIATH) { Ability(ABILITY_TANGLING_HAIR); }
        PLAYER(SPECIES_CHIBI_YUUGI);
        OPPONENT(SPECIES_WYNAUT);
        OPPONENT(SPECIES_CHIBI_YUUGI);
    } WHEN {
        TURN { SWITCH(player, 1); MOVE(opponent, MOVE_PURSUIT); }
    } SCENE {
        MESSAGE("Dugtrio, that's enough! Come back!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PURSUIT, opponent);
        ABILITY_POPUP(player, ABILITY_TANGLING_HAIR);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        MESSAGE("Foe Wynaut's Speed fell!");
        MESSAGE("Go! Wobbuffet!");
    }
}
