#include "cInput.h"

cInput::cInput()
{
    scrollInputs.screenX = 0;
    scrollInputs.screenY = 0;

    SCROLL_SPEED_KEYBOARD = 100;
    SCROLL_SPEED_MOUSE    = 100;

    bTerminateGame = false;
}

bool cInput::InitInput()
{
    //document
    Error("Initializing Input");

    std::string garbage;
    std::fstream f;
	f.open(INPUT_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);

    //Get Scroll speeds
    f >> garbage >> SCROLL_SPEED_MOUSE
      >> garbage >> SCROLL_SPEED_KEYBOARD
      >> garbage >> DOUBLE_BUTTON_TIME;

    activeScreen = SCREEN_BATTLE_GAME;

    EventHandlerObject.InitEventHandler();

    //ScrollDelays set to 0
    ScrollDelayX = 0;
    ScrollDelayY = 0;

    //Initialize all mouse buttons to be not pressed
    blockLeft = false;
    unitSelectionInputs.cursorType = CURSOR_NORMAL;

    unitSelectionInputs.doDrawRect= false;
    unitSelectionInputs.doSelectionCycle = false;

    unitSelectionInputs.rightClickGrid = false;

    unitSelectionInputs.selMiniPic = false;
    unitSelectionInputs.selMiniPicNumber = 1;

    //Command bools
    unitSelectionInputs.doMove = false;
    unitSelectionInputs.attackMove = false;
    unitSelectionInputs.stopMove = false;

    unitSelectionInputs.moveCommandX = 0;
    unitSelectionInputs.moveCommandY = 0;

    unitSelectionInputs.GetGroup = false;
    unitSelectionInputs.SetGroup = false;
    unitSelectionInputs.GroupNumber = 0;

    unitSelectionInputs.selectOnlySpecificUnit = false;

    //DrawRectangle set to 0
    unitSelectionInputs.DrawRect.x = 0;
    unitSelectionInputs.DrawRect.y = 0;
    unitSelectionInputs.DrawRect.w = 0;
    unitSelectionInputs.DrawRect.h = 0;

    //SelectRectangle set to 0 and no selection to do
    unitSelectionInputs.bSelect = false;
    unitSelectionInputs.SelectRect.x = 0;
    unitSelectionInputs.SelectRect.y = 0;
    unitSelectionInputs.SelectRect.w = 0;
    unitSelectionInputs.SelectRect.h = 0;

    //Tell the game not to terminate
    bTerminateGame = false;

    unitSelectionInputs.changeAbility = -1;
    unitSelectionInputs.castMove = -1;

    //Battle game screen variables
    battleGameInputs.clickBattleGameReady = false;

    return 1;
}

bool cInput::DoInput(Uint32 ticks)
{
	EventHandlerObject.PollEvents();

    if(EventHandlerObject.checkKeyEscape())
    {
        bTerminateGame = true;
        EventHandlerObject.setKeyEscape(false);
        return 1;
    }

	switch(activeScreen)
	{
	    case SCREEN_BATTLE_GAME:
	    {
	        DoInputBattleGame(ticks);
	        break;
	    }
	    case SCREEN_REPLAY:
	    {
	        DoInputScroll(ticks);
	        break;
	    }
	    case SCREEN_MAP_UNIT:
	    {
	        DoInputScroll(ticks);
	        DoInputUnitSelection();
	        break;
	    }
	    case SCREEN_MAP_BUILDING:
	    {
	        DoInputScroll(ticks);
	        DoInputBuildingSelection();
	        break;
	    }
	    case SCREEN_INN:
	    {
	        DoInputInnScreen();
	        break;
	    }
	}

    return 1;

}

void cInput::DoInputScroll(Uint32 ticks)
{
    mousePosition.X = EventHandlerObject.getMouseX();
    mousePosition.Y = EventHandlerObject.getMouseY();
    applyScreenJump();

    tickDownScrollDelay(ticks);
    handleScrolling();

    guaranteeToBeOnMap();
}

