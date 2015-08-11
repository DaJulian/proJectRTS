#ifndef CCONTROL_H_INCLUDED
#define CCONTROL_H_INCLUDED

#include "cSelectionControl.h"
#include "../model/cWorldMap.h"
#include "cPathFind.h"
#include "../netControl/cNetControl.h"
#include "cReplay.h"
#include "../model/cEntity.h"
#include "../model/cBuilding.h"

class cControl : public cSelectionControl , public cNetControl
{
protected:
    //Run switch; when turned off the framework should shut down
    bool bRun;

    bool bStartSignal;

    Uint32 TimeSinceStart;

    cWorldMap MapObj;
    cPathFind pathFindObj;

    cCommandList commandListObj;
    cChangeList changeListObj;
    cNetPackageList netPackToSend;
    cNetPackageList netPackToStrip;

    cReplay ReplayObj;

    //Unit specifications for initialization
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

    //Building specifications
    int BuildingSizeX[BUILDING_TYPE_MAX];
    int BuildingSizeY[BUILDING_TYPE_MAX];
    int BuildingLayoutStorgage[BUILDING_TYPE_MAX][BUILDING_MAX_WIDTH][BUILDING_MAX_HEIGHT];

    //Entity specifications for initialization
    int EntitynumModifiers[ENTITY_TYPE_MAX];
    int modifierType[ENTITY_TYPE_MAX][ENT_MAX_MODIF];
    int modifierValue[ENTITY_TYPE_MAX][ENT_MAX_MODIF];
    int AnimNumberOfSteps[ENTITY_TYPE_MAX];
    int AnimDuration[ENTITY_TYPE_MAX];
    Uint32 lifeTime[ENTITY_TYPE_MAX];
    Uint32 startaway[ENTITY_TYPE_MAX];

    //Identifiing number of the player
    int thisUser;
    unsigned short int lastPackID;

    int shiftX,shiftY;
    int drawAoe;

    //This will be used to check if there is anyone able to cast at the moment the cast is received in the group
    bool checkNewSpell;

    //Entitiy List; realized as a pointer array; will be sorted
    int NumberOfEntities;
    cEntity* EntityList[MAX_ENT];

    cUnit* IDListed[MAX_UNITS];
    int newestID;
public:
    bool InitControl();

    void MapInit();
    void InitUnits(int User,int InitSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX]);
    void LoadStandardOpponent();

    bool DoControl(Uint32 t,int scrX,int scrY,bool* TermGame,bool* SelectTile, bool* SelectMiniPic, int PicNumber,
                   SDL_Rect* SelectRect,bool* selectSpecificUnit, SDL_Rect* SpecificUnitRect,int specX,int specY,bool* doMove,bool* doAttack,bool* doStop,int moveX,int moveY,
                   bool* getGroup,bool* setGroup,bool* jumpGroup, int GroupNumber, bool* ShiftDown,bool *doCycleSel, int* activateCommand, int *doCast);
    bool CleanUpControl();

    bool checkCastable(cUnit* Caster,int slot);

    cUnit* getUnitByID(unsigned int ID);

    //routines to create Units on the map, dependent on S/C
    void CreateUnit(unsigned int ID,unsigned int type, int xPosi,int yPosi, int owner);

    void CreateBuilding(int type,int xPosi, int yPosi, int owner);

    //removing a Unit from the map
    void DestoryUnit(cUnit* Unit);

    void createEntity(int Enttype,int srx, int sry, cTile* goalTile, cUnit* goalUnit);

    //stores given Commands by the local user to be sent via TCP/IP
    void storeCommand(cUnit* Un,int Commandtype,int goalX, int goalY,int priority);

    //functions that apply the commands or changes stored on the local machine or sent via TCP/IP to the actual game
    void applyChanges();
    void giveCommands();

    void removeEntity(int num);

    //functions to access the replay
    void writeReplay();
    void readReplay();

    //packing the List of Changes or Controls to NetPackages
    void PackChangeList();
    void PackCommandList();
    //unpacking the Netpackages to Changes or Commands on the corresponding List
    void StripReceivedListServer();
    void StripReceivedListClient();

    unsigned short int createNewPackID();

    int checkFreeUnitID();
};

#endif // CCONTROL_H_INCLUDED
