#ifndef GUARD_REGION_MAP_FRLG_H
#define GUARD_REGION_MAP_FRLG_H

#include "global.h"
#include "bg.h"

enum {
    REGIONMAP_TYPE_NORMAL,
    REGIONMAP_TYPE_WALL,
    REGIONMAP_TYPE_FLY,
    REGIONMAP_TYPE_COUNT
};

void InitRegionMapWithExitCB(u8 type, u8 selectedRegion, void (*callback)(void));
void CB2_OpenFlyMap_Frlg(void);

#endif // GUARD_REGION_MAP_FRLG_H
