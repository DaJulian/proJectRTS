#include "cView.h"

cView::cView()
{

}

bool cView::InitView(cGameModel* pMod, sBattleGameModel* pBaMo, int thisUser)
{
    InputObject.InitInput();
    OutputObject.InitVideo(pMod, pBaMo, thisUser, InputObject.getMousePosition(),
                           InputObject.getScrollInputs(), InputObject.getBattleGameInputs(),
                           InputObject.getUnitSelectionInputs());
    Error("view initialization done");
    return true;
}

void cView::DoFrameView(Uint32 ticks)
{
    OutputObject.drawFrame(ticks);
    InputObject.DoInput(ticks);
}

void cView::setActiveScreen(int scr)
{
    InputObject.setActiveScreen(scr);
    OutputObject.setActiveScreen(scr);
}

void cView::setSelection(cSelection* selectionListH, int selectedNum, int selectedBui, int typeNum, int priorizedNum)
{
    OutputObject.setSelection(selectionListH,selectedNum,selectedBui,typeNum,priorizedNum);
}

void cView::setDrawAoe(int oea)
{
    OutputObject.setDrawAoe(oea);
}

sBattleGameInputs* cView::getBattleGameInputs()
{
    return InputObject.getBattleGameInputs();
}

sScrollInputs* cView::getScrollInputs()
{
    return InputObject.getScrollInputs();
}

sUnitSelectionInputs* cView::getUnitSelectionInputs()
{
    return InputObject.getUnitSelectionInputs();
}

sMousePosition* cView::getMousePosition()
{
    return InputObject.getMousePosition();
}

bool* cView::checkTerminateInput()
{
    return InputObject.checkTerminateInput();
}
