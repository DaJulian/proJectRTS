#ifndef CEVENT_H_INCLUDED
#define CEVENT_H_INCLUDED

#include <SDL/SDL.h>
#include "../global.h"

class cEvent
{
    private:
    protected:
    public:

        cEvent();

        virtual ~cEvent();

        //genereal event-Managing function, calls the corresponding function, depending on the event
        virtual bool UseEvent(SDL_Event* Event);

        //handling Key events
        virtual void InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        virtual void InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

        //hanlding Mosue movement
        virtual void InpMouseMove(int mX, int mY, int relX, int relY, bool LeftB, bool RightB, bool MiddleB);

        //handling mouse button inputs
        virtual void InpMouseButtonDown(Uint8 button,int mX, int mY);
        virtual void InpMouseButtonUp(Uint8 button,int mX, int mY);
};

#endif // CEVENT_H_INCLUDED
