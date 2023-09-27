static const struct FormChange sVenusaurFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_VENUSAUR_MEGA, ITEM_VENUSAURITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sCharizardFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_CHARIZARD_MEGA_X, ITEM_CHARIZARDITE_X},
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_CHARIZARD_MEGA_Y, ITEM_CHARIZARDITE_Y},
    {FORM_CHANGE_TERMINATOR},
};

//static const struct FormChange sMegaTemplateFormChangeTable[] = {
    //{FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_RANDOM_MEGA, ITEM_RANDOMITE},
    //{FORM_CHANGE_TERMINATOR},
//};

static const struct FormChange sCastformFormChangeTable[] = {
#if B_WEATHER_FORMS >= GEN_5
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_SUNNY, B_WEATHER_SUN, ABILITY_FORECAST},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_RAINY, B_WEATHER_RAIN, ABILITY_FORECAST},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_SNOWY, B_WEATHER_HAIL | B_WEATHER_SNOW, ABILITY_FORECAST},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM, ~(B_WEATHER_SUN | B_WEATHER_RAIN | B_WEATHER_HAIL | B_WEATHER_SNOW), ABILITY_FORECAST},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM, B_WEATHER_NONE, ABILITY_FORECAST},
#else
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_SUNNY, B_WEATHER_SUN},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_RAINY, B_WEATHER_RAIN},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM_SNOWY, B_WEATHER_HAIL | B_WEATHER_SNOW},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM, ~(B_WEATHER_SUN | B_WEATHER_RAIN | B_WEATHER_HAIL | B_WEATHER_SNOW)},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CASTFORM, B_WEATHER_NONE},
#endif
    {FORM_CHANGE_BATTLE_SWITCH,                 SPECIES_CASTFORM},
    {FORM_CHANGE_FAINT,                         SPECIES_CASTFORM},
    {FORM_CHANGE_END_BATTLE,                    SPECIES_CASTFORM},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sKyogreFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_PRIMAL_REVERSION,   SPECIES_KYOGRE_PRIMAL, ITEM_BLUE_ORB},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sGroudonFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_PRIMAL_REVERSION,   SPECIES_GROUDON_PRIMAL, ITEM_RED_ORB},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sRayquazaFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_MOVE,    SPECIES_RAYQUAZA_MEGA, MOVE_DRAGON_ASCENT},
    {FORM_CHANGE_TERMINATOR},
};

#if P_GEN_4_POKEMON == TRUE
static const struct FormChange sBurmyFormChangeTable[] = {
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY,             BATTLE_TERRAIN_GRASS},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY,             BATTLE_TERRAIN_LONG_GRASS},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY,             BATTLE_TERRAIN_POND},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY,             BATTLE_TERRAIN_MOUNTAIN},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY,             BATTLE_TERRAIN_PLAIN},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY_SANDY_CLOAK, BATTLE_TERRAIN_CAVE},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY_SANDY_CLOAK, BATTLE_TERRAIN_SAND},
    {FORM_CHANGE_END_BATTLE_TERRAIN, SPECIES_BURMY_TRASH_CLOAK, BATTLE_TERRAIN_BUILDING},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sCherrimFormChangeTable[] = {
#if B_WEATHER_FORMS >= GEN_5
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM_SUNSHINE, B_WEATHER_SUN, ABILITY_FLOWER_GIFT},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM, ~B_WEATHER_SUN, ABILITY_FLOWER_GIFT},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM, B_WEATHER_NONE, ABILITY_FLOWER_GIFT},
#else
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM_SUNSHINE, B_WEATHER_SUN},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM, ~B_WEATHER_SUN},
    {FORM_CHANGE_BATTLE_WEATHER,                SPECIES_CHERRIM, B_WEATHER_NONE},
