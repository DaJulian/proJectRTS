#ifndef CANIMATION_H_INCLUDED
#define CANIMATION_H_INCLUDED

#include "cObject.h"

class cAnimation : public cObject
{
protected:
    //Number of Steps per ActionType
    int numberOfSteps[NMBR_ACTIONS];
    int animStepNow;
    //duration of the whole(!) Animation in ms
    int animDuration[NMBR_ACTIONS];
    //remaining Animation TIme
    int animRemainingTime;

    //action that is beeing performed
    int actionType;
    //action of the last frame in order to reset the timers
    int lastAction;
public:
    cAnimation();

    int getActionType();
    void setActionType(int type);

    int getNumberOfSteps(int action);
    void setNumberOfSteps(int steps,int action);

    int getAnimDuration(int action);
    void setAnimDuration(int dura,int action);

    int getAnimStepNow();
    void setAnimStepNow(int step);

    int getAnimRemainingTime();
    void setAnimRemainingTime(int time);

    void Animate(Uint32 t);
};

#endif // CANIMATION_H_INCLUDED
