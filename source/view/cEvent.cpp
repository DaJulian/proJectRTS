#include "cEvent.h"

cEvent::cEvent() {
}

cEvent::~cEvent() {
    //Do nothing
}

bool cEvent::UseEvent(SDL_Event* Event)
{
    switch(Event->type)
    {
        case SDL_KEYDOWN:
        {
            InpKeyDown(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_KEYUP:
        {
            InpKeyUp(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_MOUSEMOTION:
        {
            InpMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel
                         ,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0
                         ,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0
                         ,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            InpMouseButtonDown(Event->button.button,Event->button.x,Event->button.y);
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            InpMouseButtonUp(Event->button.button,Event->button.x,Event->button.y);
            break;
        }
        default:
        {
            break;
        }

    }
    return true;
}

void cEvent::InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void cEvent::InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void cEvent::InpMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
}

void cEvent::InpMouseButtonDown(Uint8 button,int mX, int mY)
{
}

void cEvent::InpMouseButtonUp(Uint8 button,int mX, int mY)
{
}