void cInput::DoInputUnitSelection()
{
    updateMouseActionsUnitSelection();

    handleAttackMove();
    handleCastCommand();

    //Clicking somewhere without an active attackmove or castmove
    if(LastLeftMouseDown && blockLeft == false && checkNoAttackOrCast())
    {
        handleLeftClickOnMap();
        handleLeftClickOnHud();
        guaranteeToBeOnMap();
    }
    if(!(LastLeftMouseDown))
    {
        unitSelectionInputs.doDrawRect = false;
    }
    if(EventHandlerObject.checkRightMouseButton())
    {
        if(checkNoAttackOrCast())
        {
            handleRightClickOnMap();
            handleRightClickOnHud();
        }
        else
        {
            unitSelectionInputs.attackMove = false;
            unitSelectionInputs.castMove = -1;
        }

        EventHandlerObject.setRightMouseButton(false);
    }
    //switching attack move
    if(EventHandlerObject.checkKeyA())
    {
        if(unitSelectionInputs.attackMove)
        {
            unitSelectionInputs.attackMove = false;
        }
        else
        {
            unitSelectionInputs.attackMove = true;
        }
        EventHandlerObject.setKeyA(false);
    }
    //giving a stop command
    if(EventHandlerObject.checkKeyS())
    {
        unitSelectionInputs.stopMove = true;
        EventHandlerObject.setKeyS(false);
    }
    //Check assigning and accessing groups
    for(int i=0;i < 10; i++)
    {
        if(EventHandlerObject.checkKeyControl())
        {
            if(EventHandlerObject.checkKeyNum(i))
            {
                unitSelectionInputs.SetGroup = true;
                unitSelectionInputs.GroupNumber = i;
                if(i == 0){unitSelectionInputs.GroupNumber = 10;}
                EventHandlerObject.setKeyNum(i, false);
                EventHandlerObject.setKeyControl(false);
            }
        }
        else
        {
            if(EventHandlerObject.checkKeyNum(i))
            {
                unitSelectionInputs.GetGroup = true;
                if(doubleButton > 0 && unitSelectionInputs.GroupNumber == i)
                {
                    unitSelectionInputs.JumpGroup=true;
                }
                unitSelectionInputs.GroupNumber = i;
                if(i == 0){unitSelectionInputs.GroupNumber = 10;}
                EventHandlerObject.setKeyNum(i, false);
                doubleButton = DOUBLE_BUTTON_TIME;
            }
        }
    }
    if(EventHandlerObject.checkKeyControl())
    {
        for(int i=0;i < 4; i++)
        {
            if(EventHandlerObject.checkKeyAbility(i) || unitSelectionInputs.castMove == i)
            {
                Error("change Ability ",unitSelectionInputs.changeAbility);
                unitSelectionInputs.changeAbility = i;
                EventHandlerObject.setKeyAbility(i, false);
                if(unitSelectionInputs.rightClickGrid ==true)
                {
                    EventHandlerObject.setKeyControl(false);
                    unitSelectionInputs.rightClickGrid=false;
                }
                if(unitSelectionInputs.castMove == i)
                {
                    unitSelectionInputs.castMove=-1;
                }
            }
        }
    }

    //Check if cast moves will be applied
    for(int i=0;i < 4; i++)
    {
        if(EventHandlerObject.checkKeyAbility(i))
        {
            if(unitSelectionInputs.castMove == -1)
            {
                unitSelectionInputs.castMove = i;
                EventHandlerObject.setKeyAbility(i, false);
            }
            else
            {
                unitSelectionInputs.castMove = -1;
                EventHandlerObject.setKeyAbility(i, false);
            }
        }
    }

    if(EventHandlerObject.checkKeyTab())
    {
        unitSelectionInputs.doSelectionCycle = true;
        EventHandlerObject.setKeyTab(false);
    }
    //Set Cursortype
    if(unitSelectionInputs.attackMove == true)
    {
        unitSelectionInputs.cursorType = CURSOR_ATTACK;
    }
    else
    {
        if(unitSelectionInputs.castMove != -1)
        {
            unitSelectionInputs.cursorType = CURSOR_CAST;
        }
        else
        {
            unitSelectionInputs.cursorType = CURSOR_NORMAL;
        }
    }
    if(EventHandlerObject.checkKeyShift())
    {
        unitSelectionInputs.onlyAddSelection = true;
    }
    else
    {
        unitSelectionInputs.onlyAddSelection = false;
    }
}

