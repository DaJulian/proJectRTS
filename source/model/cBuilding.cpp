#include "cBuilding.h"

cBuilding::cBuilding()
{
    type = 0;
    owner = 0;
    sizeX = 0;
    sizeY = 0;
}
cBuilding::cBuilding(int typ,int own, int sizX, int sizY)
{
    type = typ;
    owner = own;
    sizeX = sizX;
    sizeY = sizY;
}
void cBuilding::initBuilding(cWorldMap* pWorldMap, int posX, int posY, buildingLayout lay)
{
    layout = lay;
    Error2("initializeing Building");
    for(int i=0;i<sizeX;i++)
    {
        Error2("i ",i);
        for(int j=0;j<sizeY;j++)
        {
            Error2("j ",j);
            layout.layoutTiles[i][j] = pWorldMap->getMap(posX+i,posY+j);
            if(layout.layoutSetting[i][j] == BUILDING_LAYOUT_ACCESS)
            {
                Error2("code ", BUILDING_LAYOUT_ACCESS);
                layout.layoutTiles[i][j]->setContainBuilding(this,OBJ_TYPE_BUILDING_ACCESS);
            }
            if(layout.layoutSetting[i][j] == BUILDING_LAYOUT_WALL)
            {
                Error2("code ", BUILDING_LAYOUT_WALL);
                layout.layoutTiles[i][j]->setContainBuilding(this,OBJ_TYPE_BUILDING_WALL);
            }
        }
    }
    posiX = posX;
    posiY = posY;
}

int cBuilding::getType()
{
    return type;
}
int cBuilding::getOwner()
{
    return owner;
}
void cBuilding::setOwner(int own)
{
    owner = own;
}

bool cBuilding::checkAccess(int own)
{
    for(int i=0;i<sizeX;i++)
    {
        for(int j=0;j<sizeY;j++)
        {
            if(layout.layoutTiles[i][j]->getContainUnit() != NULL)
            {
                if(layout.layoutTiles[i][j]->getContainUnit()->getOwner() == own)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void cBuilding::setSelected(bool sel)
{
    selected = sel;
}
bool cBuilding::checkSelected()
{
    return selected;
}

int cBuilding::getPositionX()
{
    return posiX;
}
int cBuilding::getPositionY()
{
    return posiY;
}
