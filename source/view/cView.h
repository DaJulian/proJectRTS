#ifndef CVIEW_H_INCLUDED
#define CVIEW_H_INCLUDED

#include "cOutput.h"
#include "../model/cWorldMap.h"

// cView: The 2d class displaying everything
class cView : public cOutput
{
protected:

public:
    cView();
    bool InitView(cGameModel* pMod,int thisUser);
    void DoFrameView(Uint32 ticks);

private:
};

#endif // CVIEW_H_INCLUDED