bool cInput::checkNoAttackOrCast()
{
    return ((!(unitSelectionInputs.attackMove)) && unitSelectionInputs.castMove == -1);
}

void cInput::updateMouseActionsUnitSelection()
{
    if(LastLeftMouseDown)
    {
        if(EventHandlerObject.checkLeftMouseButton())
        {
            //Set Rectangle for Drawing
            if(mousePosition.X >= LeftClickPositionX)
            {
                unitSelectionInputs.DrawRect.x = LeftClickPositionX;
                unitSelectionInputs.DrawRect.w = mousePosition.X - LeftClickPositionX;
            }
            else
            {
                unitSelectionInputs.DrawRect.x = mousePosition.X;
                unitSelectionInputs.DrawRect.w = LeftClickPositionX - mousePosition.X;
            }
            if(mousePosition.Y >= LeftClickPositionY)
            {
                unitSelectionInputs.DrawRect.y = LeftClickPositionY;
                unitSelectionInputs.DrawRect.h = mousePosition.Y - LeftClickPositionY;
            }
            else
            {
                unitSelectionInputs.DrawRect.y = mousePosition.Y;
                unitSelectionInputs.DrawRect.h = LeftClickPositionY - mousePosition.Y;
            }
            if(mousePosition.X <= SCR_TILE_WIDTH*TILE_WIDTH && mousePosition.Y <= SCR_TILE_HEIGHT*TILE_HEIGHT && checkNoAttackOrCast() && !blockLeft)
            {
                unitSelectionInputs.doDrawRect = true;
            }
        }
        else
        {
            if(mousePosition.X <= SCR_TILE_WIDTH*TILE_WIDTH && mousePosition.Y <= SCR_TILE_HEIGHT*TILE_HEIGHT && checkNoAttackOrCast()&& !blockLeft)
            {
                unitSelectionInputs.bSelect = true;
            }
            //SetRectangleForSelection
            if(mousePosition.X >= LeftClickPositionX)
            {
                unitSelectionInputs.SelectRect.x = LeftClickPositionX;
                unitSelectionInputs.SelectRect.w = mousePosition.X - LeftClickPositionX;
            }
            else
            {
                unitSelectionInputs.SelectRect.x = mousePosition.X;
                unitSelectionInputs.SelectRect.w = LeftClickPositionX - mousePosition.X;
            }
            if(mousePosition.Y >= LeftClickPositionY)
            {
                unitSelectionInputs.SelectRect.y = LeftClickPositionY;
                unitSelectionInputs.SelectRect.h = mousePosition.Y - LeftClickPositionY;
            }
            else
            {
                unitSelectionInputs.SelectRect.y = mousePosition.Y;
                unitSelectionInputs.SelectRect.h = LeftClickPositionY - mousePosition.Y;
            }
            //unitSelectionInputs.bSelect = true;
            LastLeftMouseDown = false;
        }
    }
    else
    {
        if(EventHandlerObject.checkLeftMouseButton())
        {
            if(blockLeft)
            {
                blockLeft = false;
                //EventHandlerObject.setLeftMouseButton(false);
                /*LastLeftMouseDown = true;
                LeftClickPositionX = EventHandlerObject.getLeftMouseDownX();
                LeftClickPositionY = EventHandlerObject.getLeftMouseDownY();*/
            }
            else
            {
                LeftClickPositionX = EventHandlerObject.getLeftMouseDownX();
                LeftClickPositionY = EventHandlerObject.getLeftMouseDownY();
                LastLeftMouseDown = true;
            }
        }
        else
        {
            LastLeftMouseDown = false;
        }
    }

    if(LastRightMouseDown)
    {
        if(EventHandlerObject.checkRightMouseButton())
        {

        }
        else
        {
            LastRightMouseDown = false;
        }
    }
    else
    {
        if(EventHandlerObject.checkRightMouseButton())
        {
            LastRightMouseDown = true;
            RightClickPositionX = EventHandlerObject.getRightMouseDownX();
            RightClickPositionY = EventHandlerObject.getRightMouseDownY();
        }
        else
        {
            LastRightMouseDown = false;
        }
    }
}

