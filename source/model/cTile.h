#ifndef CTILE_H_INCLUDED
#define CTILE_H_INCLUDED

#include "cUnit.h"
#include "cObstacle.h"

//for cross dependencies (bad programming style) better??
class cUnit;
class cObstacle;
class cBuilding;

class cTile
{
protected:
    //Landscape type
    int type;
    //Containing object
    cUnit* containingUnit;
    cObstacle* containingObstacle;
    cBuilding* containingBuilding;
    int containType;

    //position on the map (TODO is it needed??)
    int posX,posY;
public:
    cTile();

    void Init(int type);
    int Gettype();

    void setContain(cUnit* obj,cObstacle* obs);
    int getContainType();
    cUnit* getContainUnit();
    cObstacle* getContainObstacle();
    cBuilding* getContainBuilding();
    void setContainBuilding(cBuilding* bui,int ty);

    void SetPosition(int x,int y);
    int getX();
    int getY();
};


//an additional List
class cTileList
{
protected:
    cTile* pContent;
    cTileList* pNext;
    cTileList* pLast;
public:
    cTileList();
    cTileList(cTile* cont,cTileList* suc,cTileList* pre);

    cTileList* getPredecessor();
    void setPredecessor(cTileList* pre);

    cTileList* getSuccessor();
    void setSucessor(cTileList* suc);

    cTile* getContent();
    void setContent(cTile* un);
};

#endif // CTILE_H_INCLUDED
