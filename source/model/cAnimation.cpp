#include "cAnimation.h"

cAnimation::cAnimation()
{
    for(int i=0;i<NMBR_ACTIONS;i++)
    {
        numberOfSteps[i] = 0;
        animDuration[i] = 0;
    }

    animStepNow = 0;
    animRemainingTime = 0;
    lastAction = ACTION_REST;
}

int cAnimation::getActionType()
{
    return actionType;
}
void cAnimation::setActionType(int type)
{
    actionType = type;
}

int cAnimation::getNumberOfSteps(int action)
{
    return numberOfSteps[action];
}
void cAnimation::setNumberOfSteps(int steps,int action)
{
    numberOfSteps[action] = steps;
}

int cAnimation::getAnimDuration(int action)
{
    return animDuration[action];
}
void cAnimation::setAnimDuration(int dura,int action)
{
    animDuration[action] = dura;
}

int cAnimation::getAnimStepNow()
{
    return animStepNow;
}
void cAnimation::setAnimStepNow(int step)
{
    animStepNow = step;
}

int cAnimation::getAnimRemainingTime()
{
    return animRemainingTime;
}
void cAnimation::setAnimRemainingTime(int t)
{
    animRemainingTime = t;
}

void cAnimation::Animate(Uint32 t)
{
    //Check if the animation just changed (otherwise it will not be started from the beginning)
    if(lastAction != actionType){animRemainingTime = 0;}
    lastAction = actionType;
    //Change timer
    animRemainingTime -= t;
    if(animRemainingTime <= 0)
    {
        animRemainingTime += animDuration[actionType];
    }

    animStepNow = (animDuration[actionType]-animRemainingTime)*numberOfSteps[actionType]/animDuration[actionType];
    //restrict it from having the maximal value
    if(animStepNow >= numberOfSteps[actionType]) {animStepNow = numberOfSteps[actionType]-1;}
//    Error("AnimStep",animStepNow);
}