void cInput::DoInputBuildingSelection()
{

}

void cInput::DoInputInnScreen()
{

}

void cInput::applyScreenJump()
{
    if(scrollInputs.screenJumpRelativeX != 0)
    {
        scrollInputs.screenX = scrollInputs.screenX + scrollInputs.screenJumpRelativeX;
        //LmXd = 0;
        LeftClickPositionX = 0;
        LastLeftMouseDown = false;
        scrollInputs.screenJumpRelativeX = 0;
    }
    if(scrollInputs.screenJumpRelativeY != 0)
    {
        scrollInputs.screenY = scrollInputs.screenY + scrollInputs.screenJumpRelativeY;
        //LmYd = 0;
        LeftClickPositionY = 0;
        LastLeftMouseDown = false;
        scrollInputs.screenJumpRelativeY = 0;
    }
}

void  cInput::tickDownScrollDelay(Uint32 ticks)
{
    ScrollDelayY -= ticks;
    if(ScrollDelayY<0) ScrollDelayY = 0;
    ScrollDelayX -= ticks;
    if(ScrollDelayX<0) ScrollDelayX = 0;
    doubleButton -= ticks;
    if(doubleButton<0) doubleButton = 0;
}

void cInput::handleScrolling()
{
    handleKeyScrolling();
    handleMouseScrolling();
}

void cInput::handleKeyScrolling()
{
    if(EventHandlerObject.checkKeyUp())
    {
        //Scroll up if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            scrollInputs.screenY--;
            ScrollDelayY = SCROLL_SPEED_KEYBOARD;
            if(scrollInputs.screenY<0) scrollInputs.screenY =0;
        }
    }
    if(EventHandlerObject.checkKeyLeft())
    {
        //Scroll left if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            scrollInputs.screenX--;
            ScrollDelayX = SCROLL_SPEED_KEYBOARD;
            if(scrollInputs.screenX<0) scrollInputs.screenX =0;
        }
    }
    if(EventHandlerObject.checkKeyDown())
    {
        //Scroll down if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            scrollInputs.screenY++;
            ScrollDelayY = SCROLL_SPEED_KEYBOARD;
            if(scrollInputs.screenY>WORLD_HEIGHT -SCR_TILE_HEIGHT) scrollInputs.screenY = WORLD_HEIGHT -SCR_TILE_HEIGHT;
        }
    }
    if(EventHandlerObject.checkKeyRight())
    {
        //Scroll right if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            scrollInputs.screenX++;
            ScrollDelayX = SCROLL_SPEED_KEYBOARD;
            if(scrollInputs.screenX>WORLD_WIDTH -SCR_TILE_WIDTH) scrollInputs.screenX = WORLD_WIDTH -SCR_TILE_WIDTH;
        }
    }
}

void cInput::handleMouseScrolling()
{
    if(EventHandlerObject.getMouseY() < SCROLL_MARGIN)
    {
        //Scroll up if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            scrollInputs.screenY--;
            ScrollDelayY = SCROLL_SPEED_MOUSE;
            if(scrollInputs.screenY<0) scrollInputs.screenY =0;
        }
    }
    if(EventHandlerObject.getMouseX() < SCROLL_MARGIN)
    {
        //Scroll left if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            scrollInputs.screenX--;
            ScrollDelayX = SCROLL_SPEED_MOUSE;
            if(scrollInputs.screenX<0) scrollInputs.screenX =0;
        }
    }
    if(EventHandlerObject.getMouseY() > SCREEN_HEIGHT-SCROLL_MARGIN)
    {
        //Scroll down if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            scrollInputs.screenY++;
            ScrollDelayY = SCROLL_SPEED_MOUSE;
            if(scrollInputs.screenY>WORLD_HEIGHT -SCR_TILE_HEIGHT) scrollInputs.screenY = WORLD_HEIGHT -SCR_TILE_HEIGHT;
        }
    }
    if(EventHandlerObject.getMouseX() > SCREEN_WIDTH-SCROLL_MARGIN)
    {
        //Scroll right if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            scrollInputs.screenX++;
            ScrollDelayX = SCROLL_SPEED_MOUSE;
            if(scrollInputs.screenX>WORLD_WIDTH -SCR_TILE_WIDTH) scrollInputs.screenX = WORLD_WIDTH -SCR_TILE_WIDTH;
        }
    }
}