#endif
    {FORM_CHANGE_BATTLE_SWITCH,                 SPECIES_CHERRIM},
    {FORM_CHANGE_FAINT,                         SPECIES_CHERRIM},
    {FORM_CHANGE_END_BATTLE,                    SPECIES_CHERRIM},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sLopunnyFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_LOPUNNY_MEGA, ITEM_LOPUNNITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sGarchompFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_GARCHOMP_MEGA, ITEM_GARCHOMPITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sLucarioFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_LUCARIO_MEGA, ITEM_LUCARIONITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sAbomasnowFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_ABOMASNOW_MEGA, ITEM_ABOMASITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sGalladeFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_GALLADE_MEGA, ITEM_GALLADITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sDialgaFormChangeTable[] = {
    {FORM_CHANGE_ITEM_HOLD, SPECIES_DIALGA, ITEM_NONE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_DIALGA_ORIGIN, ITEM_ADAMANT_CRYSTAL},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sPalkiaFormChangeTable[] = {
    {FORM_CHANGE_ITEM_HOLD, SPECIES_PALKIA, ITEM_NONE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_PALKIA_ORIGIN, ITEM_LUSTROUS_GLOBE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sGiratinaFormChangeTable[] = {
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GIRATINA, ITEM_NONE},
#if I_GRISEOUS_ORB_FORM_CHANGE < GEN_9
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GIRATINA_ORIGIN, ITEM_GRISEOUS_ORB},
#endif
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GIRATINA_ORIGIN, ITEM_GRISEOUS_CORE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sShayminFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_SHAYMIN_SKY, ITEM_GRACIDEA, DAY},
    // {FORM_CHANGE_WITHDRAW, SPECIES_SHAYMIN},
    {FORM_CHANGE_TERMINATOR},
};
#endif

#if P_GEN_5_POKEMON == TRUE
static const struct FormChange sAudinoFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_AUDINO_MEGA, ITEM_AUDINITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sDarmanitanFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_DARMANITAN,          ABILITY_ZEN_MODE, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_DARMANITAN_ZEN_MODE, ABILITY_ZEN_MODE, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_FAINT,             SPECIES_DARMANITAN},
    {FORM_CHANGE_END_BATTLE,        SPECIES_DARMANITAN},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sDarmanitanGalarianFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_DARMANITAN_GALARIAN,          ABILITY_ZEN_MODE, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_DARMANITAN_ZEN_MODE_GALARIAN, ABILITY_ZEN_MODE, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_FAINT,             SPECIES_DARMANITAN_GALARIAN},
    {FORM_CHANGE_END_BATTLE,        SPECIES_DARMANITAN_GALARIAN},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sMeloettaFormChangeTable[] = {
    {FORM_CHANGE_FAINT,         SPECIES_MELOETTA},
    {FORM_CHANGE_END_BATTLE,    SPECIES_MELOETTA},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sTornadusFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_TORNADUS_THERIAN, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_ITEM_USE, SPECIES_TORNADUS, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sThundurusFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_THUNDURUS_THERIAN, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_ITEM_USE, SPECIES_THUNDURUS, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sLandorusFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_LANDORUS_THERIAN, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_ITEM_USE, SPECIES_LANDORUS, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sKeldeoFormChangeTable[] = {
    // {FORM_CHANGE_MOVE, SPECIES_KELDEO_RESOLUTE, MOVE_SECRET_SWORD, WHEN_LEARNED},
    // {FORM_CHANGE_MOVE, SPECIES_KELDEO, MOVE_SECRET_SWORD, WHEN_FORGOTTEN},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sGenesectFormChangeTable[] = {
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GENESECT,             ITEM_NONE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GENESECT_DOUSE_DRIVE, ITEM_DOUSE_DRIVE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GENESECT_SHOCK_DRIVE, ITEM_SHOCK_DRIVE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GENESECT_BURN_DRIVE,  ITEM_BURN_DRIVE},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_GENESECT_CHILL_DRIVE, ITEM_CHILL_DRIVE},
    {FORM_CHANGE_TERMINATOR},
};
#endif

