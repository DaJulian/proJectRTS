#include "cInput.h"

cInput::cInput()
{
    screenX = 0;
    screenY = 0;

    SCROLL_SPEED_KEYBOARD = 100;
    SCROLL_SPEED_MOUSE    = 100;
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

    //ScrollDelays set to 0
    ScrollDelayX = 0;
    ScrollDelayY = 0;

    //Initialize all mouse buttons to be not pressed
    LMButton = false;
    RMButton = false;
    MMButton = false;
    blockLeft = false;
    cursorType = CURSOR_NORMAL;

    doDrawRect= false;
    doSelectionCycle = false;

    rightClickGrid = false;

    selMiniPic = false;
    selMiniPicNumber = 1;

        //Command bools
   doMove = false;
   attackMove = false;
   stopMove = false;

   moveCommandX = 0;
   moveCommandY = 0;

   GetGroup = false;
   SetGroup = false;
   GroupNumber = 0;

   selectOnlySpecificUnit = false;

    //Set klickpositions to 0
    LBcX = 0;
    LBcY = 0;
    RBcX = 0;
    RBcY = 0;
    LmXd = 0;
    LmYd = 0;

    //DrawRectangle set to 0
    DrawRect.x = 0;
    DrawRect.y = 0;
    DrawRect.w = 0;
    DrawRect.h = 0;

    //SelectRectangle set to 0 and no selection to do
    bSelect = false;
    SelectRect.x = 0;
    SelectRect.y = 0;
    SelectRect.w = 0;
    SelectRect.h = 0;

    //Tell the game not to terminate
    bTerminateGame = false;

    changeAbility = -1;
    castMove = -1;
    keyAbility[0] = false;
    keyAbility[1] = false;
    keyAbility[2] = false;
    keyAbility[3] = false;

    //Battle game screen variables
    clickBattleGameReady = false;

    return 1;
}

