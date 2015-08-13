#include "cView.h"

cView::cView()
{

}

bool cView::InitView(cGameModel* pMod, int thisUser)
{
    InitVideo(pMod, thisUser);
    InitInput();
    return true;
}

void cView::DoFrameView(Uint32 ticks)
{
    drawFrame(ticks);
    DoInput(ticks);
}
