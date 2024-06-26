#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_DOUBLE_SHOCK].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE);
    ASSUME(MoveHasMoveEffectSelfArg(MOVE_DOUBLE_SHOCK, MOVE_EFFECT_REMOVE_ARG_TYPE, TYPE_WIND) == TRUE);
    ASSUME(gSpeciesInfo[SPECIES_CHIBI_YUUGI].types[0] != TYPE_WIND || gSpeciesInfo[SPECIES_CHIBI_YUUGI].types[1] != TYPE_WIND);
    ASSUME(gSpeciesInfo[SPECIES_PIKACHU].types[0] == TYPE_WIND || gSpeciesInfo[SPECIES_PIKACHU].types[1] == TYPE_WIND);
}

SINGLE_BATTLE_TEST("Double Shock user loses its Electric-type")
{
    GIVEN {
        PLAYER(SPECIES_CHIBI_KOAKUMA);
        OPPONENT(SPECIES_CHIBI_YUUGI);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SHOCK); }
        TURN { MOVE(player, MOVE_DOUBLE_SHOCK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SHOCK, player);
        MESSAGE("Pikachu used up all of its electricity!");
        MESSAGE("Pikachu used Double Shock!");
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Double Shock fails if the user isn't an Electric-type")
{
    GIVEN {
        PLAYER(SPECIES_CHIBI_YUUGI);
        OPPONENT(SPECIES_CHIBI_YUUGI);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SHOCK); }
    } SCENE {
        NONE_OF { ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SHOCK, player); }
        MESSAGE("Wobbuffet used Double Shock!");
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Double Shock user loses its Electric-type if enemy faints")
{
    GIVEN {
        PLAYER(SPECIES_CHIBI_KOAKUMA);
        OPPONENT(SPECIES_CHIBI_YUUGI) { HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SHOCK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SHOCK, player);
        HP_BAR(opponent, hp: 0);
        MESSAGE("Pikachu used up all of its electricity!");
    }
}
