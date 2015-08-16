#include "cGameModel.h"

cGameModel::cGameModel()
{

}

void cGameModel::InitGameModel()
{
    std::string garbage;
    std::fstream f;

    //Unit Settings; meaning the total number of objects existing in the game
    f.open(UNIT_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);
    f >> garbage >> TILE_TYPE_NUMBER
      >> garbage >> UNIT_TYPE_NUMBER
      >> garbage >> BUILDING_TYPE_NUMBER
      >> garbage >> OBSTACLE_TYPE_NUMBER
      >> garbage >> ENTITY_TYPE_NUMBER;

    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        f >> garbage >> UNIT_SPEC_FILE[i];
    }
    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f >> garbage >> BUILDING_SPEC_FILE[i];
    }
    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f >> garbage >> ENTITY_SPEC_FILE[i];
    }

    Error("TILE_TYPE_NUMBER ",TILE_TYPE_NUMBER);
    Error("UNIT_TYPE_NUMBER ",UNIT_TYPE_NUMBER);
    Error("OBSTACLE_TYPE_NUMBER ",OBSTACLE_TYPE_NUMBER);
    Error("ENTITIY_TYPE_NUMBER ",ENTITY_TYPE_NUMBER);

    f.close();

    //Initialize the unit settings
    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        f.open(UNIT_SPEC_FILE[i].c_str());
        f >> garbage >> garbage >> garbage >> garbage >> garbage;
        f >> garbage >> garbage >> unitSpecifications.UnitMovementSpeed[i];
        f >> garbage >> garbage >> unitSpecifications.health[i];
        f >> garbage >> garbage >> unitSpecifications.mana[i];
        f >> garbage >> garbage >> unitSpecifications.healthreg[i];
        f >> garbage >> garbage >> unitSpecifications.manareg[i];
        f >> garbage >> garbage >> unitSpecifications.damage[i];
        f >> garbage >> garbage >> unitSpecifications.sightRange[i];
        f >> garbage >> garbage >> unitSpecifications.Armor[i];
        f >> garbage >> garbage >> unitSpecifications.Magicarmor[i];
        f >> garbage >> garbage >> unitSpecifications.tauntRange[i];
        f >> garbage >> garbage >> unitSpecifications.sortPriority[i];
        f >> garbage >> garbage >> unitSpecifications.UnitCost[i];
        f >> garbage >> garbage >> unitSpecifications.BuildingTime[i];
        f >> garbage >> unitSpecifications.NumberOfAblilites[i];

        /*Error("Movement speed ",UnitMovementSpeed[i]);
        Error("health  ",health[i]);
        Error("mana",mana[i]);
        Error("healthreg",healthreg[i]);
        Error("manareg",manareg[i]);
        Error("damage",damage[i]);
        Error("sightRange",sightRange[i]);
        Error("Armor",Armor[i]);
        Error("Magicarmor",Magicarmor[i]);
        Error("tauntRange",tauntRange[i]);
        Error("NumberOfAblilites",NumberOfAblilites[i]);*/

        for(int j=0;j<unitSpecifications.NumberOfAblilites[i];j++)
        {
            f >> garbage >> garbage;
            f >> garbage >> garbage >> unitSpecifications.attackSpeed[i][j];
            f >> garbage >> garbage >> unitSpecifications.attackRange[i][j];
            f >> garbage >> garbage >> unitSpecifications.UnitEntityType[i][j];
            f >> garbage >> garbage >> unitSpecifications.ManaCost[i][j];
            f >> garbage;

        /*Error("attackSpeed",attackSpeed[i][j]);
        Error("attackRange",attackRange[i][j]);
        Error("ManaCost",ManaCost[i][j]);
        Error("UnitEntityType",UnitEntityType[i][j]);*/
        }

        f >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage ;

        for(int j=0;j<NMBR_ACTIONS;j++)
        {
            f >> garbage;
            f >> garbage >> unitSpecifications.UnitAnimSteps[i][j];
            f >> garbage >> unitSpecifications.UnitAnimDuration[i][j];
            Error("Command ID j",j);
            Error("UnitAnimDuration[i][j]",unitSpecifications.UnitAnimDuration[i][j]);
        }
        f.close();
    }

    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f.open(BUILDING_SPEC_FILE[i].c_str());
        f >> garbage;
        f >> buildingSpecifications.BuildingSizeX[i] >> buildingSpecifications.BuildingSizeY[i];
        f >> garbage;
        for(int j=0;j<BUILDING_MAX_WIDTH;j++)
        {
            for(int k=0;k<BUILDING_MAX_HEIGHT;k++)
            {
                f >> buildingSpecifications.BuildingLayoutStorgage[i][k][j];
            }
        }
        ///TODO READ STUFF
        f.close();
    }

    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f.open(ENTITY_SPEC_FILE[i].c_str());
        f >> garbage >> garbage >> garbage >> garbage >> garbage;
        f >> garbage >> garbage >> entitySpecifications.EntitynumModifiers[i];
        f >> garbage >> garbage >> entitySpecifications.AnimNumberOfSteps[i];
        f >> garbage >> garbage >> entitySpecifications.AnimDuration[i];
        f >> garbage >> garbage >> entitySpecifications.lifeTime[i];
        f >> garbage >> garbage >> entitySpecifications.startaway[i];

        Error(" ");
        Error(" Entity",i);
        Error("EntitynumModifiers[i]",entitySpecifications.EntitynumModifiers[i]);
        Error("AnimNumberOfSteps[i]",entitySpecifications.AnimNumberOfSteps[i]);
        Error("AnimDuration[i]",entitySpecifications.AnimDuration[i]);
        //Error("lifeTime[i]",lifeTime[i]);
        //Error("startaway[i]",startaway[i]);

        for(int j=0;j<entitySpecifications.EntitynumModifiers[i];j++)
        {
            f >> garbage >> garbage >> entitySpecifications.modifierType[i][j];
            f >> garbage >> garbage >> entitySpecifications.modifierValue[i][j];

            Error(" ");
            Error(" modifier",j);
            Error("modifierType[i][j]",entitySpecifications.modifierType[i][j]);
            Error("modifierValue[i][j]",entitySpecifications.modifierValue[i][j]);

        }
        f.close();
    }

    //safely assign initial values to the entity list
    NumberOfEntities = 0;
    for(int i=0;i< MAX_ENT;i++)
    {
        EntityList[i] = NULL;
    }
}

