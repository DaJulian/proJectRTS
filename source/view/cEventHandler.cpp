#include "cEventHandler.h"

cEventHandler::cEventHandler()
{

}

void cEventHandler::InitEventHandler()
{
        //Initialize all keys to be not pressed
    keyA = 0;
    keyM = 0;
    keyDown = 0;
    keyUp = 0;
    keyLeft = 0;
    keyRight = 0;
    keyEscape = 0;
    keyTab = false;
    keyShift = false;

        //Set klickpositions to 0
    LBcX = 0;
    LBcY = 0;
    RBcX = 0;
    RBcY = 0;
    LmXd = 0;
    LmYd = 0;

}

void cEventHandler::PollEvents()
{
    SDL_Event Event;
	while(SDL_PollEvent(&Event))
	{
		UseEvent(&Event);
	}
}

bool cEventHandler::UseEvent(SDL_Event* Event)
{
    cEvent::UseEvent(Event);
	return 1;
}

void cEventHandler::InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	switch(sym)
	{
	    case SDLK_q:
	    {
	        keyAbility[0] = true;
	        break;
	    }
	    case SDLK_w:
	    {
	        keyAbility[1] = true;
	        break;
	    }
	    case SDLK_e:
	    {
	        keyAbility[2] = true;
	        break;
	    }
	    case SDLK_r:
	    {
	        keyAbility[3] = true;
	        break;
	    }
		case SDLK_a:
		{
			keyA = true;
			break;
		}
		case SDLK_m:
		{
			keyA = true;
			break;
		}
		case SDLK_s:
		{
			keyS = true;
			break;
		}
		case SDLK_ESCAPE:
		{
			keyEscape = true;
			break;
		}
		case SDLK_UP:
		{
			keyUp = true;
			break;
		}
        case SDLK_DOWN:
		{
			keyDown = true;
			break;
		}
		case SDLK_LEFT:
		{
			keyLeft = true;
			break;
		}
		case SDLK_RIGHT:
		{
			keyRight = true;
			break;
		}
		case SDLK_LCTRL:
		{
		    keyControl = true;
		    break;
		}
		case SDLK_RCTRL:
		{
		    keyControl = true;
		    break;
		}
		case SDLK_LALT:
		{
		    keyAlt = true;
		    break;
		}
		case SDLK_RALT:
		{
		    keyAlt = true;
		    break;
		}
		case SDLK_RSHIFT:
		{
		    keyShift = true;
		    break;
		}
		case SDLK_LSHIFT:
		{
		    keyShift = true;
		    break;
		}
		case SDLK_TAB:
		{
		    keyTab = true;
		    break;
		}
		case SDLK_0:
		{
		    keyNum[0] = true;
		    break;
		}
		case SDLK_1:
		{
		    keyNum[1] = true;
		    break;
		}
		case SDLK_2:
		{
		    keyNum[2] = true;
		    break;
		}
		case SDLK_3:
		{
		    keyNum[3] = true;
		    break;
		}
		case SDLK_4:
		{
		    keyNum[4] = true;
		    break;
		}
		case SDLK_5:
		{
		    keyNum[5] = true;
		    break;
		}
		case SDLK_6:
		{
		    keyNum[6] = true;
		    break;
		}
		case SDLK_7:
		{
		    keyNum[7] = true;
		    break;
		}
		case SDLK_8:
		{
		    keyNum[8] = true;
		    break;
		}
		case SDLK_9:
		{
		    keyNum[9] = true;
		    break;
		}
		default:
		{
		    break;
		}
	}
}

void cEventHandler::InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	switch(sym)
	{
	    case SDLK_q:
	    {
	        keyAbility[0] = false;
	        break;
	    }
	    case SDLK_w:
	    {
	        keyAbility[1] = false;
	        break;
	    }
	    case SDLK_e:
	    {
	        keyAbility[2] = false;
	        break;
	    }
	    case SDLK_r:
	    {
	        keyAbility[3] = false;
	        break;
	    }
		case SDLK_a:
		{
			keyA = false;
			break;
		}
		case SDLK_m:
		{
			keyA = false;
			break;
		}
		case SDLK_s:
		{
			keyS = false;
			break;
		}
        case SDLK_ESCAPE:
		{
			keyEscape = false;
			break;
		}
        case SDLK_UP:
		{
			keyUp = false;
			break;
		}
        case SDLK_DOWN:
		{
			keyDown = false;
			break;
		}
		case SDLK_LEFT:
		{
			keyLeft = false;
			break;
		}
		case SDLK_RIGHT:
		{
			keyRight = false;
			break;
		}
		case SDLK_LALT:
		{
		    keyAlt = false;
		    break;
		}
		case SDLK_RALT:
		{
		    keyAlt = false;
		    break;
		}
        case SDLK_LCTRL:
		{
		    keyControl = false;
		    break;
		}
		case SDLK_RCTRL:
		{
		    keyControl = false;
		    break;
		}
		case SDLK_RSHIFT:
		{
		    keyShift = false;
		    break;
		}
		case SDLK_LSHIFT:
		{
		    keyShift = false;
		    break;
		}
		case SDLK_TAB:
		{
		    keyTab = false;
		    break;
		}
        case SDLK_0:
		{
		    keyNum[0] = false;
		    break;
		}
		case SDLK_1:
		{
		    keyNum[1] = false;
		    break;
		}
		case SDLK_2:
		{
		    keyNum[2] = false;
		    break;
		}
		case SDLK_3:
		{
		    keyNum[3] = false;
		    break;
		}
		case SDLK_4:
		{
		    keyNum[4] = false;
		    break;
		}
		case SDLK_5:
		{
		    keyNum[5] = false;
		    break;
		}
		case SDLK_6:
		{
		    keyNum[6] = false;
		    break;
		}
		case SDLK_7:
		{
		    keyNum[7] = false;
		    break;
		}
		case SDLK_8:
		{
		    keyNum[8] = false;
		    break;
		}
		case SDLK_9:
		{
		    keyNum[9] = false;
		    break;
		}
		default:
		{
		    break;
		}

	}
}

