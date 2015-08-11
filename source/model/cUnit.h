#ifndef CUNIT_H_INCLUDED
#define CUNIT_H_INCLUDED

#include "cAnimation.h"
#include "../control/cPathFind.h"
#include "../netControl/oComdCntrList.h"

class cUnit : public cAnimation
{
protected:
    //owning player
    int owner;
    //facing direction
    int direction;
    //sight range
    int sightRange;
    //priority in a mixed group
    int sortPriority;

    int NumberAbilities;
    int activeAbility;
    int castOnce;
    //attack Range; 0 if a melee fighter;
    int attackRange[ABILITY_MAX];
    //if it is a ranged fighter these will determine its attack
    //NOte that for casters the same thing will be used; but we will introduce an array
    bool doEntity[ABILITY_MAX];
    int Entitytype[ABILITY_MAX];
    int attackSpeed[ABILITY_MAX];
    int manaCost[ABILITY_MAX];

    //UnitID
    unsigned int ID;

    int actionQueue[QUEUE_ACTION_LENGTH];
    cTile* queueGoals[QUEUE_ACTION_LENGTH];
    cUnit* queueGoalUnits[QUEUE_ACTION_LENGTH];
    int queueTypeModifs[QUEUE_ACTION_LENGTH];

    //Where is it moving
    cTile* goal;
    cUnit* goalUnit;
    cPathKnot* pPathListHead;
    cWorldMap* pWorldMap;
    //LastTile
    int lastX,lastY;

    bool alive;
    bool selected;

    //Speed in time/perTile
    int movementSpeed;
    //remainingTime to reach goal
    int remainingTime;

    int regenRemainTime;

    //Ticks the Unit will wait before resting completely
    int waitingTicks;

    //Offset to blot to right place
    int offsetX,offsetY;

    int health;
    int maxHealth;
    int attackDamage;
    int HealthReg;

    int mana;
    int maxMana;
    int manaReg;

    int Armor;
    int MagicArmor;

    int tauntRange;

    bool lockAction;
    bool lockTarget;
public:
    cUnit();
    cUnit(unsigned int ident,int typ,int own,cTile* posi,int Steps[NMBR_ACTIONS],int Duration[NMBR_ACTIONS],
             int Speed,int AattackSpeed[ABILITY_MAX],int Ahealth,int Adamage,int AsightRange,int AtauntRange,
             int atkrng[ABILITY_MAX],int Entity[ABILITY_MAX],int manCost[ABILITY_MAX],int abtlyNum,int man,
             int manreg,int healreg, int arm, int magarm,int sorprio,cWorldMap* map);

    int getOwner();
    void setOwner(int own);

    unsigned int getID();

    bool checkSelected();
    void setSelected(bool sel);

    int getSortPriority();

    int getDirection();
    void setDirection(int dir);

    int getSightRange();
    void setSightRange(int range);

    void MovementCommand(cTile* goal,int priority);
    void AttackCommand(cTile* goal,int priority);
    void StopCommand();
    void HoldPositionCommand(int prio);
    void ChangeAbilityTypeCommand(int type,bool Ground,bool Self,cChangeList* pChList);
    void CastCommand(int type, cTile* castGoal,bool Ground,bool Self,int priority);

    void Move(Uint32 t,cPathFind* pathFindObj,cChangeList* pChList);
    void MoveClient(Uint32 t);
    bool doTauntCheck(float rang,cChangeList* pChList);

    void changeActiveAbility(int abi,cChangeList* pChList);

    void updateActionQueue(cChangeList* pChList);
    void AddCommandToQueue(int type,cTile* goal,cUnit* goalUnit,int modifType,int priority);


    void deletePath();
    void calculateOffsets();
    void calculateDirection();

    int getOffsetX();
    int getOffsetY();

    int getHealth();
    void setHealth(int heal);
    void takeDamage(int dmg,cChangeList* pChList);

    int getMana();
    int getMaxMana();
    void setMana(int man);
    void useMana(int man,cChangeList* pChList);
    void changeMana(int man,cChangeList* pChList);

    void setSpeed(int sped);
    int getSpeed();

    int getMaxHealth();
    void setMaxHealth(int heal);

    int getArmor();
    int getMagicArmor();

    int getAttackDamage();
    void setAttackDamage(int dmg);

    int getAttackRange(int slot);
    void setAttackRange(int ran,int slot);

    int getTauntRange();
    void setTauntRange(int ran);

    void setGoal(cTile* goa);
    void setUnitGoal(cUnit* uni);

    int getCastOnce();
    void setCastOnce(int cOne);

    cTile* getGoal();
    cUnit* getUnitGoal();

    bool checkAlive();

    int checkEntity(int slot);
    int getEnttype(int slot);
    bool checkAbility(int slot);

    int getNumberOfAblilities();
    int getActiveAbility();
    void setActiveAbility(int abi);

    void createEntity(int slot);

    //these functions have to be called in order to change any state that is relevant for network communication
    void removeUnit(cChangeList* pChList);
    void changePosition(cTile* pos, cChangeList* pChList);
    void changeGoal(cTile* goa,cChangeList* pChList);
    void changeUnitGoal(cUnit* goaUn,cChangeList* pChList);
    void changeMoveType(int type,cChangeList* pChList);
    void changeHealth(int health,cChangeList* pChList);
    void changeCreateEntity(int Enttype,cChangeList* pChList);
    void changeCastOnce(int cOne,cChangeList* pChList);

    void applyMovementChange(int type);
    void applyPositionChange(cTile* pos,cChangeList* pChList);


    ~cUnit();

};

/*class cUnitList
{
protected:
    cUnit* pContent;
    cUnitList* pNext;
    cUnitList* pLast;
public:
    cUnitList();
    cUnitList(cUnit* cont,cUnitList* suc,cUnitList* pre);

    cUnitList* getPredecessor();
    void setPredecessor(cUnitList* pre);

    cUnitList* getSuccessor();
    void setSucessor(cUnitList* suc);

    cTile* getContent();
    void setContent(cTile* un);
};*/

#endif // CUNIT_H_INCLUDED
