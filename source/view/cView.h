#ifndef CVIEW_H_INCLUDED
#define CVIEW_H_INCLUDED

#include "cEvent.h"
#include "cSurface.h"
#include "../model/cWorldMap.h"

// cView: The 2d class displaying everything
class cView : public cEvent
{
    protected:
        //Switch to terminate the game
        bool bTerminateGame;

    	//Mouse position
    	int MouseX;
    	int MouseY;

    	//Scroll Speeds
    	int SCROLL_SPEED_KEYBOARD;
    	int SCROLL_SPEED_MOUSE;
    	int DOUBLE_BUTTON_TIME;

    	//Time since last scroll
    	int ScrollDelayX;
        int ScrollDelayY;
        int doubleButton;

    	// List of the MouseButtons
    	bool LMButton;
    	bool blockLeft;
        bool RMButton;
        bool MMButton;

        //
        bool doDrawRect;

        //Command bools
        bool doMove;
        bool attackMove;
        bool stopMove;

        int moveCommandX;
        int moveCommandY;

        //set the selection to a certain group
        bool SetGroup;
        int GroupNumber;
        bool GetGroup;
        bool JumpGroup;

        bool selMiniPic;
        int selMiniPicNumber;

        int changeAbility;

        int castMove;

        //Coordinates of Mousedown
        int LBcX;
        int LBcY;
        int LmXd;
        int LmYd;
        int RBcX;
        int RBcY;

        //Draw a rectangle for selection
        SDL_Rect DrawRect;

        //Select Inside this rectangle
        bool bSelect;
        SDL_Rect SelectRect;
        SDL_Rect SelectSpecificRect;
        bool selectOnlySpecificUnit;
        int specificUnitX,specificUnitY;
        bool rightClickGrid;

        bool doSelectionCycle;

        // List of the keys
        bool keyA;
        bool keyM;
        bool keyS;
        bool keyEscape;
        bool keyUp;
        bool keyDown;
        bool keyLeft;
        bool keyRight;
        bool keyControl;
        bool keyTab;
        bool keyNum[10];
        bool keyAlt;
        bool keyAbility[4];
        bool keyShift;

        //Coordinates of Players Screen
        int screenX;
        int screenY;

        //Cursortype
        int cursorType;

        //battle game input Variables
        bool clickBattleGameReady;
    public:
        cView();

        bool InitInput();
        bool DoInput(Uint32 t, int* screenJumpRelativeX, int* screenJumpRelativeY);
        void DoBattleGameInput(Uint32 t,int* BuyUnits,int* SellUnits,int* placeUnit, int* placeUnitX
                               ,int* placeUnitY,int* removeUnitX, int* removeUnitY);

        void applyScreenJump(int* screenJumpRelativeX, int* screenJumpRelativeY);
        void tickDownScrollDelay(Uint32 ticks);
        void handleScrolling();
        void handleKeyScrolling();
        void handleMouseScrolling();

        void handleAttackMove();
        void handleCastCommand();

        void handleLeftClickOnMap();
        void handleLeftClickOnHud();

        void handleLeftClickOnMinimap();
        void handleLeftClickOnRightHud();

        void handleRightClickOnMap();
        void handleRightClickOnHud();

        void handleRightClickOnMinimap();
        void handleRightClickOnRightHud();

        void ClickGridLeft();
        void ClickGridRight();

        bool CleanUpInput();

        //virtuals already definded in cEvent
        //genereal event-Managing function, calls the corresponding function, depending on the event
        bool UseEvent(SDL_Event* Event);

        //handling Key events
        void InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

        //hanlding Mosue movement
        void InpMouseMove(int mX, int mY, int relX, int relY, bool LeftB, bool RightB, bool MiddleB);

        //handling mouse button inputs
        void InpMouseButtonDown(Uint8 button,int mX, int mY);
        void InpMouseButtonUp(Uint8 button,int mX, int mY);
    private:
};

#endif // CVIEW_H_INCLUDED
