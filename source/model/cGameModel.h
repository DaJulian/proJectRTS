#ifndef CGAMEMODEL_H_INCLUDED
#define CGAMEMODEL_H_INCLUDED

#include "cWorldMap.h"
#include "cEntity.h"
#include "cBuilding.h"

struct sUnitSpecifications
{
    int UnitMovementSpeed[UNIT_TYPE_MAX];
    int health[UNIT_TYPE_MAX];
    int mana[UNIT_TYPE_MAX];
    int healthreg[UNIT_TYPE_MAX];
    int manareg[UNIT_TYPE_MAX];
    int damage[UNIT_TYPE_MAX];
    int sightRange[UNIT_TYPE_MAX];
    int Armor[UNIT_TYPE_MAX];
    int Magicarmor[UNIT_TYPE_MAX];
    int tauntRange[UNIT_TYPE_MAX];
    int sortPriority[UNIT_TYPE_MAX];
    int NumberOfAblilites[UNIT_TYPE_MAX];
    int UnitCost[UNIT_TYPE_MAX];
    int BuildingTime[UNIT_TYPE_MAX];

    int attackSpeed[UNIT_TYPE_MAX][ABILITY_MAX];
    int attackRange[UNIT_TYPE_MAX][ABILITY_MAX];
    int ManaCost[UNIT_TYPE_MAX][ABILITY_MAX];
    int UnitEntityType[UNIT_TYPE_MAX][ABILITY_MAX];

    int UnitAnimSteps[UNIT_TYPE_MAX][NMBR_ACTIONS];
    int UnitAnimDuration[UNIT_TYPE_MAX][NMBR_ACTIONS];
};

struct sBuildingSpecifications
{
    int BuildingSizeX[BUILDING_TYPE_MAX];
    int BuildingSizeY[BUILDING_TYPE_MAX];
    int BuildingLayoutStorgage[BUILDING_TYPE_MAX][BUILDING_MAX_WIDTH][BUILDING_MAX_HEIGHT];
};

struct sEntitySpecifications
{
    int EntitynumModifiers[ENTITY_TYPE_MAX];
    int modifierType[ENTITY_TYPE_MAX][ENT_MAX_MODIF];
    int modifierValue[ENTITY_TYPE_MAX][ENT_MAX_MODIF];
    int AnimNumberOfSteps[ENTITY_TYPE_MAX];
    int AnimDuration[ENTITY_TYPE_MAX];
    Uint32 lifeTime[ENTITY_TYPE_MAX];
    Uint32 startaway[ENTITY_TYPE_MAX];
};

class cGameModel
{
protected:
    cWorldMap MapObj;

    int NumberOfEntities;
    cEntity* EntityList[MAX_ENT];

    cUnit* IDListed[MAX_UNITS];
    int newestID;

    sUnitSpecifications unitSpecifications;
    sBuildingSpecifications buildingSpecifications;
    sEntitySpecifications entitySpecifications;

public:
    cGameModel();
    void InitGameModel();
    void InitMap(std::string filename, cChangeList* pChList);

    cUnit* getUnitByID(unsigned int ID);
    int checkFreeUnitID();
    void setNewestID(int id);

    void CreateUnit(unsigned int ID, unsigned int type, int xPosi,int yPosi, int owner);
    void DestroyUnit(cUnit* Unit);

    void CreateBuilding(int type, int xPosi, int yPosi, int owner);

    void createEntity(int Enttype, int srx, int sry, cTile* goalTile, cUnit* goalUnit);
    void removeEntity(int num);

    cWorldMap* getMapObj();
    cTile* getMap(int x, int y);

    cEntity* getEntity(int num);
    int getNumberOfEntities();

    sUnitSpecifications* getUnitSpecifications();
    sBuildingSpecifications* getBuildingSpecifications();
    sEntitySpecifications* getEntitySpecifications();
};

#endif // CGAMEMODEL_H_INCLUDED