#if P_GEN_6_POKEMON == TRUE
static const struct FormChange sGreninjaBattleBondFormChangeTable[] = {
    {FORM_CHANGE_FAINT,         SPECIES_GRENINJA_BATTLE_BOND},
    {FORM_CHANGE_END_BATTLE,    SPECIES_GRENINJA_BATTLE_BOND},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sAegislashFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_SWITCH, SPECIES_AEGISLASH},
    {FORM_CHANGE_FAINT,         SPECIES_AEGISLASH},
    {FORM_CHANGE_END_BATTLE,    SPECIES_AEGISLASH},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sXerneasFormChangeTable[] = {
    {FORM_CHANGE_BEGIN_BATTLE, SPECIES_XERNEAS_ACTIVE},
    {FORM_CHANGE_END_BATTLE,   SPECIES_XERNEAS,      },
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sZygardePowerConstructFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_ZYGARDE_COMPLETE, ABILITY_POWER_CONSTRUCT, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_FAINT},
    {FORM_CHANGE_END_BATTLE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sDiancieFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,    SPECIES_DIANCIE_MEGA, ITEM_DIANCITE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sHoopaFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_HOOPA_UNBOUND, ITEM_PRISON_BOTTLE, SPECIES_HOOPA},
    {FORM_CHANGE_WITHDRAW, SPECIES_HOOPA},
    {FORM_CHANGE_TERMINATOR},
};
#endif

#if P_GEN_7_POKEMON == TRUE
static const struct FormChange sOricorioFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_ORICORIO,         ITEM_RED_NECTAR},
    {FORM_CHANGE_ITEM_USE, SPECIES_ORICORIO_POM_POM, ITEM_YELLOW_NECTAR},
    {FORM_CHANGE_ITEM_USE, SPECIES_ORICORIO_PAU,     ITEM_PINK_NECTAR},
    {FORM_CHANGE_ITEM_USE, SPECIES_ORICORIO_SENSU,   ITEM_PURPLE_NECTAR},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sWishiwashiFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_WISHIWASHI_SCHOOL, ABILITY_SCHOOLING, HP_HIGHER_THAN,   25},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_WISHIWASHI,        ABILITY_SCHOOLING, HP_LOWER_EQ_THAN, 25},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_WISHIWASHI},
    {FORM_CHANGE_FAINT,             SPECIES_WISHIWASHI},
    {FORM_CHANGE_END_BATTLE,        SPECIES_WISHIWASHI},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sSilvallyFormChangeTable[] = {
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY,          ITEM_NONE,            ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_FIGHTING, ITEM_FIGHTING_MEMORY, ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_FLYING,   ITEM_FLYING_MEMORY,   ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_POISON,   ITEM_POISON_MEMORY,   ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_GROUND,   ITEM_GROUND_MEMORY,   ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_ROCK,     ITEM_ROCK_MEMORY,     ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_BUG,      ITEM_BUG_MEMORY,      ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_GHOST,    ITEM_GHOST_MEMORY,    ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_STEEL,    ITEM_STEEL_MEMORY,    ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_FIRE,     ITEM_FIRE_MEMORY,     ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_WATER,    ITEM_WATER_MEMORY,    ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_GRASS,    ITEM_GRASS_MEMORY,    ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_ELECTRIC, ITEM_ELECTRIC_MEMORY, ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_PSYCHIC,  ITEM_PSYCHIC_MEMORY,  ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_ICE,      ITEM_ICE_MEMORY,      ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_DRAGON,   ITEM_DRAGON_MEMORY,   ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_DARK,     ITEM_DARK_MEMORY,     ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_ITEM_HOLD, SPECIES_SILVALLY_FAIRY,    ITEM_FAIRY_MEMORY,    ABILITY_RKS_SYSTEM},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sMimikyuFormChangeTable[] = {
    {FORM_CHANGE_FAINT,         SPECIES_MIMIKYU},
    {FORM_CHANGE_END_BATTLE,    SPECIES_MIMIKYU},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sMiniorRedFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR,          ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_RED, ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_RED},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_RED},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_RED},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorBlueFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_BLUE, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_BLUE,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_BLUE},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_BLUE},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_BLUE},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorGreenFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_GREEN, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_GREEN,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_GREEN},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_GREEN},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_GREEN},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorIndigoFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_INDIGO, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_INDIGO,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_INDIGO},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_INDIGO},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_INDIGO},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorOrangeFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_ORANGE, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_ORANGE,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_ORANGE},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_ORANGE},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_ORANGE},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorVioletFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_VIOLET, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_VIOLET,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_VIOLET},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_VIOLET},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_VIOLET},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sMiniorYellowFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_METEOR_YELLOW, ABILITY_SHIELDS_DOWN, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_MINIOR_CORE_YELLOW,   ABILITY_SHIELDS_DOWN, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MINIOR_CORE_YELLOW},
    {FORM_CHANGE_FAINT,             SPECIES_MINIOR_CORE_YELLOW},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MINIOR_CORE_YELLOW},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sNecrozmaDuskManeFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_ULTRA_BURST,    SPECIES_NECROZMA_ULTRA, ITEM_ULTRANECROZIUM_Z},
    {FORM_CHANGE_TERMINATOR},
};
static const struct FormChange sNecrozmaDawnWingsFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_ULTRA_BURST,    SPECIES_NECROZMA_ULTRA, ITEM_ULTRANECROZIUM_Z},
    {FORM_CHANGE_TERMINATOR},
};
#endif