void cEventHandler::InpMouseMove(int mX, int mY, int relX, int relY, bool LeftB, bool RightB, bool MiddleB)
{
    MouseX = mX;
    MouseY = mY;
    if(LMButton)
    {
        LmXd=mX;
        LmYd=mY;

        //if(mY>=MINIMAP_Y && doDrawRect == true)
        //{
           // bSelect = true;
           // doDrawRect = false;
        //}

    }
}

void cEventHandler::InpMouseButtonDown(Uint8 button, int mX, int mY)
{
    switch(button)
    {
        case SDL_BUTTON_LEFT:
        {
            LMButton = true;
            LBcX = mX;
            LBcY = mY;
            break;
        }
        case SDL_BUTTON_RIGHT:
        {
            RMButton = true;
            RBcX = mX;
            RBcY = mY;
            break;
        }
    }
}

void cEventHandler::InpMouseButtonUp(Uint8 button, int mX, int mY)
{
    switch(button)
    {
        case SDL_BUTTON_LEFT:
        {
            LMButton = false;
            break;
        }
        case SDL_BUTTON_RIGHT:
        {
            RMButton = false;
            break;
        }
    }
}

bool cEventHandler::checkKeyA()

{
    return keyA;
}

void cEventHandler::setKeyA(bool key)
{
    keyA = key;
}

bool cEventHandler::checkKeyM()

{
    return keyM;
}

void cEventHandler::setKeyM(bool key)
{
    keyM = key;
}

bool cEventHandler::checkKeyS()

{
    return keyS;
}

void cEventHandler::setKeyS(bool key)
{
    keyS = key;
}

bool cEventHandler::checkKeyEscape()

{
    return keyEscape;
}

void cEventHandler::setKeyEscape(bool key)
{
    keyEscape = key;
}

bool cEventHandler::checkKeyUp()

{
    return keyUp;
}

void cEventHandler::setKeyUp(bool key)
{
    keyUp = key;
}

bool cEventHandler::checkKeyDown()
{
    return keyDown;
}

void cEventHandler::setKeyDown(bool key)
{
    keyDown = key;
}

bool cEventHandler::checkKeyLeft()
{
    return keyLeft;
}

void cEventHandler::setKeyLeft(bool key)
{
    keyLeft = key;
}

bool cEventHandler::checkKeyRight()
{
    return keyRight;
}

void cEventHandler::setKeyRight(bool key)
{
    keyRight = key;
}

bool cEventHandler::checkKeyControl()
{
    return keyControl;
}

void cEventHandler::setKeyControl(bool key)
{
    keyControl = key;
}

bool cEventHandler::checkKeyTab()
{
    return keyTab;
}

void cEventHandler::setKeyTab(bool key)
{
    keyTab = key;
}

bool cEventHandler::checkKeyNum(int num)

{
    return keyNum[num];
}

void cEventHandler::setKeyNum(int num, bool key)
{
    keyNum[num] = key;
}

bool cEventHandler::checkKeyAlt()

{
    return keyAlt;
}

void cEventHandler::setKeyAlt(bool key)
{
    keyAlt = key;
}

bool cEventHandler::checkKeyAbility(int num)
{
    return keyAbility[num];
}

void cEventHandler::setKeyAbility(int num, bool key)
{
    keyAbility[num] = key;
}

bool cEventHandler::checkKeyShift()
{
    return keyShift;
}

void cEventHandler::setKeyShift(bool key)
{
    keyShift = key;
}

bool cEventHandler::checkLeftMouseButton()
{
    return LMButton;
}

void cEventHandler::setLeftMouseButton(bool set)
{
    LMButton = set;
}

bool cEventHandler::checkRightMouseButton()
{
    return RMButton;
}

void cEventHandler::setRightMouseButton(bool set)
{
    RMButton = set;
}

int cEventHandler::getMouseX()
{
    return MouseX;
}

int cEventHandler::getMouseY()
{
    return MouseY;
}

int cEventHandler::getLeftMouseDownX()
{
    return LBcX;
}

int cEventHandler::getLeftMouseDownY()
{
    return LBcY;
}

void cEventHandler::setLeftMouseDown(int x, int y)
{
    LBcX = x;
    LBcY = y;
}

int cEventHandler::getRightMouseDownX()
{
    return RBcX;
}

int cEventHandler::getRightMouseDownY()
{
    return RBcY;
}

void cEventHandler::setRightMouseDown(int x, int y)
{
    RBcX = x;
    RBcY = y;
}
