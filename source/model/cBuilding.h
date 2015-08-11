#ifndef CBUILDING_H_INCLUDED
#define CBUILDING_H_INCLUDED

#include "../global.h"
#include "cTile.h"
#include "cWorldMap.h"

class cTile;
class cWorldMap;

struct buildingLayout
{
    int layoutSetting[BUILDING_MAX_WIDTH][BUILDING_MAX_HEIGHT];
    cTile* layoutTiles[BUILDING_MAX_WIDTH][BUILDING_MAX_HEIGHT];
};

class cBuilding
{
protected:
    buildingLayout layout;

    int sizeX;
    int sizeY;
    int owner;
    int type;
    bool accessedByActivePlayer;
    int posiX;
    int posiY;
public:
    cBuilding();
    cBuilding(int typ,int own, int sizX, int sizY);
    void initBuilding(cWorldMap* pWorldMap, int posX, int posY, buildingLayout lay);

    int getType();
    int getOwner();
    void setOwner(int own);

    int getPositionX();
    int getPositionY();

    bool checkAccess(int own);
};

#endif // CBUILDING_H_INCLUDED