bool cInput::DoInput(Uint32 ticks)
{
	SDL_Event Event;
	while(SDL_PollEvent(&Event))
	{
		UseEvent(&Event);
	}

    if(keyEscape == true)
    {
        bTerminateGame = true;
        keyEscape = false;
        return 1;
    }

	switch(activeScreen)
	{
	    case SCREEN_BATTLE_GAME:
	    {
	        DoInputBattleGame();
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

    //The following actions can only be applied if it is not a replay
    if(CONTROL_SETTINGS != MODE_REPLAY)
    {
        handleAttackMove();
        handleCastCommand();

        //Clicking somewhere without an active attackmove or castmove
        if(LMButton == true && blockLeft == false && attackMove == false && castMove == -1)
        {
            handleLeftClickOnMap();
            handleLeftClickOnHud();
        }
        if(LMButton == false)
        {
            doDrawRect = false;
        }
        if(RMButton == true)
        {
            if(attackMove == false && castMove == -1)
            {
                handleRightClickOnMap();
                handleRightClickOnHud();
            }
            else
            {
                attackMove = false;
                castMove = -1;
            }

            RMButton = false;
        }
        //switching attack move
        if(keyA == true)
        {
            if(attackMove == true)
            {
                attackMove = false;
            }
            else
            {
                attackMove = true;
            }
            keyA = false;
        }
        //giving a stop command
        if(keyS == true)
        {
            stopMove = true;
            keyS = false;
        }
        //Check assigning and accessing groups
        for(int i=0;i < 10; i++)
        {
            if(keyControl == true)
            {
                if(keyNum[i] == true)
                {
                    SetGroup = true;
                    GroupNumber = i;
                    if(i == 0){GroupNumber = 10;}
                    keyNum[i] = false;
                    keyControl = false;
                }
            }
            else
            {
                if(keyNum[i] == true)
                {
                    GetGroup = true;
                    if(doubleButton > 0 && GroupNumber == i)
                    {
                        JumpGroup=true;
                    }
                    GroupNumber = i;
                    if(i == 0){GroupNumber = 10;}
                    keyNum[i] = false;
                    doubleButton = DOUBLE_BUTTON_TIME;
                }
            }
        }

        if(keyControl == true)
        {
            for(int i=0;i < 4; i++)
            {
                if(keyAbility[i] == true || castMove == i)
                {
                    Error("change Ability ",changeAbility);
                    changeAbility = i;
                    keyAbility[i] = false;
                    if(rightClickGrid ==true){keyControl = false;rightClickGrid=false;}
                    if(castMove == i){castMove=-1;}
                }
            }
        }

        //Check if cast moves will be applied
        for(int i=0;i < 4; i++)
        {
            if(keyAbility[i] == true )
            {
                if(castMove == -1)
                {
                    castMove = i;
                    keyAbility[i] = false;
                }
                else
                {
                    castMove = -1;
                    keyAbility[i] = false;
                }
            }
        }

        if(keyTab == true)
        {
            doSelectionCycle = true;
            keyTab = false;
        }

        //Set Cursortype
        if(attackMove == true)
        {
            cursorType = CURSOR_ATTACK;
        }
        else
        {
            if(castMove != -1)
            {
                cursorType = CURSOR_CAST;
            }
            else
            {
                cursorType = CURSOR_NORMAL;
            }
        }
    }

    return 1;

}

void cInput::DoInputScroll(Uint32 ticks)
{
    applyScreenJump();

    tickDownScrollDelay(ticks);
    handleScrolling();

    guaranteeToBeOnMap();
}

void cInput::DoInputUnitSelection()
{

}

void cInput::DoInputBuildingSelection()
{

}

void cInput::DoInputInnScreen()
{

}

void cInput::applyScreenJump()
{
    if(screenJumpRelativeX != 0)
    {
        screenX = screenX + screenJumpRelativeX;
        LmXd = 0;
        screenJumpRelativeX = 0;
    }
    if(screenJumpRelativeY != 0)
    {
        screenY = screenY + screenJumpRelativeY;
        LmYd = 0;
        screenJumpRelativeY = 0;
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
    if(keyUp == true)
    {
        //Scroll up if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            screenY--;
            ScrollDelayY = SCROLL_SPEED_KEYBOARD;
            if(screenY<0) screenY =0;
        }
    }
    if(keyLeft == true)
    {
        //Scroll left if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            screenX--;
            ScrollDelayX = SCROLL_SPEED_KEYBOARD;
            if(screenX<0) screenX =0;
        }
    }
    if(keyDown == true)
    {
        //Scroll down if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            screenY++;
            ScrollDelayY = SCROLL_SPEED_KEYBOARD;
            if(screenY>WORLD_HEIGHT -SCR_TILE_HEIGHT) screenY = WORLD_HEIGHT -SCR_TILE_HEIGHT;
        }
    }
    if(keyRight == true)
    {
        //Scroll right if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            screenX++;
            ScrollDelayX = SCROLL_SPEED_KEYBOARD;
            if(screenX>WORLD_WIDTH -SCR_TILE_WIDTH) screenX = WORLD_WIDTH -SCR_TILE_WIDTH;
        }
    }
}

void cInput::handleMouseScrolling()
{
    if(MouseY < 10)
    {
        //Scroll up if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            screenY--;
            ScrollDelayY = SCROLL_SPEED_MOUSE;
            if(screenY<0) screenY =0;
        }
    }
    if(MouseX < 10)
    {
        //Scroll left if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            screenX--;
            ScrollDelayX = SCROLL_SPEED_MOUSE;
            if(screenX<0) screenX =0;
        }
    }
    if(MouseY > SCREEN_HEIGHT-10)
    {
        //Scroll down if program didnt scroll recently
        if(ScrollDelayY == 0)
        {
            screenY++;
            ScrollDelayY = SCROLL_SPEED_MOUSE;
            if(screenY>WORLD_HEIGHT -SCR_TILE_HEIGHT) screenY = WORLD_HEIGHT -SCR_TILE_HEIGHT;
        }
    }
    if(MouseX > SCREEN_WIDTH-10)
    {
        //Scroll right if program didnt scroll recently
        if(ScrollDelayX == 0)
        {
            screenX++;
            ScrollDelayX = SCROLL_SPEED_MOUSE;
            if(screenX>WORLD_WIDTH -SCR_TILE_WIDTH) screenX = WORLD_WIDTH -SCR_TILE_WIDTH;
        }
    }
}

