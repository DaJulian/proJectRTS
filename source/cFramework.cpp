#include "cFramework.h"

cFramework::cFramework()
{
    //Calling basic constructors
    cView();
    cOutput();

    //Assign zero for safety
    TimeThisFrame = 0;
    TimeLastFrame = 0;
    frameLength = 0;
}

bool cFramework::InitFramework()
{
    //document program start
    Error("Program starting \n");

    //read the filenames of all settings files
    std::string garbage;
    std::fstream f;
	f.open("Settings.txt", std::ios::in | std::ios::app);
	f >> garbage >> GRAPHIC_SETTINGS_FILE
	  >> garbage >> UNIT_SETTINGS_FILE
	  >> garbage >> MAP_SETTINGS_FILE
	  >> garbage >> INPUT_SETTINGS_FILE
	  >> garbage >> NET_SETTINGS_FILE
	  >> garbage >> BATTLE_GAME_SETTINGS_FILE;

    //Log the Settings files
    Error(GRAPHIC_SETTINGS_FILE.c_str());
    Error(UNIT_SETTINGS_FILE.c_str());
    Error(MAP_SETTINGS_FILE.c_str());
    Error(INPUT_SETTINGS_FILE.c_str());
    Error(NET_SETTINGS_FILE.c_str());

    f.close();

    //Initializing the actual game
    if((InitControl()) == false){ Error("Error initializing Control Settings");return false;}
    MapInit();
    //Initializing the output
    if((InitVideo(&MapObj,thisUser)) == false){ Error("Error initializing Video Settings");return false;}
    //Initializing the input
    if((InitInput()) == false){ Error("Error initializing Input Settings");return false;}

    InitSettingsBattleGame(UnitMovementSpeed,health,mana,healthreg,manareg,damage
                           ,sightRange,Armor,Magicarmor,tauntRange,sortPriority,NumberOfAblilites
                           ,UnitCost,BuildingTime,attackSpeed,attackRange,ManaCost,UnitEntityType);
    InitBattleGame(thisUser,&MapObj);

    Error("all initialized");


    //Inititalize Timers
    if((TimeThisFrame = SDL_GetTicks()) == false){ Error("Error getting system time");}
    TimeLastFrame = TimeThisFrame;
    frameLength = 0;
    //TODO write Systemtime to Error.log

    return 1;
}

bool cFramework::Run()
{
    while(bRun)
    {
        //Calculating timedifference in this frame
        if((TimeThisFrame = SDL_GetTicks()) == false){ Error("Error getting system time"); break;}
        frameLength = TimeThisFrame - TimeLastFrame;
        /*if(frameLength <= MIN_FRAME_LENGTH)
        {
            int timtest =SDL_GetTicks();
            Error("before waiting ",timtest);
            int waitfor = MIN_FRAME_LENGTH-frameLength;
            SDL_Delay(waitfor);
            frameLength = MIN_FRAME_LENGTH;
            Error("waited for ",waitfor);
            timtest =SDL_GetTicks();
            Error("after waiting ",timtest);
        }*/
        int timtest = TimeThisFrame;
        Error("TimeThisFrame ",timtest );
        timtest = TimeLastFrame;
        Error("TimeLastFrame ",timtest );
        timtest = frameLength;
        Error("framelength ",timtest );
        if(CONTROL_SETTINGS == MODE_REPLAY) bdoBattleGameScreen = false;
        if(bdoBattleGameScreen == true)
        {
            DoFrameBattleGame();
        }
        else
        {
            if(bStartSignal == true && startSector[0][0] != -3)
            {
                InitUnits(thisUser,startSector);
                startSector[0][0] = -3;
            }
            if (DoFrame() == false)
            {
                Error("Error doing the frame");
                //TODO show time
                break;
            }
        }
        //Update the time
        TimeLastFrame=TimeThisFrame;
    }

    return 1;
}

bool cFramework::DoFrame()
{
    //Handle Input
    Error("DoInput");

    if((DoInput(frameLength,&screenJumpRelativeX,&screenJumpRelativeY)) == false){ Error("Error handling input"); return false;}
    //Draw Graphics
    Error("DoDrawing");
    if((drawFrame(frameLength,MouseX,MouseY,cursorType,screenX,screenY,&doDrawRect,&DrawRect,pListHead[0],selectedNumber[0],selectedBuildings[0],selectedTypes[0],priorizedNumber[0]
        ,EntityList,NumberOfEntities,drawAoe)) == false){ Error("Error drawing"); return false;}
    //Mangae the actual game
    Error("DoControl");
    if((DoControl(frameLength,screenX,screenY,&bTerminateGame,&bSelect, &selMiniPic,selMiniPicNumber, &SelectRect, &selectOnlySpecificUnit, &SelectSpecificRect, specificUnitX,specificUnitY, &doMove,&attackMove,&stopMove
                  ,moveCommandX,moveCommandY,&GetGroup,&SetGroup,&JumpGroup,GroupNumber,&keyShift,&doSelectionCycle,&changeAbility,&castMove)) == false){ Error("Error In DoControl()"); return false;}

    Error("all done");
    return 1;
}

void cFramework::DoFrameBattleGame()
{
    DoBattleGameInput(frameLength,&BuyUnits,&SellUnits,&placeUnit,&placeUnitsX,&placeUnitsY,&removeUnitX,&removeUnitY);
    if(bTerminateGame == true) bRun = false;
    drawFrameBattleGameScreen(frameLength,MouseX,MouseY,readyUnits,placeUnit,ready,startSector,placeUnit);
    if(DoFrameBattleGameScreen(frameLength,&clickBattleGameReady) == true)
    {
        bdoBattleGameScreen = false;
        //Tell the server that the game is on
        if(CONTROL_SETTINGS == MODE_CLIENT)
        {
            Command* newCommand;
            newCommand = new Command;
            newCommand->commandType = COMMAND_CLIENT_READY;
            commandListObj.Add(newCommand);
        }
        if(CONTROL_SETTINGS == MODE_SINGPLAY)
        {
            bStartSignal= true;
            LoadStandardOpponent();
        }
    }
    NetworkManage(TimeThisFrame,&netPackToSend ,&netPackToStrip);
}

bool cFramework::CleanUpFramework()
{
    //Initializing the output
    if((CleanUpVideo()) == false){ Error("Error clearing Video Settings");return false;}
    //Initializing the input
    if((CleanUpInput()) == false){ Error("Error clearing Input Settings");return false;}
    //Initializing the input
    if((CleanUpControl()) == false){ Error("Error clearing Control");return false;}

    //document program end
    Error("terminating program \n");
    //TODO write System Time to Error.log

    return 1;
}