void cInput::guaranteeToBeOnMap()
{
    if(scrollInputs.screenX > WORLD_WIDTH-SCR_TILE_WIDTH){scrollInputs.screenX = WORLD_WIDTH-SCR_TILE_WIDTH;}
    if(scrollInputs.screenY > WORLD_HEIGHT-SCR_TILE_HEIGHT){scrollInputs.screenY = WORLD_HEIGHT-SCR_TILE_HEIGHT;}
    if(scrollInputs.screenX < 0){scrollInputs.screenX = 0;}
    if(scrollInputs.screenY < 0){scrollInputs.screenY = 0;}
}

void cInput::handleAttackMove()
{
    if(LastLeftMouseDown && blockLeft == false && unitSelectionInputs.attackMove)
    {
        //attackmove on the screen
        if(LeftClickPositionX <= SCR_TILE_WIDTH*TILE_WIDTH && LeftClickPositionY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
        {
            unitSelectionInputs.moveCommandX = scrollInputs.screenX + (LeftClickPositionX / TILE_WIDTH);
            unitSelectionInputs.moveCommandY = scrollInputs.screenY + (LeftClickPositionY / TILE_HEIGHT);
            unitSelectionInputs.doMove = true;
        }
        //attackmove on the minimap
        if(LeftClickPositionX >= MINIMAP_X && LeftClickPositionX <= MINIMAP_X+MINIMAP_WIDTH
           && LeftClickPositionY >= MINIMAP_Y && LeftClickPositionY <= MINIMAP_Y+MINIMAP_HEIGHT)
        {
            unitSelectionInputs.moveCommandX = (LeftClickPositionX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
            unitSelectionInputs.moveCommandY = (LeftClickPositionY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
            unitSelectionInputs.doMove = true;
        }
        blockLeft = true;
    }
}

void cInput::handleCastCommand()
{
    if(LastLeftMouseDown && blockLeft == false && unitSelectionInputs.castMove != -1)
    {
        //Cast COmmand On the screen
        if(LeftClickPositionX <= SCR_TILE_WIDTH*TILE_WIDTH && LeftClickPositionY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
        {
            unitSelectionInputs.moveCommandX = scrollInputs.screenX + (LeftClickPositionX / TILE_WIDTH);
            unitSelectionInputs.moveCommandY = scrollInputs.screenY + (LeftClickPositionY / TILE_HEIGHT);
            unitSelectionInputs.doMove = true;
        }
        //Cast Command On the minimap
        if(LeftClickPositionX >= MINIMAP_X && LeftClickPositionX <= MINIMAP_X+MINIMAP_WIDTH
           && LeftClickPositionY >= MINIMAP_Y
           && LeftClickPositionY <= MINIMAP_Y+MINIMAP_HEIGHT)
        {
            unitSelectionInputs.moveCommandX = (LeftClickPositionX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
            unitSelectionInputs.moveCommandY = (LeftClickPositionY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
            unitSelectionInputs.doMove = true;
        }
        blockLeft = true;
    }
}

void cInput::handleLeftClickOnMap()
{
    //draw a rectangle on the screen
    if(LeftClickPositionX <= SCR_TILE_WIDTH*TILE_WIDTH && LeftClickPositionY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
    {
        if(EventHandlerObject.checkKeyControl())
        {
            unitSelectionInputs.selectOnlySpecificUnit = true;
            unitSelectionInputs.SelectSpecificRect.w = SCR_TILE_WIDTH*TILE_WIDTH;
            unitSelectionInputs.SelectSpecificRect.h = SCR_TILE_HEIGHT*TILE_HEIGHT;
            unitSelectionInputs.SelectSpecificRect.x = 0;
            unitSelectionInputs.SelectSpecificRect.y = 0;
            unitSelectionInputs.specificUnitX = LeftClickPositionX/TILE_WIDTH + scrollInputs.screenX;
            unitSelectionInputs.specificUnitY = LeftClickPositionY/TILE_HEIGHT + scrollInputs.screenY;
            if(unitSelectionInputs.specificUnitX < 0){unitSelectionInputs.specificUnitX = 0;}
            if(unitSelectionInputs.specificUnitX >= WORLD_WIDTH){unitSelectionInputs.specificUnitX = WORLD_WIDTH-1;}
            if(unitSelectionInputs.specificUnitY < 0){unitSelectionInputs.specificUnitY = 0;}
            if(unitSelectionInputs.specificUnitY >= WORLD_HEIGHT){unitSelectionInputs.specificUnitY = WORLD_HEIGHT-1;}
            EventHandlerObject.setLeftMouseButton(false);
            blockLeft = true;
        }
        else
        {
            unitSelectionInputs.doDrawRect = true;
        }
    }

}

void cInput::handleLeftClickOnHud()
{
    handleLeftClickOnMinimap();
    handleLeftClickOnRightHud();
}

void cInput::handleLeftClickOnMinimap()
{
    if(mousePosition.X >= MINIMAP_X && mousePosition.X <= MINIMAP_X+MINIMAP_WIDTH && mousePosition.Y >= MINIMAP_Y && mousePosition.Y <= MINIMAP_Y+MINIMAP_HEIGHT)
    {
        scrollInputs.screenX = (mousePosition.X - MINIMAP_X)/MINIMAP_TILE_WIDTH - SCR_TILE_WIDTH/2;
        scrollInputs.screenY = (mousePosition.Y - MINIMAP_Y)/MINIMAP_TILE_HEIGHT - SCR_TILE_HEIGHT/2;
        Error("jump to X: ",scrollInputs.screenX);
        Error("jump to Y: ",scrollInputs.screenY);
    }
}

void cInput::handleLeftClickOnRightHud()
{
    //the minipic range is clicked; select the clicked unit
    if(LeftClickPositionX >= MINIPIC_X && LeftClickPositionY >= MINIPIC_Y && LeftClickPositionX <= (MINIPIC_X + MINIPIC_COLUMNS*MINIPIC_WIDTH) && LeftClickPositionY <= (MINIPIC_Y + MINIPIC_ROWS*MINIPIC_HEIGHT))
    {
        unitSelectionInputs.selMiniPicNumber = 10;
        int selX,selY;
        selX = ((LeftClickPositionX - MINIPIC_X))/MINIPIC_WIDTH + 1;
        selY = ((LeftClickPositionY - MINIPIC_Y))/MINIPIC_HEIGHT;
        unitSelectionInputs.selMiniPicNumber = selX + selY*MINIPIC_COLUMNS;
        ///DEBUG
        //selMiniPicNumber = 2;
        unitSelectionInputs.selMiniPic = true;
        EventHandlerObject.setLeftMouseButton(false);
        LastLeftMouseDown = false;
    }
    if(LeftClickPositionX >= GRID_X && LeftClickPositionY >= GRID_Y)
    {
        ClickGridLeft();
    }
}

void cInput::handleRightClickOnMap()
{
    if(RightClickPositionX <= SCR_TILE_WIDTH*TILE_WIDTH && RightClickPositionY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
    {
        unitSelectionInputs.moveCommandX = scrollInputs.screenX + (RightClickPositionX / TILE_WIDTH);
        unitSelectionInputs.moveCommandY = scrollInputs.screenY + (RightClickPositionY / TILE_HEIGHT);
        unitSelectionInputs.doMove = true;
    }
}

void cInput::handleRightClickOnHud()
{
    handleRightClickOnMinimap();
    handleRightClickOnRightHud();
}

void cInput::handleRightClickOnMinimap()
{
    if(RightClickPositionX >= MINIMAP_X && RightClickPositionX <= MINIMAP_X+MINIMAP_WIDTH
       && RightClickPositionY >= MINIMAP_Y && RightClickPositionY <= MINIMAP_Y+MINIMAP_HEIGHT)
    {
        unitSelectionInputs.moveCommandX = (RightClickPositionX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
        unitSelectionInputs.moveCommandY = (RightClickPositionY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
        unitSelectionInputs.doMove = true;
    }
}

void cInput::handleRightClickOnRightHud()
{
    if(RightClickPositionX >= GRID_X && RightClickPositionY >= GRID_Y)
    {
        ClickGridRight();
    }
}

void cInput::DoInputBattleGame(Uint32 t)
{
    updateMouseActionsBattleGame();
    if(EventHandlerObject.checkLeftMouseButton())
    {
        if(LeftClickPositionX >= BATTLE_GAME_GOBUTTON_X && LeftClickPositionX <= BATTLE_GAME_GOBUTTON_X+BATTLE_GAME_GOBUTTON_WIDTH
           && LeftClickPositionY >= BATTLE_GAME_GOBUTTON_Y && LeftClickPositionY <= BATTLE_GAME_GOBUTTON_Y+BATTLE_GAME_GOBUTTON_HEIGHT)
        {
            battleGameInputs.clickBattleGameReady = true;
        }
        //if(LeftClickPositionX >= BATTLE_GAME_AVAILABLE_X && )
        if(LeftClickPositionX >= BATTLE_GAME_AVAILABLE_X && LeftClickPositionX <= BATTLE_GAME_AVAILABLE_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && LeftClickPositionY >= BATTLE_GAME_AVAILABLE_Y && LeftClickPositionY <= BATTLE_GAME_AVAILABLE_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            int useX;
            useX = (LeftClickPositionX - BATTLE_GAME_AVAILABLE_X)/BATTLE_GAME_PIC_WIDTH;
            battleGameInputs.BuyUnits = useX;
        }
        if(LeftClickPositionX >= BATTLE_GAME_UNUSED_X && LeftClickPositionX <= BATTLE_GAME_UNUSED_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && LeftClickPositionY >= BATTLE_GAME_UNUSED_Y && LeftClickPositionY <= BATTLE_GAME_UNUSED_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            battleGameInputs.placeUnit = (LeftClickPositionX - BATTLE_GAME_UNUSED_X)/BATTLE_GAME_PIC_WIDTH;
        }
        if(LeftClickPositionX >= BATTLE_GAME_SECTOR_X && LeftClickPositionX <= BATTLE_GAME_SECTOR_X+START_SECTOR_X*TILE_WIDTH
           && LeftClickPositionY >= BATTLE_GAME_SECTOR_Y && LeftClickPositionY <= BATTLE_GAME_SECTOR_Y+START_SECTOR_Y*TILE_HEIGHT)
        {
             battleGameInputs.placeUnitX = (LeftClickPositionX - BATTLE_GAME_SECTOR_X)/TILE_WIDTH;
             battleGameInputs.placeUnitY = (LeftClickPositionY - BATTLE_GAME_SECTOR_Y)/TILE_HEIGHT;
        }

        EventHandlerObject.setLeftMouseButton(false);
    }
    if(EventHandlerObject.checkRightMouseButton())
    {
        if(RightClickPositionX >= BATTLE_GAME_AVAILABLE_X && RightClickPositionX <= BATTLE_GAME_AVAILABLE_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && RightClickPositionY >= BATTLE_GAME_AVAILABLE_Y && RightClickPositionY <= BATTLE_GAME_AVAILABLE_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            int useX;
            useX = (RightClickPositionX - BATTLE_GAME_AVAILABLE_X)/BATTLE_GAME_PIC_WIDTH;
            battleGameInputs.SellUnits = useX;
        }
        if(RightClickPositionX >= BATTLE_GAME_SECTOR_X && RightClickPositionX <= BATTLE_GAME_SECTOR_X+START_SECTOR_X*TILE_WIDTH
           && RightClickPositionY >= BATTLE_GAME_SECTOR_Y && RightClickPositionY <= BATTLE_GAME_SECTOR_Y+START_SECTOR_Y*TILE_HEIGHT)
        {
             battleGameInputs.removeUnitX = (RightClickPositionX - BATTLE_GAME_SECTOR_X)/TILE_WIDTH;
             battleGameInputs.removeUnitY = (RightClickPositionY - BATTLE_GAME_SECTOR_Y)/TILE_HEIGHT;
        }
        EventHandlerObject.setRightMouseButton(false);
    }
    if(EventHandlerObject.checkKeyEscape())
    {
        bTerminateGame = true;
    }
}

void cInput::updateMouseActionsBattleGame()
{
    mousePosition.X = EventHandlerObject.getMouseX();
    mousePosition.Y = EventHandlerObject.getMouseY();
    if(EventHandlerObject.checkLeftMouseButton())
    {
        LeftClickPositionX = EventHandlerObject.getLeftMouseDownX();
        LeftClickPositionY = EventHandlerObject.getLeftMouseDownY();
    }
    else
    {
        LeftClickPositionX = 0;
        LeftClickPositionY = 0;
    }
    if(EventHandlerObject.checkRightMouseButton())
    {
        RightClickPositionX = EventHandlerObject.getRightMouseDownX();
        RightClickPositionY = EventHandlerObject.getRightMouseDownY();
    }
    else
    {
        RightClickPositionX = 0;
        RightClickPositionY = 0;
    }
}

void cInput::ClickGridLeft()
{
    if(LeftClickPositionY <= GRID_Y + GRID_PIC_HEIGHT)
    {
        if(LeftClickPositionX <= GRID_X + GRID_PIC_WIDTH)
        {
            EventHandlerObject.setKeyAbility(0,true);
        }
        if((LeftClickPositionX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(1,true);
        }
        if((LeftClickPositionX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(2,true);
        }
        if((LeftClickPositionX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(3,true);
        }
    }
    if((LeftClickPositionY >= GRID_Y + GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y) && (LeftClickPositionY <= GRID_Y + 2*GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y))
    {
        if(LeftClickPositionX <= GRID_X + GRID_PIC_WIDTH)
        {
            EventHandlerObject.setKeyA(true);
        }
        if((LeftClickPositionX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyS(true);
        }
        if((LeftClickPositionX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
           //HoldPosi
        }
        if((LeftClickPositionX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (LeftClickPositionX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            //moveCommand
        }
    }
}

void cInput::ClickGridRight()
{
    if(RightClickPositionY <= GRID_Y + GRID_PIC_HEIGHT)
    {
        if(RightClickPositionX <= GRID_X + GRID_PIC_WIDTH)
        {
            EventHandlerObject.setKeyAbility(0,true);
            EventHandlerObject.setKeyControl(true);
            unitSelectionInputs.rightClickGrid = true;
        }
        if((RightClickPositionX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(1,true);
            EventHandlerObject.setKeyControl(true);
            unitSelectionInputs.rightClickGrid = true;
        }
        if((RightClickPositionX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(2,true);
            EventHandlerObject.setKeyControl(true);
            unitSelectionInputs.rightClickGrid = true;
        }
        if((RightClickPositionX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyAbility(3,true);
            EventHandlerObject.setKeyControl(true);
            unitSelectionInputs.rightClickGrid = true;
        }
    }
    if((RightClickPositionY >= GRID_Y + GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y) && (RightClickPositionY <= GRID_Y + 2*GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y))
    {
        if(RightClickPositionX <= GRID_X + GRID_PIC_WIDTH)
        {
            EventHandlerObject.setKeyA(true);
        }
        if((RightClickPositionX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            EventHandlerObject.setKeyS(true);
        }
        if((RightClickPositionX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
           //HoldPosi
        }
        if((RightClickPositionX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (RightClickPositionX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            //moveCommand
        }
    }
}

sBattleGameInputs* cInput::getBattleGameInputs()
{
    return &battleGameInputs;
}

sScrollInputs* cInput::getScrollInputs()
{
    return &scrollInputs;
}

sUnitSelectionInputs* cInput::getUnitSelectionInputs()
{
    return &unitSelectionInputs;
}

void cInput::setActiveScreen(int scr)
{
    activeScreen = scr;
}

sMousePosition* cInput::getMousePosition()
{
    return &mousePosition;
}

bool* cInput::checkTerminateInput()
{
    return &bTerminateGame;
}

bool cInput::CleanUpInput()
{
    return 1;
}
