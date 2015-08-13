#include "cGameModel.h"


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
        f >> garbage >> garbage >> UnitMovementSpeed[i];
        f >> garbage >> garbage >> health[i];
        f >> garbage >> garbage >> mana[i];
        f >> garbage >> garbage >> healthreg[i];
        f >> garbage >> garbage >> manareg[i];
        f >> garbage >> garbage >> damage[i];
        f >> garbage >> garbage >> sightRange[i];
        f >> garbage >> garbage >> Armor[i];
        f >> garbage >> garbage >> Magicarmor[i];
        f >> garbage >> garbage >> tauntRange[i];
        f >> garbage >> garbage >> sortPriority[i];
        f >> garbage >> garbage >> UnitCost[i];
        f >> garbage >> garbage >> BuildingTime[i];
        f >> garbage >> NumberOfAblilites[i];

        Error("Movement speed ",UnitMovementSpeed[i]);
        Error("health  ",health[i]);
        Error("mana",mana[i]);
        Error("healthreg",healthreg[i]);
        Error("manareg",manareg[i]);
        Error("damage",damage[i]);
        Error("sightRange",sightRange[i]);
        Error("Armor",Armor[i]);
        Error("Magicarmor",Magicarmor[i]);
        Error("tauntRange",tauntRange[i]);
        Error("NumberOfAblilites",NumberOfAblilites[i]);

        for(int j=0;j<NumberOfAblilites[i];j++)
        {
            f >> garbage >> garbage;
            f >> garbage >> garbage >> attackSpeed[i][j];
            f >> garbage >> garbage >> attackRange[i][j];
            f >> garbage >> garbage >> UnitEntityType[i][j];
            f >> garbage >> garbage >> ManaCost[i][j];
            f >> garbage;

        Error("attackSpeed",attackSpeed[i][j]);
        Error("attackRange",attackRange[i][j]);
        Error("ManaCost",ManaCost[i][j]);
        Error("UnitEntityType",UnitEntityType[i][j]);
        }

        f >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage ;

        for(int j=0;j<NMBR_ACTIONS;j++)
        {
            f >> garbage;
            f >> garbage >> UnitAnimSteps[i][j];
            f >> garbage >> UnitAnimDuration[i][j];
            Error("Command ID j",j);
            Error("UnitAnimDuration[i][j]",UnitAnimDuration[i][j]);
        }
        f.close();
    }

    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f.open(BUILDING_SPEC_FILE[i].c_str());
        f >> garbage;
        f >> BuildingSizeX[i] >> BuildingSizeY[i];
        f >> garbage;
        for(int j=0;j<BUILDING_MAX_WIDTH;j++)
        {
            for(int k=0;k<BUILDING_MAX_HEIGHT;k++)
            {
                f >> BuildingLayoutStorgage[i][k][j];
            }
        }
        ///TODO READ STUFF
        f.close();
    }

    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f.open(ENTITY_SPEC_FILE[i].c_str());
        f >> garbage >> garbage >> garbage >> garbage >> garbage;
        f >> garbage >> garbage >> EntitynumModifiers[i];
        f >> garbage >> garbage >> AnimNumberOfSteps[i];
        f >> garbage >> garbage >> AnimDuration[i];
        f >> garbage >> garbage >> lifeTime[i];
        f >> garbage >> garbage >> startaway[i];

        Error(" ");
        Error(" Entity",i);
        Error("EntitynumModifiers[i]",EntitynumModifiers[i]);
        Error("AnimNumberOfSteps[i]",AnimNumberOfSteps[i]);
        Error("AnimDuration[i]",AnimDuration[i]);
        //Error("lifeTime[i]",lifeTime[i]);
        //Error("startaway[i]",startaway[i]);

        for(int j=0;j<EntitynumModifiers[i];j++)
        {
            f >> garbage >> garbage >> modifierType[i][j];
            f >> garbage >> garbage >> modifierValue[i][j];

            Error(" ");
            Error(" modifier",j);
            Error("modifierType[i][j]",modifierType[i][j]);
            Error("modifierValue[i][j]",modifierValue[i][j]);

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
void cGameModel::CreateUnit(unsigned int type, int xPosi,int yPosi,int owner)
{
    cUnit* newUnit;
    newUnit = new cUnit(checkFreeUnitID(),type,owner,MapObj.getMap(xPosi,yPosi),UnitAnimSteps[type],UnitAnimDuration[type]
                        ,UnitMovementSpeed[type],attackSpeed[type],health[type],damage[type],sightRange[type]
                        ,tauntRange[type],attackRange[type], UnitEntityType[type],ManaCost[type],NumberOfAblilites[type]
                        ,mana[type],manareg[type],healthreg[type],Armor[type],Magicarmor[type],sortPriority[type],&MapObj);
    //put it onto the ID List
    ///TODO dynamic ID system
    if(ID < MAX_UNITS)
    {
        IDListed[ID] = newUnit;
    }
    //put it onto the map
    MapObj.getMap(xPosi,yPosi)->setContain(newUnit,NULL);
}

void cGameModel::CreateBuilding(int type,int xPosi, int yPosi, int owner)
{
    cBuilding* newBuilding;
    buildingLayout lay;
    for(int i=0;i<BuildingSizeX[type];i++)
    {
        for(int j=0;j<BuildingSizeY[type];j++)
        {
            lay.layoutSetting[i][j] = BuildingLayoutStorgage[type][i][j];
        }
    }
    Error2("BuildingSizeX[type] ",BuildingSizeX[type]);
    Error2("BuildingSizeY[type] ",BuildingSizeY[type]);
    newBuilding = new cBuilding(type, owner, BuildingSizeX[type], BuildingSizeY[type]);

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
    if(modifierType[Enttype][0] >= ENT_PROP_AOE && modifierType[Enttype][0] < ENT_PROP_SELF)
    {
        //if so get its range and find pixels within range
        range = sqrt(modifierValue[Enttype][0]);
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
                    newEnt = new cEntity(srx,sry,MapObj.getMap(goalX+i,goalY+j),NULL,lifeTime[Enttype],Enttype,EntitynumModifiers[Enttype],
                                    modifierType[Enttype],modifierValue[Enttype],AnimNumberOfSteps[Enttype],AnimDuration[Enttype]);
                    newEnt->Init(&MapObj,startaway[Enttype],distanceX,distanceY,1);
                    NumberOfEntities++;
                    EntityList[NumberOfEntities-1] = newEnt;
                }
            }
        }
    }
    else
    {
        newEnt = new cEntity(srx,sry,goalTile,goalUnit,lifeTime[Enttype],Enttype,EntitynumModifiers[Enttype],
                         modifierType[Enttype],modifierValue[Enttype],AnimNumberOfSteps[Enttype],AnimDuration[Enttype]);
        newEnt->Init(&MapObj,startaway[Enttype],distanceX,distanceY,1);
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
