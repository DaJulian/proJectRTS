#ifndef CINPUT_H_INCLUDED
#define CINPUT_H_INCLUDED

#include "cEventHandler.h"
#include "cSurface.h"
#include "../model/cWorldMap.h"

struct sScrollInputs
{
    int screenX;
    int screenY;

    int screenJumpRelativeX;
    int screenJumpRelativeY;
};

struct sUnitSelectionInputs
{
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

    bool onlyAddSelection;

    int cursorType;
};

struct sBattleGameInputs
{
    //battle game input Variables
    bool clickBattleGameReady;

    int BuyUnits;
    int SellUnits;
    int placeUnit;
    int placeUnitX;
    int placeUnitY;
    int removeUnitX;
    int removeUnitY;
};

struct sMousePosition
{
    int X;
    int Y;
};

class cInput
{
protected:
    //Switch to terminate the game
    bool bTerminateGame;

    sBattleGameInputs battleGameInputs;
    sScrollInputs scrollInputs;
    sUnitSelectionInputs unitSelectionInputs;
    sMousePosition mousePosition;

    //Scroll Speeds
    int SCROLL_SPEED_KEYBOARD;
    int SCROLL_SPEED_MOUSE;
    int DOUBLE_BUTTON_TIME;

    //Time since last scroll
    int ScrollDelayX;
    int ScrollDelayY;
    int doubleButton;

    bool blockLeft;
    bool LastLeftMouseDown;
    int LeftClickPositionX;
    int LeftClickPositionY;

    bool LastRightMouseDown;
    int RightClickPositionX;
    int RightClickPositionY;

    int activeScreen;

    cEventHandler EventHandlerObject;
public:
    cInput();

    bool InitInput();
    bool DoInput(Uint32 ticks);

    void DoInputBattleGame(Uint32 ticks);
    void DoInputScroll(Uint32 ticks);
    void DoInputUnitSelection();
    void DoInputBuildingSelection();
    void DoInputInnScreen();

    bool checkNoAttackOrCast();

    void updateMouseActionsUnitSelection();

    void updateMouseActionsBattleGame();

    void applyScreenJump();
    void tickDownScrollDelay(Uint32 ticks);
    void handleScrolling();
    void handleKeyScrolling();
    void handleMouseScrolling();

    void guaranteeToBeOnMap();

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

    sBattleGameInputs* getBattleGameInputs();
    sScrollInputs* getScrollInputs();
    sUnitSelectionInputs* getUnitSelectionInputs();
    sMousePosition* getMousePosition();

    void setActiveScreen(int scr);

    bool* checkTerminateInput();

    bool CleanUpInput();
private:
};

#endif // CINPUT_H_INCLUDED
