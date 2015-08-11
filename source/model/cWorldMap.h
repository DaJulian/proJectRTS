#ifndef CWORLDMAP_H_INCLUDED
#define CWORLDMAP_H_INCLUDED

#include "cTile.h"
#include "../netControl/oComdCntrList.h"

class cChangeList;

class cWorldMap
{
protected:
    cTile* Map[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];
public:
    cWorldMap();
    void InitMap(std::string filename,cChangeList* pChList,cWorldMap* pMap);

    cTile* getMap(int x,int y);
    void SetMap(int x,int y, cTile* Til);

    void ClearMap();
};

#endif // CWORLDMAP_H_INCLUDED