//routines to create Units on the map, dependent on S/C
void cGameModel::CreateUnit(unsigned int ID,unsigned int type, int xPosi,int yPosi,int owner)
{
    cUnit* newUnit;
    newUnit = new cUnit(ID,type,owner,MapObj.getMap(xPosi,yPosi),unitSpecifications.UnitAnimSteps[type]
                        ,unitSpecifications.UnitAnimDuration[type]
                        ,unitSpecifications.UnitMovementSpeed[type],unitSpecifications.attackSpeed[type],
                        unitSpecifications.health[type],unitSpecifications.damage[type],unitSpecifications.sightRange[type]
                        ,unitSpecifications.tauntRange[type],unitSpecifications.attackRange[type],
                        unitSpecifications.UnitEntityType[type],unitSpecifications.ManaCost[type],
                        unitSpecifications.NumberOfAblilites[type]
                        ,unitSpecifications.mana[type],unitSpecifications.manareg[type],unitSpecifications.healthreg[type],
                        unitSpecifications.Armor[type],unitSpecifications.Magicarmor[type],unitSpecifications.sortPriority[type],&MapObj);
    //put it onto the ID List
    ///TODO dynamic ID system
    if(ID < MAX_UNITS)
    {
        IDListed[ID] = newUnit;
    }
    //put it onto the map
    MapObj.getMap(xPosi,yPosi)->setContain(newUnit,NULL);
}

void cGameModel::DestroyUnit(cUnit* Unit)
{
    Error("killing unit");
    Unit->getPosition()->setContain(NULL,NULL);
    IDListed[Unit->getID()] = NULL;
    delete Unit;
}

void cGameModel::CreateBuilding(int type,int xPosi, int yPosi, int owner)
{
    cBuilding* newBuilding;
    buildingLayout lay;
    for(int i=0;i<buildingSpecifications.BuildingSizeX[type];i++)
    {
        for(int j=0;j<buildingSpecifications.BuildingSizeY[type];j++)
        {
            lay.layoutSetting[i][j] = buildingSpecifications.BuildingLayoutStorgage[type][i][j];
        }
    }
    Error2("BuildingSizeX[type] ",buildingSpecifications.BuildingSizeX[type]);
    Error2("BuildingSizeY[type] ",buildingSpecifications.BuildingSizeY[type]);
    newBuilding = new cBuilding(type, owner, buildingSpecifications.BuildingSizeX[type], buildingSpecifications.BuildingSizeY[type]);

    newBuilding->initBuilding(&MapObj, xPosi, yPosi, lay);
}