#if P_GEN_8_POKEMON == TRUE
static const struct FormChange sCramorantFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_CRAMORANT_GULPING, ABILITY_GULP_MISSILE, HP_HIGHER_THAN,   50},
    {FORM_CHANGE_BATTLE_HP_PERCENT, SPECIES_CRAMORANT_GORGING, ABILITY_GULP_MISSILE, HP_LOWER_EQ_THAN, 50},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_CRAMORANT},
    {FORM_CHANGE_FAINT,             SPECIES_CRAMORANT},
    {FORM_CHANGE_END_BATTLE,        SPECIES_CRAMORANT},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sEiscueFormChangeTable[] = {
    {FORM_CHANGE_FAINT,             SPECIES_EISCUE},
    {FORM_CHANGE_END_BATTLE,        SPECIES_EISCUE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sMorpekoFormChangeTable[] = {
    {FORM_CHANGE_BATTLE_TURN_END,   SPECIES_MORPEKO_HANGRY, ABILITY_HUNGER_SWITCH},
    {FORM_CHANGE_BATTLE_TURN_END,   SPECIES_MORPEKO       , ABILITY_HUNGER_SWITCH},
    {FORM_CHANGE_BATTLE_SWITCH,     SPECIES_MORPEKO},
    {FORM_CHANGE_FAINT,             SPECIES_MORPEKO},
    {FORM_CHANGE_END_BATTLE,        SPECIES_MORPEKO},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sZacianFormChangeTable[] = {
    {FORM_CHANGE_BEGIN_BATTLE, SPECIES_ZACIAN_CROWNED_SWORD, ITEM_RUSTED_SWORD, MOVE_DRAWN_LINE,      MOVE_BEHEMOTH_BLADE},
    {FORM_CHANGE_END_BATTLE,   SPECIES_ZACIAN,               ITEM_RUSTED_SWORD, MOVE_BEHEMOTH_BLADE, MOVE_DRAWN_LINE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sZamazentaFormChangeTable[] = {
    {FORM_CHANGE_BEGIN_BATTLE, SPECIES_ZAMAZENTA_CROWNED_SHIELD, ITEM_RUSTED_SHIELD, MOVE_DRAWN_LINE,      MOVE_BEHEMOTH_BASH},
    {FORM_CHANGE_END_BATTLE,   SPECIES_ZAMAZENTA,                ITEM_RUSTED_SHIELD, MOVE_BEHEMOTH_BASH,  MOVE_DRAWN_LINE},
    {FORM_CHANGE_TERMINATOR},
};

static const struct FormChange sEnamorusFormChangeTable[] = {
    {FORM_CHANGE_ITEM_USE, SPECIES_ENAMORUS, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_ITEM_USE, SPECIES_ENAMORUS_THERIAN, ITEM_REVEAL_GLASS},
    {FORM_CHANGE_TERMINATOR},
};
#endif

#undef WHEN_LEARNED
#undef WHEN_FORGOTTEN
