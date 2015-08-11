#ifndef CREPLAY_H_INCLUDED
#define CREPLAY_H_INCLUDED

#include "../netControl/oComdCntrList.h"

class cReplay
{
protected:
    Uint32 nextTime;
    std::fstream fReplay;
public:
    cReplay();

    void InitRead();
    void InitWrite();

    void storeChange(Change* Chang,Uint32 timestamp);
    Change* readChange(Uint32 timestamp);

    void Cleanup();
};

#endif // CREPLAY_H_INCLUDED
