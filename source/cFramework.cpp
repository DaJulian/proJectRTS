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
    if((controlObject.InitControl(&viewObject, &gameModelObj)) == false){ Error("Error initializing Control Settings");return false;}
    controlObject.MapInit();
    //Initializing the output
    if((viewObject.InitView(&gameModelObj,battleGameObject.getBattleGameModel(), controlObject.getUser())) == false){ Error("Error initializing view Settings");return false;}

    Error("before initializing battelgame");

    battleGameObject.InitSettingsBattleGame(gameModelObj.getUnitSpecifications());
    battleGameObject.InitBattleGame(controlObject.getUser(), gameModelObj.getMapObj(), viewObject.getBattleGameInputs());

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
    while(controlObject.checkRunning())
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

        if(CONTROL_SETTINGS == MODE_REPLAY) battleGameObject.setRun(false);
        if(battleGameObject.checkRun())
        {
            Error("Do Battle Game Frame");
            DoFrameBattleGame();
        }
        else
        {
            sBattleGameModel* pMod = battleGameObject.getBattleGameModel();
            if(pMod->ready && pMod->startSector[0][0] != -3)
            {
                controlObject.InitUnits(pMod->startSector);
                pMod->startSector[0][0] = -3;
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

    //if((DoInput(frameLength,&screenJumpRelativeX,&screenJumpRelativeY)) == false){ Error("Error handling input"); return false;}
    //Draw Graphics
    Error("DoDrawing");
    //if((drawFrame(frameLength,MouseX,MouseY,cursorType,screenX,screenY,&doDrawRect,&DrawRect,pListHead[0],selectedNumber[0],selectedBuildings[0],selectedTypes[0],priorizedNumber[0]
    //    ,EntityList,NumberOfEntities,drawAoe)) == false){ Error("Error drawing"); return false;}
    viewObject.DoFrameView(frameLength);


    //Mangae the actual game
    Error("DoControl");
    //if((DoControl(frameLength,screenX,screenY,&bTerminateGame,&bSelect, &selMiniPic,selMiniPicNumber, &SelectRect, &selectOnlySpecificUnit, &SelectSpecificRect, specificUnitX,specificUnitY, &doMove,&attackMove,&stopMove
    //              ,moveCommandX,moveCommandY,&GetGroup,&SetGroup,&JumpGroup,GroupNumber,&keyShift,&doSelectionCycle,&changeAbility,&castMove)) == false){ Error("Error In DoControl()"); return false;}
    if(!(controlObject.DoControl(frameLength, viewObject.checkTerminateInput()))){Error("Error drawing and inputing");}

    Error("all done");
    return 1;
}

void cFramework::DoFrameBattleGame()
{
    //DoBattleGameInput(frameLength);
    //if(bTerminateGame == true) bRun = false;
    //drawFrameBattleGameScreen(frameLength);
    viewObject.DoFrameView(frameLength);
    if(battleGameObject.DoFrameBattleGameScreen(frameLength) == true)
    {
        sBattleGameModel* pMod = battleGameObject.getBattleGameModel();
        battleGameObject.setRun(false);
        //Tell the server that the game is on
        if(CONTROL_SETTINGS == MODE_CLIENT)
        {
            controlObject.storeCommand(NULL, COMMAND_CLIENT_READY ,0,0,1);
        }
        if(CONTROL_SETTINGS == MODE_SINGPLAY)
        {
            pMod->ready = true;
            controlObject.LoadStandardOpponent();
        }
        Error("battle game set ready");
        viewObject.setActiveScreen(SCREEN_MAP_UNIT);
        ///controlObject.setActiveScreen(SCREEN_MAP_UNIT);
    }
    ///controlObject.NetworkManage(TimeThisFrame, &netPackToSend ,&netPackToStrip);
}

bool cFramework::CleanUpFramework()
{
    //Initializing the output
    ///if((CleanUpVideo()) == false){ Error("Error clearing Video Settings");return false;}
    //Initializing the input
    ///if((CleanUpInput()) == false){ Error("Error clearing Input Settings");return false;}
    //Initializing the input
    if((controlObject.CleanUpControl()) == false){ Error("Error clearing Control");return false;}

    //document program end
    Error("terminating program \n");
    //TODO write System Time to Error.log

    return 1;
}