cUnit* cGameModel::getUnitByID(unsigned int ID)
{
    if(ID < MAX_UNITS)
    {
        return IDListed[ID];
    }
    else
    {
        return NULL;
    }
    return NULL;
}

void cGameModel::createEntity(int Enttype,int srx, int sry, cTile* goalTile, cUnit* goalUnit)
{
    Error("Create Entity");
    cEntity* newEnt;
    int distanceX,distanceY;
    int goalX,goalY,rangeInt;
    float range;
    goalX = goalTile->getX();
    goalY = goalTile->getY();

    distanceX = (srx - goalX)*TILE_WIDTH;// + goalUnit->getOffsetX();
    distanceY = (sry - goalY)*TILE_WIDTH;// + goalUnit->getOffsetY();

    //check if we have an AOE spell;
    if(entitySpecifications.modifierType[Enttype][0] >= ENT_PROP_AOE && entitySpecifications.modifierType[Enttype][0] < ENT_PROP_SELF)
    {
        //if so get its range and find pixels within range
        range = sqrt(entitySpecifications.modifierValue[Enttype][0]);
        rangeInt = int(range);
        for(int i=-rangeInt;i<=rangeInt;i++)
        {
            for(int j=-rangeInt;j<=rangeInt;j++)
            {
                //if in range and if on the map create the entity there
                if((realDist(goalX,goalY,goalX+i,goalY+j) <= range)
                   && goalX+i >= 0 && goalY+j >= 0 && goalX+i < WORLD_WIDTH && goalY+j < WORLD_HEIGHT)
                {
                    distanceX = (srx - goalX-i)*TILE_WIDTH;
                    distanceY = (sry - goalY-j)*TILE_HEIGHT;
                    newEnt = new cEntity(srx,sry,MapObj.getMap(goalX+i,goalY+j),NULL,
                                         entitySpecifications.lifeTime[Enttype],Enttype,entitySpecifications.EntitynumModifiers[Enttype],
                                    entitySpecifications.modifierType[Enttype],entitySpecifications.modifierValue[Enttype],
                                    entitySpecifications.AnimNumberOfSteps[Enttype],entitySpecifications.AnimDuration[Enttype]);
                    newEnt->Init(&MapObj,entitySpecifications.startaway[Enttype],distanceX,distanceY,1);
                    NumberOfEntities++;
                    EntityList[NumberOfEntities-1] = newEnt;
                }
            }
        }
    }
    else
    {
        newEnt = new cEntity(srx,sry,goalTile,goalUnit,entitySpecifications.lifeTime[Enttype],
                             Enttype,entitySpecifications.EntitynumModifiers[Enttype],
                         entitySpecifications.modifierType[Enttype],entitySpecifications.modifierValue[Enttype],
                         entitySpecifications.AnimNumberOfSteps[Enttype],entitySpecifications.AnimDuration[Enttype]);
        newEnt->Init(&MapObj,entitySpecifications.startaway[Enttype],distanceX,distanceY,1);
        NumberOfEntities++;
        EntityList[NumberOfEntities-1] = newEnt;
    }

}

void cGameModel::removeEntity(int num)
{
    cEntity* remEnt;
    remEnt = EntityList[num];
    NumberOfEntities--;
    for(int i=num;i<NumberOfEntities;i++)
    {
        EntityList[i] = EntityList[i+1];
    }
    delete remEnt;
}

int cGameModel::getNumberOfEntities()
{
    return NumberOfEntities;
}

int cGameModel::checkFreeUnitID()
{
    int ID=newestID;
    for(int i=0;i<MAX_UNITS;i++)
    {
        if(IDListed[ID] == NULL)
        {
            return ID;
        }
        ID++;
        if(ID == MAX_UNITS) ID=0;
    }
    return -1;
}

void cGameModel::setNewestID(int id)
{
    newestID = id;
}

cWorldMap* cGameModel::getMapObj()
{
    return &MapObj;
}

cTile* cGameModel::getMap(int x, int y)
{
    return MapObj.getMap(x,y);
}

void cGameModel::InitMap(std::string filename, cChangeList* pChList)
{
    MapObj.InitMap(filename, pChList, &MapObj);
}

cEntity* cGameModel::getEntity(int num)
{
    return EntityList[num];
}

sUnitSpecifications* cGameModel::getUnitSpecifications()
{
    return &unitSpecifications;
}

sBuildingSpecifications* cGameModel::getBuildingSpecifications()
{
    return &buildingSpecifications;
}
sEntitySpecifications* cGameModel::getEntitySpecifications()
{
    return &entitySpecifications;
}
