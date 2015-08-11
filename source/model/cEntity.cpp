#include "cEntity.h"

cEntity::cEntity(int posX,int posY, cTile* goaTile,cUnit* goaUnit,Uint32 life,int typ,int numMod, int modTyp[ENT_MAX_MODIF]
            ,int modVal[ENT_MAX_MODIF],int AnimNumb,int AnimDura)
{
    positionX = posX;
    postitionY = posY;
    type = typ;
    goalTile = goaTile;
    goalUnit = goaUnit;
    lifeTime = life;

    numModifiers = numMod;
    for(int i=0;i<ENT_MAX_MODIF;i++)
    {
        modifierType[i] = modTyp[i];
        modifierValue[i]= modVal[i];
    }
    offsetX = 0;
    offsetY = 0;

    direction = 1;
    AnimNumberOfSteps = AnimNumb;
    AnimDuration = AnimDura;
    AnimTimeLeft = AnimDuration;
    AnimStepNow = 1;

    isActive = true;
    awaytime = 0;
    startaway = 0;
    distanceX = 0;
    distanceY = 0;

    World = NULL;
}
cEntity::cEntity()
{
    positionX = 0;
    postitionY = 0;
    type = 0;
    goalTile = NULL;
    goalUnit = NULL;
    lifeTime = 0;

    numModifiers = 1;
    modifierType[0] = 1;
    modifierValue[0] = 0;

    offsetX = 0;
    offsetY = 0;

    direction = 1;
    AnimNumberOfSteps = 1;
    AnimDuration = 1;
    AnimTimeLeft = 0;
    AnimStepNow = 1;

    isActive = true;
    awaytime = 0;
    startaway = 0;
    distanceX = 0;
    distanceY = 0;

    World = NULL;
}

void cEntity::Init(cWorldMap* pWorldMap,Uint32 away,int distX,int distY,int dira)
{
    World = pWorldMap;
    awaytime = away;
    startaway = away;
    distanceX = distX;
    distanceY = distY;
    if(goalUnit!= NULL)
    {
        distanceX += goalUnit->getOffsetX();
        distanceY += goalUnit->getOffsetY();
    }
    offsetX = distX;
    offsetY = distY;
    direction = dira;
    for(int i=0;i<numModifiers;i++)
    {
        if(modifierType[i] > ENT_PROP_SELF) modifierType[i] -= ENT_PROP_SELF;
        switch(modifierType[i])
        {
            case ENT_MODIF_BUFF_SPEED:
            {
                //+Error("Speed Buffing");
                goalUnit->setSpeed(goalUnit->getSpeed()+modifierValue[i]);
                break;
            }
        }
    }
}

void cEntity::Animate(Uint32 t)
{
    AnimTimeLeft -= t;
    if(AnimTimeLeft <= 0)
    {
        AnimTimeLeft += AnimDuration;
    }

    AnimStepNow = (AnimDuration-AnimTimeLeft)*AnimNumberOfSteps/AnimDuration;
    //restrict it from having the maximal value
    if(AnimStepNow >= AnimNumberOfSteps) {AnimStepNow = AnimNumberOfSteps-1;}
}
void cEntity::Move(Uint32 t,cChangeList* pChList)
{
    lifeTime-=t;
    awaytime-=t;

    if(goalUnit !=NULL)
    {
        if(goalUnit->getActionType() == ACTION_DYING) isActive = false;
    }


    for(int i=0;i<numModifiers;i++)
    {
        if(lifeTime <= 0)
        {
            switch(modifierType[i])
            {
                case ENT_MODIF_DAMAGE:
                {
                    int damage;
                    damage = modifierValue[i] - goalUnit->getArmor();
                    if(damage < 0) damage = 1;
                    goalUnit->takeDamage(damage,pChList);
                    break;
                }
                case ENT_MODIF_MAGICDAMAGE:
                {
                    int damage;
                    damage = modifierValue[i] - goalUnit->getMagicArmor();
                    if(damage <= 0) damage = 1;
                    goalUnit->takeDamage(damage,pChList);
                    break;
                }
                case ENT_MODIF_HEAL:
                {
                    goalUnit->takeDamage((-1*modifierValue[i]),pChList);
                    break;
                }
                case ENT_MODIF_GROUND_BURST:
                {
                    if(goalTile->getContainUnit() != NULL)
                    {
                        int damage;
                        damage = modifierValue[i] - goalTile->getContainUnit()->getMagicArmor();
                        if(damage <= 0) damage = 1;
                         goalTile->getContainUnit()->takeDamage(damage,pChList);
                    }
                    break;
                }
                case ENT_MODIF_GROUND_BURST_MAGIC:
                {
                    if(goalTile->getContainUnit() != NULL)
                    {
                        int damage;
                        damage = modifierValue[i] - goalTile->getContainUnit()->getMagicArmor();
                        if(damage <= 0) damage = 1;
                         goalTile->getContainUnit()->takeDamage(damage,pChList);
                    }
                    break;
                }
                case ENT_MODIF_BUFF_SPEED:
                {
                    goalUnit->setSpeed(goalUnit->getSpeed()-modifierValue[i]);
                    break;
                }
            }
            isActive = false;
        }
        else
        {
            switch(modifierType[i])
            {
                //case ENT_MODIF_GROUND_BURST_AOE_1:
                //{

                //}
            }
        }
    }
    if(awaytime >= 0)
    {
        offsetX = distanceX  *awaytime/startaway;
        offsetY = distanceY  *awaytime/startaway;
        if(goalUnit != NULL)
        {
            offsetX += goalUnit->getOffsetX()*(1-awaytime/startaway);
            offsetY += goalUnit->getOffsetY()*(1-awaytime/startaway);
        }
    }
    else
    {
        offsetX = 0;
        offsetY = 0;
        if(goalUnit != NULL)
        {
            offsetX += goalUnit->getOffsetX();//*awaytime/startaway;
            offsetY += goalUnit->getOffsetY();//*awaytime/startaway;
        }
    }
}

void cEntity::MoveClient(Uint32 t,cChangeList* pChList)
{
    lifeTime-=t;
    awaytime-=t;
    for(int i=0;i<numModifiers;i++)
    {
        if(lifeTime <= 0)
        {
            isActive = false;
        }
    }
    if(awaytime >= 0)
    {
        offsetX = distanceX  *awaytime/startaway;
        offsetY = distanceY  *awaytime/startaway;
    }
    else
    {
        offsetX = 0;
        offsetY = 0;
    }
}

int cEntity::getType()
{
    return type;
}

int cEntity::getOffsetX()
{
    return offsetX;
}
int cEntity::getOffsetY()
{
    return offsetY;
}

bool cEntity::checkActive()
{
    return isActive;
}

int cEntity::getX()
{
    if(goalUnit != NULL)
    {
        return goalUnit->getPosition()->getX();
    }
    if(goalTile != NULL)
    {
        return goalTile->getX();
    }
    return 0;
}

int cEntity::getY()
{
    if(goalUnit != NULL)
    {
        return goalUnit->getPosition()->getY();
    }
    if(goalTile != NULL)
    {
        return goalTile->getY();
    }
    return 0;
}

int cEntity::getAnimStepNow()
{
    return AnimStepNow;
}