void guaranteeToBeOnMap()
{
    if(screenX > WORLD_WIDTH-SCR_TILE_WIDTH){screenX = WORLD_WIDTH-SCR_TILE_WIDTH;}
    if(screenY > WORLD_HEIGHT-SCR_TILE_HEIGHT){screenY = WORLD_HEIGHT-SCR_TILE_HEIGHT;}
    if(screenX < 0){screenX = 0;}
    if(screenY < 0){screenY = 0;}
}

void cInput::handleAttackMove()
{
    if(LMButton == true && blockLeft == false && attackMove == true && castMove == -1)
    {
        //attackmove on the screen
        if(LBcX <= SCR_TILE_WIDTH*TILE_WIDTH && LBcY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
        {
            moveCommandX = screenX + (LBcX / TILE_WIDTH);
            moveCommandY = screenY + (LBcY / TILE_HEIGHT);
            doMove = true;
        }
        //attackmove on the minimap
        if(LBcX >= MINIMAP_X && LBcX <= MINIMAP_X+MINIMAP_WIDTH && LBcY >= MINIMAP_Y && LBcY <= MINIMAP_Y+MINIMAP_HEIGHT)
        {
            moveCommandX = (LBcX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
            moveCommandY = (LBcY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
            doMove = true;
        }
        blockLeft = true;
    }
}

void cInput::handleCastCommand()
{
    if(LMButton == true && blockLeft == false && castMove != -1 && attackMove == false)
    {
        //Cast COmmand On the screen
        if(LBcX <= SCR_TILE_WIDTH*TILE_WIDTH && LBcY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
        {
            moveCommandX = screenX + (LBcX / TILE_WIDTH);
            moveCommandY = screenY + (LBcY / TILE_HEIGHT);
            doMove = true;
        }
        //Cast Command On the minimap
        if(LBcX >= MINIMAP_X && LBcX <= MINIMAP_X+MINIMAP_WIDTH && LBcY >= MINIMAP_Y && LBcY <= MINIMAP_Y+MINIMAP_HEIGHT)
        {
            moveCommandX = (LBcX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
            moveCommandY = (LBcY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
            doMove = true;
        }
        blockLeft = true;
    }
}

void cInput::handleLeftClickOnMap()
{
    //draw a rectangle on the screen
    if(LBcX <= SCR_TILE_WIDTH*TILE_WIDTH && LBcY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
    {
        if(keyControl == true)
        {
            selectOnlySpecificUnit = true;
            SelectSpecificRect.w = SCR_TILE_WIDTH*TILE_WIDTH;
            SelectSpecificRect.h = SCR_TILE_HEIGHT*TILE_HEIGHT;
            SelectSpecificRect.x = 0;
            SelectSpecificRect.y = 0;
            specificUnitX = LBcX/TILE_WIDTH + screenX;
            specificUnitY = LBcY/TILE_HEIGHT + screenY;
            if(specificUnitX < 0){specificUnitX = 0;}
            if(specificUnitX >= WORLD_WIDTH){specificUnitX = WORLD_WIDTH-1;}
            if(specificUnitY < 0){specificUnitY = 0;}
            if(specificUnitY >= WORLD_HEIGHT){specificUnitY = WORLD_HEIGHT-1;}
            LMButton = false;
            blockLeft = true;
        }
        else
        {
            doDrawRect = true;
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
    if(LBcX >= MINIMAP_X && LBcX <= MINIMAP_X+MINIMAP_WIDTH && LBcY >= MINIMAP_Y && LBcY <= MINIMAP_Y+MINIMAP_HEIGHT)
    {
        screenX = (LBcX - MINIMAP_X)/MINIMAP_TILE_WIDTH - SCR_TILE_WIDTH/2;
        screenY = (LBcY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT - SCR_TILE_HEIGHT/2;
    }
    //sliding over the minimap with pressed left mouse button
    if(LmXd >= MINIMAP_X && LmXd <= MINIMAP_X+MINIMAP_WIDTH && LmYd >= MINIMAP_Y && LmYd <= MINIMAP_Y+MINIMAP_HEIGHT)
    {
        screenX = (LmXd - MINIMAP_X)/MINIMAP_TILE_WIDTH - SCR_TILE_WIDTH/2;
        screenY = (LmYd - MINIMAP_Y)/MINIMAP_TILE_HEIGHT - SCR_TILE_HEIGHT/2;
    }
}

void cInput::handleLeftClickOnRightHud()
{
    //the minipic range is clicked; select the clicked unit
    if(LBcX >= MINIPIC_X && LBcY >= MINIPIC_Y && LBcX <= (MINIPIC_X + MINIPIC_COLUMNS*MINIPIC_WIDTH) && LBcY <= (MINIPIC_Y + MINIPIC_ROWS*MINIPIC_HEIGHT))
    {
        selMiniPicNumber = 10;
        int selX,selY;
        selX = ((LBcX - MINIPIC_X))/MINIPIC_WIDTH + 1;
        selY = ((LBcY - MINIPIC_Y))/MINIPIC_HEIGHT;
        selMiniPicNumber = selX + selY*MINIPIC_COLUMNS;
        ///DEBUG
        //selMiniPicNumber = 2;
        selMiniPic = true;
        LMButton = false;
    }
    if(LBcX >= GRID_X && LBcY >= GRID_Y)
    {
        ClickGridLeft();
    }
}

void cInput::handleRightClickOnMap()
{
    if(RBcX <= SCR_TILE_WIDTH*TILE_WIDTH && RBcY <= SCR_TILE_HEIGHT*TILE_HEIGHT)
    {
        moveCommandX = screenX + (RBcX / TILE_WIDTH);
        moveCommandY = screenY + (RBcY / TILE_HEIGHT);
        doMove = true;
    }
}

void cInput::handleRightClickOnHud()
{
    handleRightClickOnMinimap();
    handleRightClickOnRightHud();
}

void cInput::handleRightClickOnMinimap()
{
    if(RBcX >= MINIMAP_X && RBcX <= MINIMAP_X+MINIMAP_WIDTH && RBcY >= MINIMAP_Y && RBcY <= MINIMAP_Y+MINIMAP_HEIGHT)
    {
        moveCommandX = (RBcX - MINIMAP_X)/MINIMAP_TILE_WIDTH;
        moveCommandY = (RBcY - MINIMAP_Y)/MINIMAP_TILE_HEIGHT;
        doMove = true;
    }
}

void cInput::handleRightClickOnRightHud()
{
    if(RBcX >= GRID_X && RBcY >= GRID_Y)
    {
        ClickGridRight();
    }
}

void cInput::DoBattleGameInput(Uint32 t,int* BuyUnits,int* SellUnits,int* placeUnit, int* placeUnitX,int* placeUnitY,
                              int* removeUnitX, int* removeUnitY)
{
    //handle Input
	SDL_Event Event;

	while(SDL_PollEvent(&Event))
	{
		UseEvent(&Event);
	}
    if(LMButton == true)
    {
        if(LBcX >= BATTLE_GAME_GOBUTTON_X && LBcX <= BATTLE_GAME_GOBUTTON_X+BATTLE_GAME_GOBUTTON_WIDTH
           && LBcY >= BATTLE_GAME_GOBUTTON_Y && LBcY <= BATTLE_GAME_GOBUTTON_Y+BATTLE_GAME_GOBUTTON_HEIGHT)
        {
            clickBattleGameReady = true;
        }
        //if(LBcX >= BATTLE_GAME_AVAILABLE_X && )
        if(LBcX >= BATTLE_GAME_AVAILABLE_X && LBcX <= BATTLE_GAME_AVAILABLE_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && LBcY >= BATTLE_GAME_AVAILABLE_Y && LBcY <= BATTLE_GAME_AVAILABLE_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            int useX;
            useX = (LBcX - BATTLE_GAME_AVAILABLE_X)/BATTLE_GAME_PIC_WIDTH;
            *BuyUnits = useX;
        }
        if(LBcX >= BATTLE_GAME_UNUSED_X && LBcX <= BATTLE_GAME_UNUSED_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && LBcY >= BATTLE_GAME_UNUSED_Y && LBcY <= BATTLE_GAME_UNUSED_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            *placeUnit = (LBcX - BATTLE_GAME_UNUSED_X)/BATTLE_GAME_PIC_WIDTH;
        }
        if(LBcX >= BATTLE_GAME_SECTOR_X && LBcX <= BATTLE_GAME_SECTOR_X+START_SECTOR_X*TILE_WIDTH
           && LBcY >= BATTLE_GAME_SECTOR_Y && LBcY <= BATTLE_GAME_SECTOR_Y+START_SECTOR_Y*TILE_HEIGHT)
        {
             *placeUnitX = (LBcX - BATTLE_GAME_SECTOR_X)/TILE_WIDTH;
             *placeUnitY = (LBcY - BATTLE_GAME_SECTOR_Y)/TILE_HEIGHT;
        }

        LMButton = false;
    }
    if(RMButton ==true)
    {
        if(RBcX >= BATTLE_GAME_AVAILABLE_X && RBcX <= BATTLE_GAME_AVAILABLE_X+UNIT_TYPE_NUMBER*BATTLE_GAME_PIC_WIDTH
           && RBcY >= BATTLE_GAME_AVAILABLE_Y && RBcY <= BATTLE_GAME_AVAILABLE_Y+BATTLE_GAME_PIC_HEIGHT)
        {
            int useX;
            useX = (RBcX - BATTLE_GAME_AVAILABLE_X)/BATTLE_GAME_PIC_WIDTH;
            *SellUnits = useX;
        }
        if(RBcX >= BATTLE_GAME_SECTOR_X && RBcX <= BATTLE_GAME_SECTOR_X+START_SECTOR_X*TILE_WIDTH
           && RBcY >= BATTLE_GAME_SECTOR_Y && RBcY <= BATTLE_GAME_SECTOR_Y+START_SECTOR_Y*TILE_HEIGHT)
        {
             *removeUnitX = (RBcX - BATTLE_GAME_SECTOR_X)/TILE_WIDTH;
             *removeUnitY = (RBcY - BATTLE_GAME_SECTOR_Y)/TILE_HEIGHT;
        }
        RMButton = false;
    }
    if(keyEscape == true)
    {
        bTerminateGame = true;
    }
}

void cInput::ClickGridLeft()
{
    if(LBcY <= GRID_Y + GRID_PIC_HEIGHT)
    {
        if(LBcX <= GRID_X + GRID_PIC_WIDTH)
        {
            keyAbility[0]=true;
        }
        if((LBcX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            keyAbility[1]=true;
        }
        if((LBcX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
            keyAbility[2]=true;
        }
        if((LBcX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            keyAbility[3]=true;
        }
    }
    if((LBcY >= GRID_Y + GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y) && (LBcY <= GRID_Y + 2*GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y))
    {
        if(LBcX <= GRID_X + GRID_PIC_WIDTH)
        {
            keyA=true;
        }
        if((LBcX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            keyS=true;
        }
        if((LBcX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
           //HoldPosi
        }
        if((LBcX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (LBcX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            //moveCommand
        }
    }
}

void cInput::ClickGridRight()
{
    if(RBcY <= GRID_Y + GRID_PIC_HEIGHT)
    {
        if(RBcX <= GRID_X + GRID_PIC_WIDTH)
        {
            keyAbility[0]=true;
            keyControl = true;
            rightClickGrid = true;
        }
        if((RBcX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            keyAbility[1]=true;
            keyControl = true;
            rightClickGrid = true;
        }
        if((RBcX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
            keyAbility[2]=true;
            keyControl = true;
            rightClickGrid = true;
        }
        if((RBcX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            keyAbility[3]=true;
            keyControl = true;
            rightClickGrid = true;
        }
    }
    if((RBcY >= GRID_Y + GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y) && (RBcY <= GRID_Y + 2*GRID_PIC_HEIGHT + 3*GRID_HALFSPACE_Y))
    {
        if(RBcX <= GRID_X + GRID_PIC_WIDTH)
        {
            keyA=true;
        }
        if((RBcX >= GRID_X + GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 2*GRID_PIC_WIDTH + 3*GRID_HALFSPACE_X))
        {
            keyS=true;
        }
        if((RBcX >= GRID_X + 2*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 3*GRID_PIC_WIDTH + 5*GRID_HALFSPACE_X))
        {
           //HoldPosi
        }
        if((RBcX >= GRID_X + 3*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X) && (RBcX <= GRID_X + 4*GRID_PIC_WIDTH + 7*GRID_HALFSPACE_X))
        {
            //moveCommand
        }
    }
}

bool cInput::CleanUpInput()
{
    return 1;
}

bool cInput::UseEvent(SDL_Event* Event)
{
    cEvent::UseEvent(Event);
	return 1;
}

void cInput::InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
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

void cInput::InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
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

void cInput::InpMouseMove(int mX, int mY, int relX, int relY, bool LeftB, bool RightB, bool MiddleB)
{
    MouseX = mX;
    MouseY = mY;
    if(LMButton)
    {
        LmXd=mX;
        LmYd=mY;
            //Set Rectangle for Drawing
            if(mX>=LBcX)
            {
                DrawRect.x = LBcX;
                DrawRect.w = mX - LBcX;
            }
            else
            {
                DrawRect.x = mX;
                DrawRect.w = LBcX - mX;
            }

            if(mY>=LBcY)
            {
                DrawRect.y = LBcY;
                DrawRect.h = mY - LBcY;
            }
            else
            {
                DrawRect.y = mY;
                DrawRect.h = LBcY - mY;
            }
        //if(mY>=MINIMAP_Y && doDrawRect == true)
        //{
           // bSelect = true;
           // doDrawRect = false;
        //}

    }
}

void cInput::InpMouseButtonDown(Uint8 button,int mX, int mY)
{
    switch(button)
    {
        case SDL_BUTTON_LEFT:
        {
            if(blockLeft == false)
            {
                LMButton = true;
                LBcX = mX;
                LBcY = mY;
            }
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

void cInput::InpMouseButtonUp(Uint8 button,int mX, int mY)
{
    switch(button)
    {
        case SDL_BUTTON_LEFT:
        {
            LMButton = false;
            if(blockLeft == false)
            {
                //if(doDrawRect){bSelect = true;}
                if(mX <= SCR_TILE_WIDTH*TILE_WIDTH && mY <= SCR_TILE_HEIGHT*TILE_HEIGHT && attackMove == false)
                {
                    bSelect = true;
                }
                //SetRectangleForSelection
                if(mX>=LBcX)
                {
                    SelectRect.x = LBcX;
                    SelectRect.w = mX - LBcX;
                }
                else
                {
                    SelectRect.x = mX;
                    SelectRect.w = LBcX - mX;
                }

                if(mY>=LBcY)
                {
                    SelectRect.y = LBcY;
                    SelectRect.h = mY - LBcY;
                }
                else
                {
                    SelectRect.y = mY;
                    SelectRect.h = LBcY - mY;
                }

                //Set Mousedown Positions to 0 for safety
                LBcX = 0;
                LBcY = 0;
                DrawRect.x = 0;
                DrawRect.y = 0;
                DrawRect.w = 0;
                DrawRect.h = 0;
                break;
            }
            blockLeft = false;

        }
        case SDL_BUTTON_RIGHT:
        {
            RMButton = false;
            doMove = false;
            RBcX = 0;
            RBcY = 0;
            break;
        }
    }
}
