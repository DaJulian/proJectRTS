#include "cTile.h"

cTile::cTile()
{
    type = 0;
    containingUnit = NULL;
    containingObstacle = NULL;
    containType = 0;
}

void cTile::Init(int typ)
{
    type = typ;
}

int cTile::Gettype()
{
    return type;
}

void cTile::setContain(cUnit* obj,cObstacle* obs)
{
    if(obj != NULL)
    {
        if(obs != NULL){Error("critical Warning: Call of cTile::setContain with more then one type");}
        if(containType == OBJ_TYPE_BUILDING_ACCESS)
        {
            containType = OBJ_TYPE_BUILDING_WITH_UNIT;
        }
        else
        {
            containType = OBJ_TYPE_UNIT;
        }
    }
    if(obs != NULL)
    {
        if(obj != NULL){Error("critical Warning: Call of cTile::setContain with more then one type");}
        containType = OBJ_TYPE_OBSTACLE;
    }
    containingUnit = obj;
    containingObstacle = obs;
    if(obs == NULL && obj == NULL)
    {
        if(containingBuilding != NULL)
        {
            containType = OBJ_TYPE_BUILDING_ACCESS;
        }
        else
        {
            containType = 0;
        }
    }

}

cUnit* cTile::getContainUnit()
{
    return containingUnit;
}
cObstacle* cTile::getContainObstacle()
{
    return containingObstacle;
}
cBuilding* cTile::getContainBuilding()
{
    return containingBuilding;
}
void cTile::setContainBuilding(cBuilding* bui,int ty)
{
    containingBuilding = bui;
    containType = ty;
}

int cTile::getContainType()
{
    return containType;
}

void cTile::SetPosition(int x,int y)
{
    posX = x;
    posY = y;
}
int cTile::getX()
{
    return posX;
}
int cTile::getY()
{
    return posY;
}




cTileList::cTileList()
{
    pContent = NULL;
    pLast = NULL;
    pNext = NULL;
}

cTileList::cTileList(cTile* cont,cTileList* suc,cTileList* pre)
{
    pContent = cont;
    pLast = pre;
    pNext = suc;
}

cTile* cTileList::getContent()
{
    return pContent;
}

void cTileList::setContent(cTile* cont)
{
    pContent = cont;
}

cTileList* cTileList::getSuccessor()
{
    return pNext;
}

void cTileList::setSucessor(cTileList* suc)
{
    pNext = suc;
}

cTileList* cTileList::getPredecessor()
{
    return pLast;
}

void cTileList::setPredecessor(cTileList* pre)
{
    pLast = pre;
}
