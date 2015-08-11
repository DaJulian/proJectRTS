#ifndef CENTITY_H_INCLUDED
#define CENTITY_H_INCLUDED

#include "../global.h"
#include "cUnit.h"

class cTile;
//class cUnit;
class cWorldMap;
class cChangeList;

class cEntity
{
protected:
    int positionX,postitionY;
    int type;
    cTile* goalTile;
    cUnit* goalUnit;
    int lifeTime;

    cWorldMap* World;

    int numModifiers;
    int modifierType[ENT_MAX_MODIF];
    int modifierValue[ENT_MAX_MODIF];

    int offsetX,offsetY;
    int direction;
    int AnimNumberOfSteps;
    int AnimDuration;
    int AnimTimeLeft;
    int AnimStepNow;

    int awaytime;
    int startaway;
    int distanceX;
    int distanceY;

    bool isActive;
public:
    cEntity();
    cEntity(int posX,int posY, cTile* goaTile,cUnit* goaUnit,Uint32 life,int typ,int numMod, int modTyp[ENT_MAX_MODIF]
            ,int modVal[ENT_MAX_MODIF],int AnimNumb,int AnimDura);

    void Init(cWorldMap* pWorldMap,Uint32 away,int distX,int distY,int dira);

    void Animate(Uint32 t);
    void Move(Uint32 t,cChangeList* pChangeList);
    void MoveClient(Uint32 t,cChangeList* pChangeList);

    int getType();

    int getOffsetX();
    int getOffsetY();

    bool checkActive();

    int getX();
    int getY();

    int getAnimStepNow();
};

#endif // CENTITY_H_INCLUDED
