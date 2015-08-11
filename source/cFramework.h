#ifndef CFRAMEWORK_H_INCLUDED
#define CFRAMEWORK_H_INCLUDED

#include "view/cView.h"
#include "view/cOutput.h"
#include "control/cControl.h"
#include "control/cBattleGame.h"

class cFramework : public cView, public cOutput, public cControl, public cBattleGame
{
protected:
    Uint32 TimeThisFrame;
    Uint32 TimeLastFrame;
    Uint32 frameLength;
public:
    cFramework();

    bool InitFramework();
    bool Run();
    bool DoFrame();
    void DoFrameBattleGame();
    bool CleanUpFramework();
};

#endif // CFRAMEWORK_H_INCLUDED
