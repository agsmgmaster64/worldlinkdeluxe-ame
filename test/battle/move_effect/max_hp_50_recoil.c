#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_STEEL_BEAM].effect == EFFECT_MAX_HP_50_RECOIL);
}

SINGLE_BATTLE_TEST("Steel Beam causes the user to take damage equal to half of its maximum HP")
{
    s16 recoilDamage;

    GIVEN {
        PLAYER(SPECIES_CHIBI_YUUGI);
        OPPONENT(SPECIES_CHIBI_YUUGI);
    } WHEN {
        TURN { MOVE(player, MOVE_STEEL_BEAM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_BEAM, player);
        HP_BAR(opponent);
        HP_BAR(player, captureDamage: &recoilDamage);
    } THEN {
        EXPECT_EQ(player->maxHP / 2, recoilDamage);
    }
}

SINGLE_BATTLE_TEST("Steel Beam hp loss is prevented by Magic Guard")
{
    GIVEN {
        PLAYER(SPECIES_TECH_SAKUYA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_CHIBI_YUUGI);
    } WHEN {
        TURN { MOVE(player, MOVE_STEEL_BEAM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEEL_BEAM, player);
        HP_BAR(opponent);
        NOT HP_BAR(player);
    }
}
