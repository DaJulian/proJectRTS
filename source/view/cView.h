#ifndef CVIEW_H_INCLUDED
#define CVIEW_H_INCLUDED

#include "cOutput.h"
#include "../model/cWorldMap.h"

// cView: The 2d class displaying everything
class cView
{
protected:
    cInput InputObject;
    cOutput OutputObject;

public:
    cView();
    bool InitView(cGameModel* pMod, sBattleGameModel* pBaMo, int thisUser);
    void DoFrameView(Uint32 ticks);

    void setActiveScreen(int scr);

    void setSelection(cSelection* selectionListH, int selectedNum, int selectedBui, int typeNum, int priorizedNum);
    void setDrawAoe(int aoe);

    sBattleGameInputs* getBattleGameInputs();
    sScrollInputs* getScrollInputs();
    sUnitSelectionInputs* getUnitSelectionInputs();
    sMousePosition* getMousePosition();

    bool* checkTerminateInput();

private:
};

#endif // CVIEW_H_INCLUDED
