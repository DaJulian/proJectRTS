#include "cReplay.h"

cReplay::cReplay()
{

}

void cReplay::InitRead()
{
    Error("Replay init for read");
	fReplay.open(REPLAY_FILE.c_str(), std::ios::in | std::ios::app);
    fReplay >> nextTime;
    int test;
    test = nextTime;
    Error("first next time",test);
}

void cReplay::InitWrite()
{
    Error("Replay init for write");
	fReplay.open(REPLAY_FILE.c_str(), std::ios::out /*| std::ios::app*/ | std::ios::trunc);
}

void cReplay::storeChange(Change* Chang,Uint32 timestamp)
{
    fReplay << timestamp << "\n";
    fReplay << Chang->changeType << "\n";
    fReplay << Chang->posiX << "\n";
    fReplay << Chang->posiY << "\n";
    fReplay << Chang->UnitID << "\n";
    fReplay << Chang->goalX << "\n";
    fReplay << Chang->goalY << "\n";
    fReplay << Chang->goalUnitID << "\n";
    fReplay << Chang->moveType << "\n";
    fReplay << Chang->health << "\n";
}

Change* cReplay::readChange(Uint32 timestamp)
{
    if(timestamp >= nextTime)
    {
        Change* newChange;
        newChange = new Change;
        fReplay >> newChange->changeType;
        fReplay >> newChange->posiX;
        fReplay >> newChange->posiY ;
        fReplay >> newChange->UnitID ;
        fReplay >> newChange->goalX ;
        fReplay >> newChange->goalY ;
        fReplay >> newChange->goalUnitID;
        fReplay >> newChange->moveType ;
        fReplay >> newChange->health;
        fReplay >> nextTime;
        return newChange;
    }
    else
    {
        return NULL;
    }
}

void cReplay::Cleanup()
{
    fReplay.close();
}
