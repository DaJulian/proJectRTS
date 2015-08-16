#include "cControl.h"

bool cControl::InitControl(cView* pViewObj, cGameModel* pGameModelObj)
{
    pView = pViewObj;
    pGameModel = pGameModelObj;

    TimeSinceStart = 0;
    lastPackID = 0;

    //tell program to run
    bRun = true;
    bStartSignal = false;
    pView->setDrawAoe(-1);

    pGameModel->InitGameModel();

    pMousePosition = pView->getMousePosition();
    pUnitSelectionInputs = pView->getUnitSelectionInputs();
    pScrollInputs = pView->getScrollInputs();

    std::string garbage;
    std::fstream f;

    //Network settings;
    f.open(NET_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);
    f >> garbage >> CONTROL_SETTINGS
      >> garbage >> IP_ADDY
      >> garbage >> PORT
      >> garbage >> REPLAY_FILE
      >> garbage >> thisUser;
    f.close();

    Error("controlSettings",CONTROL_SETTINGS);
    Error(PORT);
    Error(REPLAY_FILE);

    WRITE_REPLAY = true;
    if(CONTROL_SETTINGS == MODE_SERVER)
    {
        // Initialize the Network; weit for connections etc;
        InitNetServer(thisUser);
    }
    if(CONTROL_SETTINGS == MODE_CLIENT)
    {
        // Initialize the Network; weit for connections etc;
        InitNetClient(thisUser);
    }
    if(CONTROL_SETTINGS == MODE_REPLAY)
    {
        // do what a replay should do
        ReplayObj.InitRead();
    }
    else
    {
        ReplayObj.InitWrite();
    }

    pScrollInputs->screenJumpRelativeX = 0;
    pScrollInputs->screenJumpRelativeY = 0;

    //Initialize the pathFinder TODO more things then just map
    pathFindObj.Init();
    pathFindObj.setMap(pGameModel->getMapObj());

    return 1;
}

void cControl::MapInit()
{
    pGameModel->InitMap(MAP_SETTINGS_FILE, &changeListObj);
}

void cControl::LoadStandardOpponent()
{

}

void cControl::InitUnits(int InitSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX])
{
    Change* newChange;
    Command* newCommand;
    int ID=0;
    for(int i=0;i<START_SECTOR_X;i++)
    {
        for(int j=0;j<START_SECTOR_Y;j++)
        {
            if(InitSector[i][j] >= 0)
            {
                if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
                {
                    newChange = new Change;
                    newChange->UnitID = ID;
                    ID++;
                    if(thisUser == 1)
                    {
                        newChange->posiX = PLAYER_1_START_X+i;
                        newChange->posiY = PLAYER_1_START_Y+j;

                    }
                    if(thisUser == 2)
                    {
                        newChange->posiX = PLAYER_2_START_X+i;
                        newChange->posiY = PLAYER_2_START_Y+j;

                    }

                    newChange->goalX = thisUser;
                    newChange->goalY = 0;
                    newChange->goalUnitID = InitSector[i][j];
                    newChange->time = 0;
                    newChange->changeType = CHANGE_NEW;

                    changeListObj.Add(newChange);
                }
                if(CONTROL_SETTINGS == MODE_CLIENT)
                {
                    //DO A SEND OF WHAT HE DID
                    newCommand = new Command;
                    newCommand->commandType = COMMAND_INIT_SPAWN;
                    //ID++;
                    if(thisUser ==1)
                    {
                        newCommand->posiX = PLAYER_1_START_X+i;
                        newCommand->posiY = PLAYER_1_START_Y+j;

                    }
                    if(thisUser ==2)
                    {
                        newCommand->posiX = PLAYER_2_START_X+i;
                        newCommand->posiY = PLAYER_2_START_Y+j;

                    }
                    newCommand->UnitID = InitSector[i][j];
                    newCommand->goalX = thisUser;
                    newCommand->goalY = 0;
                    newCommand->priority = 0;
                    newCommand->time = 0;
                    commandListObj.Add(newCommand);
                }
            }
        }
    }
    pGameModel->setNewestID(ID);
    if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
    {
        cChangeList bufferList;
        Change* giveNext;
        while((giveNext = changeListObj.getFirst())!=NULL)
        {
            pGameModel->CreateUnit(giveNext->UnitID,giveNext->goalUnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
            bufferList.Add(giveNext);
        }
        while((giveNext = bufferList.getFirst())!=NULL)
        {
            changeListObj.Add(giveNext);
        }
    }

    ///TODO just a test
    pGameModel->CreateBuilding(0,6,6,1);

}

bool cControl::DoControl(Uint32 t, bool* TermGame )
{
    //Update the overall time
    TimeSinceStart += t;
    int timetest = TimeSinceStart;
    Error("TimeSinceStart in Control ",timetest);
    if(*TermGame == true)
    {
        //End the game (TODO when having a finalizing screen open it)
        storeCommand(NULL,COMMAND_TERMINATE_GAME,0, 0,0);
        if(CONTROL_SETTINGS == MODE_CLIENT || CONTROL_SETTINGS == MODE_REPLAY){bRun = false;}
        *TermGame = false;
    }

    if(pUnitSelectionInputs->doSelectionCycle)
    {
        cyclePriorization(0);
        pUnitSelectionInputs->doSelectionCycle = false;
    }

    //Check if the new cast command can actually be cast right now; if not undo the command
    if(pUnitSelectionInputs->castMove != -1 && checkNewSpell == true)
    {
        cSelection* toMove;
        toMove = getFirstPriorizedUnit(0);
        int ThisType;
        if(toMove!= NULL)
        {if(toMove->getContent() != NULL) ThisType = toMove->getContent()->getType();}
        bool checkDone = false;
        while(toMove != NULL)
        {
            if(toMove->getContent()!=NULL && toMove->getContent()->getType() == ThisType)
            {
                if(checkCastable(toMove->getContent(),pUnitSelectionInputs->castMove))
                {
                    checkDone = true;
                    break;
                }
            }
            toMove = toMove->getSuccessor();
        }
        checkNewSpell = false;
        if(checkDone == false) pUnitSelectionInputs->castMove = -1;
    }
    //reset the checking variable
    if(pUnitSelectionInputs->castMove == -1) checkNewSpell = true;

    //tell aoe Draw range
    drawAoe = -1;
    if(selectedNumber[0] > 0 && pUnitSelectionInputs->castMove != -1)
    {
        cSelection* toMove;
        toMove = getFirstPriorizedUnit(0);
        sEntitySpecifications* enSpec;
        enSpec = pGameModel->getEntitySpecifications();
        if(toMove->getContent()->getNumberOfAblilities() > pUnitSelectionInputs->castMove)
        {
            if(enSpec->modifierType[toMove->getContent()->getEnttype(pUnitSelectionInputs->castMove)][0] >= ENT_PROP_AOE)
            {
                drawAoe = enSpec->modifierValue[toMove->getContent()->getEnttype(pUnitSelectionInputs->castMove)][0];
                //Error("setting aoe range in COntrol to ", drawAoe);
            }
        }
    }
    pView->setDrawAoe(drawAoe);
    //Do a selection
    if( pUnitSelectionInputs->bSelect)
    {
        //Working variables Rectangle sizes in units of tiles
        int sx,sy,sh,sw;

        //determine the position of the rectangle in units of tiles NOTE: first adding sy(sx) and then subtracting after you have it in units of tiles yields the real eidth in tiles
        sx = pUnitSelectionInputs->SelectRect.x;
        sy = pUnitSelectionInputs->SelectRect.y;
        sh = pUnitSelectionInputs->SelectRect.h + sy;
        sw = pUnitSelectionInputs->SelectRect.w + sx;
        sx = (sx - (sx % TILE_WIDTH)) / TILE_WIDTH;
        sy = (sy - (sy % TILE_HEIGHT)) / TILE_HEIGHT;
        sh = (sh - (sh % TILE_HEIGHT)) / TILE_HEIGHT;
        sh = sh - sy + 1;
        sw = (sw - (sw % TILE_WIDTH)) / TILE_WIDTH;
        sw = sw - sx + 1;
        //remove a bug that cause a collapse when selection to close to the corner
        if(pScrollInputs->screenX+sx+sw > WORLD_WIDTH){sw = WORLD_WIDTH-sx-pScrollInputs->screenX;}
        if(pScrollInputs->screenY+sy+sh > WORLD_HEIGHT){sh = WORLD_HEIGHT-sy-pScrollInputs->screenY;}

        if(!(pUnitSelectionInputs->onlyAddSelection)) ClearSelection(0);
        //Build a list from all the tiles in the selec
        for(int i=0;i<sw;i++)
        {
            for(int j=0;j<sh;j++)
            {
                if(pUnitSelectionInputs->onlyAddSelection) RemoveSelectionByTile(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j),0);
                AddSelectionByTile(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j),thisUser,0);
            }
        }
        sortSelection(0);

        //Mark selection as done
        pUnitSelectionInputs->bSelect = false;
    }
    if(pUnitSelectionInputs->selectOnlySpecificUnit)
    {
        Error("specific selectX",pUnitSelectionInputs->specificUnitX);
        Error("specific selectY", pUnitSelectionInputs->specificUnitY);
        if(pGameModel->getMap(pUnitSelectionInputs->specificUnitX,pUnitSelectionInputs->specificUnitY)->getContainUnit() != NULL)
        {
            int sx,sy,sh,sw;
            int ThisType = pGameModel->getMap(pUnitSelectionInputs->specificUnitX,pUnitSelectionInputs->specificUnitY)->getContainUnit()->getType();
            Error("ThisType",ThisType);

            //determine the position of the rectangle in units of tiles NOTE: first adding sy(sx) and then subtracting after you have it in units of tiles yields the real eidth in tiles
            sx = pUnitSelectionInputs->SelectSpecificRect.x;
            sy = pUnitSelectionInputs->SelectSpecificRect.y;
            sh = pUnitSelectionInputs->SelectSpecificRect.h + sy;
            sw = pUnitSelectionInputs->SelectSpecificRect.w + sx;
            sx = (sx - (sx % TILE_WIDTH)) / TILE_WIDTH;
            sy = (sy - (sy % TILE_HEIGHT)) / TILE_HEIGHT;
            sh = (sh - (sh % TILE_HEIGHT)) / TILE_HEIGHT;
            sh = sh - sy + 1;
            sw = (sw - (sw % TILE_WIDTH)) / TILE_WIDTH;
            sw = sw - sx + 1;
            //remove a bug that cause a collapse when selection to close to the corner
            if(pScrollInputs->screenX+sx+sw > WORLD_WIDTH){sw = WORLD_WIDTH-sx-pScrollInputs->screenX;}
            if(pScrollInputs->screenY+sy+sh > WORLD_HEIGHT){sh = WORLD_HEIGHT-sy-pScrollInputs->screenY;}

            if(!(pUnitSelectionInputs->onlyAddSelection)) ClearSelection(0);
            //Build a list from all the tiles in the selec
            for(int i=0;i<sw;i++)
            {
                for(int j=0;j<sh;j++)
                {
                    if(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j)->getContainUnit() != NULL)
                    {
                        if(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j)->getContainUnit()->getType() == ThisType)
                        {
                            if(pUnitSelectionInputs->onlyAddSelection) RemoveSelectionByTile(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j),0);
                            AddSelectionByTile(pGameModel->getMap(pScrollInputs->screenX+sx+i,pScrollInputs->screenY+sy+j),thisUser,0);
                        }
                    }
                }
            }
            sortSelection(0);
        }
        pUnitSelectionInputs->selectOnlySpecificUnit = false;
    }
    //Remove the commands if no unit is selected
    if(selectedNumber[0] == 0 && (pUnitSelectionInputs->attackMove || pUnitSelectionInputs->castMove != -1 ))
    {
        pUnitSelectionInputs->attackMove = false;
        pUnitSelectionInputs->castMove = -1;
    }

    //Give Commands TODO multiple possible commands
    if(pUnitSelectionInputs->doMove == true && pUnitSelectionInputs->castMove == -1)
    {
        if(pUnitSelectionInputs->moveCommandX >= WORLD_WIDTH){pUnitSelectionInputs->moveCommandX = WORLD_WIDTH-1;}
        if(pUnitSelectionInputs->moveCommandY >= WORLD_HEIGHT){pUnitSelectionInputs->moveCommandY = WORLD_HEIGHT-1;}
        cSelection* toMove;
        toMove = pListHead[0]->getSuccessor();
        while(toMove != NULL)
        {
            Error("giving movement commands");
            if(toMove->getContent()!=NULL)
            {
                if(toMove->getContent()->getOwner() == thisUser)
                {
                    if(pUnitSelectionInputs->attackMove)
                    {
                        storeCommand(toMove->getContent(),COMMAND_ATTACK,pUnitSelectionInputs->moveCommandX,pUnitSelectionInputs->moveCommandY,1);
                        //toMove->getContent()->AttackCommand(pGameModel->getMap(moveX,moveY),1);
                    }
                    else
                    {
                        storeCommand(toMove->getContent(),COMMAND_MOVE,pUnitSelectionInputs->moveCommandX, pUnitSelectionInputs->moveCommandY,1);
                        //toMove->getContent()->MovementCommand(pGameModel->getMap(moveX,moveY),1);
                    }
                }
            }
            toMove = toMove->getSuccessor();
        }
        pUnitSelectionInputs->doMove = false;
        pUnitSelectionInputs->attackMove = false;
    }

    //apply an actual cast command;
    if(pUnitSelectionInputs->doMove && pUnitSelectionInputs->castMove != -1 && pUnitSelectionInputs->attackMove == false)
    {
        Error("Checking for closest unit");
        if(pUnitSelectionInputs->moveCommandX >= WORLD_WIDTH){pUnitSelectionInputs->moveCommandX = WORLD_WIDTH-1;}
        if(pUnitSelectionInputs->moveCommandY >= WORLD_HEIGHT){pUnitSelectionInputs->moveCommandY = WORLD_HEIGHT-1;}
        float minimalDistance = 1000;
        float sample;
        int ThisType;
        cSelection* toMove;
        cSelection* candidate;
        toMove = NULL;
        candidate = getFirstPriorizedUnit(0);
        if(candidate != NULL)
        {if(candidate->getContent()!=NULL) ThisType = candidate->getContent()->getType(); }
        while(candidate != NULL)
        {
            if(candidate->getContent()!=NULL)
            {
                if(candidate->getContent()->getType() == ThisType)
                {
                    //This function checks if the cast is ready
                    if(checkCastable(candidate->getContent(),pUnitSelectionInputs->castMove))
                    {
                        sample = realDist(pUnitSelectionInputs->moveCommandX,pUnitSelectionInputs->moveCommandY,candidate->getContent()->getPosition()->getX()
                                          ,candidate->getContent()->getPosition()->getY());
                        if(sample < minimalDistance)
                        {
                            Error("found one; distance ",sample);
                            minimalDistance = sample;
                            toMove = candidate;
                        }
                    }
                }
                else {break;}
            }
            candidate = candidate->getSuccessor();
        }
        if(toMove != NULL)
        {
            Error("storing castcommand to ",pUnitSelectionInputs->moveCommandX);
            Error("storing castcommand to ",pUnitSelectionInputs->moveCommandY);
            if(pUnitSelectionInputs->castMove == 0)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_0,pUnitSelectionInputs->moveCommandX, pUnitSelectionInputs->moveCommandY,1);
                pUnitSelectionInputs->castMove = -1;
            }
            if(pUnitSelectionInputs->castMove == 1)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_1,pUnitSelectionInputs->moveCommandX, pUnitSelectionInputs->moveCommandY,1);
                pUnitSelectionInputs->castMove = -1;
            }
            if(pUnitSelectionInputs->castMove == 2)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_2,pUnitSelectionInputs->moveCommandX, pUnitSelectionInputs->moveCommandY,1);
                pUnitSelectionInputs->castMove = -1;
            }
            if(pUnitSelectionInputs->castMove == 3)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_3,pUnitSelectionInputs->moveCommandX, pUnitSelectionInputs->moveCommandY,1);
                pUnitSelectionInputs->castMove = -1;
            }
        }
        pUnitSelectionInputs->doMove = false;
        pUnitSelectionInputs->castMove = -1;
    }

    //give all selected Units a Stop Command
    if(pUnitSelectionInputs->stopMove == true)
    {
        cSelection* toMove;
        toMove = pListHead[0]->getSuccessor();
        while(toMove != NULL)
        {
            Error("tick");
            if(toMove->getContent()!=NULL)
            {
                if(toMove->getContent()->getOwner() == thisUser)
                {
                    storeCommand(toMove->getContent(),COMMAND_STOP,0,0,1);
                    //toMove->getContent()->StopCommand();
                }
            }
            Error("tack");
            toMove = toMove->getSuccessor();
        }
        pUnitSelectionInputs->stopMove = false;
    }

    //Group Settings
    if(pUnitSelectionInputs->GetGroup)
    {
        if(!pUnitSelectionInputs->onlyAddSelection)
        {
            copySelection(pUnitSelectionInputs->GroupNumber,0);
        }
        else
        {
            AddSelectionFromGroup(pUnitSelectionInputs->GroupNumber,0);
            Error("Added selection from Group");
            sortSelection(0);
        }
        pUnitSelectionInputs->GetGroup = false;
    }

    if(pUnitSelectionInputs->SetGroup)
    {
        if(pUnitSelectionInputs->onlyAddSelection == false)
        {
            copySelection(0,pUnitSelectionInputs->GroupNumber);
        }
        else
        {
            AddSelectionFromGroup(0,pUnitSelectionInputs->GroupNumber);
            Error("Added selection to Group");
            sortSelection(pUnitSelectionInputs->GroupNumber);
        }
        pUnitSelectionInputs->SetGroup = false;
    }

    if(pUnitSelectionInputs->JumpGroup)
    {
        if(selectedNumber[pUnitSelectionInputs->GroupNumber] > 0) pScrollInputs->screenJumpRelativeX = CenterMassX(pUnitSelectionInputs->GroupNumber) - SCR_TILE_WIDTH/2 - pScrollInputs->screenX;
        if(selectedNumber[pUnitSelectionInputs->GroupNumber] > 0) pScrollInputs->screenJumpRelativeY = CenterMassY(pUnitSelectionInputs->GroupNumber) - SCR_TILE_HEIGHT/2 -pScrollInputs->screenY;
        pUnitSelectionInputs->JumpGroup = false;
    }

    if(pUnitSelectionInputs->selMiniPic == true)
    {
        Error("selecting MiniPic Number ",pUnitSelectionInputs->selMiniPicNumber);
        if(selectedNumber[0] >= pUnitSelectionInputs->selMiniPicNumber)
        {
            cSelection* sel;
            cUnit* Uni;
            sel = pListHead[0];
            for(int i=0;i< pUnitSelectionInputs->selMiniPicNumber;i++)
            {
                Error("jumping through selection",i);
                sel=sel->getSuccessor();
            }
            Uni = sel->getContent();
            ClearSelection(0);
            AddSelection(Uni,NULL,0);
        }
        else{ClearSelection(0);}
        pUnitSelectionInputs->selMiniPic = false;
    }

    if(pUnitSelectionInputs->changeAbility >= 0)
    {
        Error("activate Command",pUnitSelectionInputs->changeAbility);
        cSelection* toMove;
        int ThisType;
        toMove = NULL;
        toMove = getFirstPriorizedUnit(0);
        if(toMove != NULL)
        {if(toMove->getContent()!=NULL) ThisType = toMove->getContent()->getType(); }
        toMove = pListHead[0]->getSuccessor();
        while(toMove != NULL)
        {
            Error("tick");
            if(toMove->getContent()!=NULL)
            {
                if(toMove->getContent()->getOwner() == thisUser && toMove->getContent()->getType() == ThisType)
                {
                    Error("store activate Command",pUnitSelectionInputs->changeAbility);
                    storeCommand(toMove->getContent(),COMMAND_ACTIVATE_ABILITY,0,0,pUnitSelectionInputs->changeAbility);
                    //toMove->getContent()->StopCommand();
                }
            }
            Error("tack");
            toMove = toMove->getSuccessor();
        }
        pUnitSelectionInputs->changeAbility = -1;
    }

    pView->setSelection(pListHead[0],selectedNumber[0],selectedBuildings[0],selectedTypes[0],priorizedNumber[0]);

    ///TODO adapt to Changelist
    //Move all objects TODO here animations of the world attacks and so on, as each tile is checked
	for(int i=0;i< WORLD_HEIGHT;i++)
	{
	    for(int j=0;j< WORLD_WIDTH;j++)
	    {
	        cUnit* Unit;
            Unit = pGameModel->getMap(j,i)->getContainUnit();
            if(Unit!=NULL)
            {
                //Animate
                Unit->Animate(t);
                //do the movement TODO Collision detection + Pathfinding
                if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
                {
                    Unit->Move(t,&pathFindObj,&changeListObj);
                    //check if it is still alive
                    if(Unit->checkAlive()==false)
                    {
                        Unit->removeUnit(&changeListObj);
                        DestroyUnit(Unit);
                    }
                    int entTyp;
                    for(int k=0;k<Unit->getNumberOfAblilities();k++)
                    {
                        if((entTyp = Unit->checkEntity(k)) >= 0)
                        {
                            Error("preparing create Entity");
                            pGameModel->createEntity(entTyp,Unit->getPosition()->getX(),Unit->getPosition()->getY(),Unit->getGoal(),Unit->getUnitGoal());
                        }
                    }
                }
                else
                {
                    Unit->MoveClient(t);
                }
            }
	    }
	}
	for(int i=0;i< pGameModel->getNumberOfEntities(); i++)
	{
	    if(pGameModel->getEntity(i)->checkActive() == false)
	    {
            pGameModel->removeEntity(i);
	    }
        else
        {
            pGameModel->getEntity(i)->Animate(t);
            if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
            {
                pGameModel->getEntity(i)->Move(t,&changeListObj);
            }
            else
            {
                pGameModel->getEntity(i)->MoveClient(t,&changeListObj);
            }
        }
	}
    if(CONTROL_SETTINGS == MODE_SINGPLAY || CONTROL_SETTINGS == MODE_SERVER)
    {
        if(WRITE_REPLAY == true){writeReplay();}
    }

    // Send/Receive: Do it only if the system is in Server Mode
    if(CONTROL_SETTINGS == MODE_SERVER)
    {
        PackChangeList();
        NetworkManage(t,&netPackToSend ,&netPackToStrip);
        StripReceivedListServer();
    }
    if(CONTROL_SETTINGS == MODE_CLIENT)
    {
        PackCommandList();
        NetworkManage(t,&netPackToSend ,&netPackToStrip);
        StripReceivedListClient();
        if(WRITE_REPLAY == true){writeReplay();}
        applyChanges();
    }
    if(CONTROL_SETTINGS == MODE_REPLAY)
    {
        readReplay();
        applyChanges();
    }
    giveCommands();

    return 1;
}

bool cControl::checkCastable(cUnit* Caster,int slot)
{
    if((Caster->getOwner() == thisUser) && (Caster->checkAbility(slot) == true)) return true;
    return false;
}

void cControl::storeCommand(cUnit* commandedUnit, int CommandType,int unitGoalX, int unitGoalY,int cmdPriority)
{
    Error("storeCommand ", CommandType);
    Command* storeNext;
    storeNext = new Command;
    //The command actually refers to a unit
    if(CommandType < 100)
    {
        //assign the corresponding values
        storeNext->CommandedUnit = commandedUnit;
        //only the following matter for the network transfer
        storeNext->UnitID = commandedUnit->getID();
        storeNext->commandType = CommandType;
        storeNext->posiX = commandedUnit->getPosition()->getX();
        storeNext->posiY = commandedUnit->getPosition()->getY();
        storeNext->goalX = unitGoalX;
        storeNext->goalY = unitGoalY;
        storeNext->priority = cmdPriority;
        storeNext->time = TimeSinceStart;
    }
    else
    {
        //assign the corresponding values
        storeNext->CommandedUnit = NULL;
        //only the following matter for the network transfer
        storeNext->UnitID = 0;
        storeNext->commandType = CommandType;
        storeNext->posiX = 0;
        storeNext->posiY = 0;
        storeNext->goalX = 0;
        storeNext->goalY = 0;
        storeNext->priority = 0;
        storeNext->time = TimeSinceStart;
    }

    //Add the Command to the storage List (from here commands are sent to the server if necessary)
    commandListObj.Add(storeNext);
}

void cControl::applyChanges()
{
    Change* giveNext;
    while((giveNext = changeListObj.getFirst())!= NULL)
    {
        Error("applying change ",giveNext->changeType);
        switch(giveNext->changeType)
        {
            case CHANGE_NEW :
            {
                pGameModel->CreateUnit(giveNext->UnitID,giveNext->goalUnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
                break;
            }
            case CHANGE_REMOVE :
            {
                cUnit* Unit;
                Unit = giveNext->CommandedUnit;
                DestroyUnit(Unit);
                break;
            }
            case CHANGE_POSITION :
            {
                giveNext->CommandedUnit->applyPositionChange(pGameModel->getMap(giveNext->goalX,giveNext->goalY),&changeListObj);
                break;
            }
            case CHANGE_GOAL :
            {
                giveNext->CommandedUnit->setGoal(pGameModel->getMap(giveNext->goalX,giveNext->goalY));
                break;
            }
            case CHANGE_UNITGOAL :
            {
                Error("changing Unitgoal to X ", giveNext->goalX);
                Error("changing Unitgoal to Y ", giveNext->goalY);
                if((giveNext->goalX > WORLD_WIDTH) || (giveNext->goalY > WORLD_HEIGHT))
                {
                    giveNext->CommandedUnit->setUnitGoal(NULL);
                }
                else
                {
                    giveNext->CommandedUnit->setUnitGoal(pGameModel->getMap(giveNext->goalX,giveNext->goalY)->getContainUnit());
                }
                break;
                Error("done");
            }
            case CHANGE_MOVETYPE :
            {
                giveNext->CommandedUnit->setActionType(giveNext->moveType);
                break;
            }
            case CHANGE_HEALTH :
            {
                Error("apllying health change : ",giveNext->health);
                giveNext->CommandedUnit->setHealth(giveNext->health);
                break;
            }
            case CHANGE_CREATE_ENT :
            {
                Error("Create Entity Control");
                if(pGameModel->getMap(giveNext->goalX,giveNext->goalY)->getContainUnit() != NULL)
                {
                    pGameModel->createEntity(giveNext->CommandedUnit->getEnttype(giveNext->moveType),giveNext->posiX,giveNext->posiY
                                ,pGameModel->getMap(giveNext->goalX,giveNext->goalY)
                                ,pGameModel->getMap(giveNext->goalX,giveNext->goalY)->getContainUnit());
                }
                else
                {
                    pGameModel->createEntity(giveNext->CommandedUnit->getEnttype(giveNext->moveType),giveNext->posiX,giveNext->posiY
                             ,pGameModel->getMap(giveNext->goalX,giveNext->goalY),NULL);
                }
                break;
            }
            case CHANGE_MANA:
            {
                Error("apllying mana change : ",giveNext->health);
                giveNext->CommandedUnit->setMana(giveNext->health);
                break;
            }
            case CHANGE_ACTIVE_ABILITY:
            {
                giveNext->CommandedUnit->setActiveAbility(giveNext->moveType);
                break;
            }
            case CHANGE_CAST_ONCE:
            {
                if(giveNext->moveType == 100000) {giveNext->CommandedUnit->setCastOnce(-1);}
                else {giveNext->CommandedUnit->setCastOnce(giveNext->moveType);}

                break;
            }
        }
        delete giveNext;
    }
}
void cControl::giveCommands()
{
    bool Ground = false;
    bool Self = false;
    Command* giveNext;
    int ID;
    while((giveNext = commandListObj.getFirst())!= NULL)
    {
        Ground = false;
        Self = false;
        Error("Command type ",giveNext->commandType);
        switch(giveNext->commandType)
        {
            case COMMAND_CLIENT_READY:
            {
                bStartSignal = true;
                break;
            }
            case COMMAND_INIT_SPAWN:
            {
                Error("trying to spawn");
                //DO A SEND OF WHAT HE DID
                ID = pGameModel->checkFreeUnitID();
                Error("ID",ID);
                if(ID != -1)
                {
                    Change* newChange;
                    pGameModel->CreateUnit(ID,giveNext->UnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
                    newChange = new Change;
                    newChange->UnitID = ID;
                    newChange->posiX = giveNext->posiX;
                    newChange->posiY = giveNext->posiY;
                    newChange->goalX = giveNext->goalX;
                    newChange->goalY = 0;
                    newChange->goalUnitID = giveNext->UnitID;
                    newChange->time = 0;
                    newChange->changeType = CHANGE_NEW;

                    changeListObj.Add(newChange);
                }
                break;
            }

            case COMMAND_TERMINATE_GAME:
            {
                //End the game (TODO decide wheter it was the other client or oneself)
                bRun = false;
                break;
            }
            case COMMAND_STOP:
            {
                giveNext->CommandedUnit->StopCommand();
                break;
            }
            case COMMAND_HOLDPOSI:
            {
                //TODO implement HoldPosition
                break;
            }
            case COMMAND_MOVE:
            {
                giveNext->CommandedUnit->MovementCommand(pGameModel->getMap(giveNext->goalX,giveNext->goalY),giveNext->priority);
                break;
            }
            case COMMAND_ATTACK:
            {
                giveNext->CommandedUnit->AttackCommand(pGameModel->getMap(giveNext->goalX,giveNext->goalY),giveNext->priority);
                break;
            }
            case COMMAND_ACTIVATE_ABILITY:
            {
                sUnitSpecifications* pUnitSpecifications = pGameModel->getUnitSpecifications();
                sEntitySpecifications* pEntitySpecifications = pGameModel->getEntitySpecifications();
                if(pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority] >= 0 &&
                   pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority] < ENTITY_TYPE_MAX )
                   {
                       if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_GROUND) Ground = true;
                       if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                   }

                giveNext->CommandedUnit->ChangeAbilityTypeCommand(giveNext->priority,Ground,Self,&changeListObj);
                break;
            }
            case COMMAND_CAST_0:
            {
                sUnitSpecifications* pUnitSpecifications = pGameModel->getUnitSpecifications();
                sEntitySpecifications* pEntitySpecifications = pGameModel->getEntitySpecifications();
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][0]][0] >= ENT_PROP_GROUND) Ground = true;
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(0,pGameModel->getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_1:
            {
                sUnitSpecifications* pUnitSpecifications = pGameModel->getUnitSpecifications();
                sEntitySpecifications* pEntitySpecifications = pGameModel->getEntitySpecifications();
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][1]][0] >= ENT_PROP_GROUND) Ground = true;
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                Error("giving cast command 1");
                giveNext->CommandedUnit->CastCommand(1,pGameModel->getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_2:
            {
                sUnitSpecifications* pUnitSpecifications = pGameModel->getUnitSpecifications();
                sEntitySpecifications* pEntitySpecifications = pGameModel->getEntitySpecifications();
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][2]][0] >= ENT_PROP_GROUND) Ground = true;
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(2,pGameModel->getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_3:
            {
                sUnitSpecifications* pUnitSpecifications = pGameModel->getUnitSpecifications();
                sEntitySpecifications* pEntitySpecifications = pGameModel->getEntitySpecifications();
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][3]][0] >= ENT_PROP_GROUND) Ground = true;
                if(pEntitySpecifications->modifierType[pUnitSpecifications->UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(3,pGameModel->getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
        }
        //Clear the Command, as it is no longer on the list
        delete giveNext;
    }
}



void cControl::PackCommandList()
{
    NetPackage* sendNext;
    Command* packNext;
    char newPackCore[NET_CORESIZE];
    char* buf4;

    //get Items from the commandList until there are none left
    while((packNext = commandListObj.getFirst()) != NULL)
    {
        sendNext = new NetPackage;
        sendNext->owner = thisUser;
        sendNext->packID = createNewPackID();
        sendNext->resendtime = 0;

        //first 4 bytes the command type
        buf4 = intToChar4(packNext->commandType);
        for(int i=0;i<4;i++){newPackCore[i] = buf4[i];}
        delete buf4;
        buf4 = NULL;

        //bytes 5-8 by UnitID
        buf4 = intToChar4(packNext->UnitID);
        for(int i=4;i<8;i++){newPackCore[i] = buf4[i-4];}
        delete buf4;
        buf4 = NULL;

        //bytes 9-16 the time
        buf4 = intToChar4( packNext->time);
        for(int i=8;i<12;i++){newPackCore[i] = buf4[i-8];}
        delete buf4;
        buf4 = NULL;

        //bytes 17-20 & 21-24 by X and Y of the position
        buf4 = intToChar4( packNext->posiX);
        for(int i=12;i<16;i++){newPackCore[i] = buf4[i-12];}
        delete buf4;
        buf4 = NULL;
        buf4 = intToChar4( packNext->posiY);
        for(int i=16;i<20;i++){newPackCore[i] = buf4[i-16];}
        delete buf4;
        buf4 = NULL;

        //bytes 25-28 & 29-32 by X and Y of the goal (if present)
        buf4 = intToChar4(packNext->goalX);
        for(int i=20;i<24;i++){newPackCore[i] = buf4[i-20];}
        delete buf4;
        buf4 = NULL;
        buf4 = intToChar4(packNext->goalY);
        for(int i=24;i<28;i++){newPackCore[i] = buf4[i-24];}
        delete buf4;
        buf4 = NULL;

        //bytes 33-36 by the priority
        buf4 = intToChar4( packNext->priority);
        for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
        delete buf4;
        buf4 = NULL;

        //The rest is filled with zero's
        for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}

        //assign the saved values
        for(int i=0;i<NET_CORESIZE;i++){sendNext->packageCore[i] = newPackCore[i];}
        //and add it to the sending list
        netPackToSend.Add(sendNext);

        delete packNext;
    }
}

void cControl::PackChangeList()
{
    NetPackage* sendNext;
    Change* packNext;
    char newPackCore[NET_CORESIZE];
    char* buf4;
    //cChangeList bufferList;

    ///TODO for the server these changes need to be copied!
    //get Items from the changeList until there are none left
    while((packNext = changeListObj.getFirst()) != NULL)
    {
        //bufferList.Add(packNext);
        sendNext = new NetPackage;
        sendNext->owner = thisUser;
        sendNext->packID = createNewPackID();
        sendNext->resendtime = 0;

        //first 4 bytes the change type
        buf4 = intToChar4( packNext->changeType);
        for(int i=0;i<4;i++){newPackCore[i] = buf4[i];}
        delete buf4;
        buf4 = NULL;

        //bytes 5-8 by UnitID
        buf4 = intToChar4( packNext->UnitID);
        for(int i=4;i<8;i++){newPackCore[i] = buf4[i-4];}
        delete buf4;
        buf4 = NULL;

        //bytes 9-16 the time
        buf4 = intToChar4( packNext->time);
        for(int i=8;i<12;i++){newPackCore[i] = buf4[i-8];}
        delete buf4;
        buf4 = NULL;

        //bytes 17-20 & 21-24 by X and Y of the position
        buf4 = intToChar4( packNext->posiX);
        for(int i=12;i<16;i++){newPackCore[i] = buf4[i-12];}
        delete buf4;
        buf4 = NULL;
        buf4 = intToChar4( packNext->posiY);
        for(int i=16;i<20;i++){newPackCore[i] = buf4[i-16];}
        delete buf4;
        buf4 = NULL;

        //bytes 25-28 & 29-32 by X and Y of the goal (if present)
        buf4 = intToChar4( packNext->goalX);
        for(int i=20;i<24;i++){newPackCore[i] = buf4[i-20];}
        delete buf4;
        buf4 = NULL;
        buf4 = intToChar4( packNext->goalY);
        for(int i=24;i<28;i++){newPackCore[i] = buf4[i-24];}
        delete buf4;
        buf4 = NULL;

        switch(packNext->changeType)
        {
            //There is no additional information needed
            case CHANGE_NEW :
            {
                Error("Adding new Unit");
                //store the Unittype in goalUnitID
                buf4 = intToChar4( packNext->goalUnitID);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_REMOVE : {}
            case CHANGE_POSITION : {}
            case CHANGE_GOAL :
            {
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_UNITGOAL :
            {
                //assign a UnitGoalID, to check for consistency
                buf4 = intToChar4( packNext->goalUnitID);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_MOVETYPE :
            {
                //assign a UnitGoalID, to check for consistency
                buf4 = intToChar4( packNext->moveType);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_MANA: {}
            case CHANGE_HEALTH :
            {
                //assign a UnitGoalID, to check for consistency
                buf4 = intToChar4( packNext->health);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_CREATE_ENT:
            {
                //assign a UnitGoalID, to check for consistency
                buf4 = intToChar4( packNext->moveType);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_ACTIVE_ABILITY:
            {
                //assign the number of active ability
                buf4 = intToChar4( packNext->moveType);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            case CHANGE_CAST_ONCE:
            {
                //assign the number of cast once
                buf4 = intToChar4( packNext->moveType);
                for(int i=28;i<32;i++){newPackCore[i] = buf4[i-28];}
                delete buf4;
                buf4 = NULL;
                //The rest is filled with zero's
                for(int i=32;i<NET_CORESIZE;i++){newPackCore[i] = 0;}
                break;
            }
            default:
            {
                //NetError: wrong sort of package
                Error("wrong Type of Package");
                break;
            }
        }

        //assign the saved values
        for(int i=0;i<NET_CORESIZE;i++){sendNext->packageCore[i] = newPackCore[i];}
        //and add it to the sending list
        netPackToSend.Add(sendNext);

        delete packNext;
    }
}

void cControl::StripReceivedListServer()
{
    NetPackage* stripNext;
    Command* newCommand;

    char newPackCore[NET_CORESIZE];
    char buf4[4];
    //Get a new package
    while((stripNext = netPackToStrip.getFirst())!=NULL)
    {
        Error("stripping a command package");
        newCommand = new Command;

        for(int i=0;i<NET_CORESIZE;i++){newPackCore[i] = stripNext->packageCore[i];}

        //As this is the server version the package should be a command package
        for(int i=0;i<4;i++){buf4[i] = newPackCore[i];}
        newCommand->commandType = Char4ToInt(buf4);

        //bytes 5-8 by UnitID
        for(int i=4;i<8;i++){buf4[i-4] = newPackCore[i];}
        newCommand->UnitID = Char4ToInt(buf4);

        //bytes 9-16 the time
        for(int i=8;i<12;i++){buf4[i-8] = newPackCore[i];}
        newCommand->time = Char4ToInt(buf4);

        //bytes 17-20 & 21-24 by X and Y of the position
        for(int i=12;i<16;i++){buf4[i-12] = newPackCore[i];}
        newCommand->posiX = Char4ToInt(buf4);
        for(int i=16;i<20;i++){buf4[i-16] = newPackCore[i];}
        newCommand->posiY = Char4ToInt(buf4);

        //bytes 25-28 & 29-32 by X and Y of the goal (if present)
        for(int i=20;i<24;i++){buf4[i-20] = newPackCore[i];}
        newCommand->goalX = Char4ToInt(buf4);
        for(int i=24;i<28;i++){buf4[i-24] = newPackCore[i];}
        newCommand->goalY = Char4ToInt(buf4);

        //bytes 33-36 by the priority
        for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
        newCommand->priority = Char4ToInt(buf4);

        //If we have an actual command
        if(newCommand->commandType < 100)
        {
            //Check if the Unit with the corresponding ID has these coordinates
            if(pGameModel->getMap(newCommand->posiX,newCommand->posiY)->getContainUnit() != NULL)
            {
                if((pGameModel->getMap(newCommand->posiX,newCommand->posiY)->getContainUnit()->getID()) == newCommand->UnitID)
                {
                        newCommand->CommandedUnit = pGameModel->getMap(newCommand->posiX,newCommand->posiY)->getContainUnit();
                }
                else
                {
                    newCommand->CommandedUnit = pGameModel->getUnitByID(newCommand->UnitID);
                }
            }
            else
            {
                newCommand->CommandedUnit = pGameModel->getUnitByID(newCommand->UnitID);
            }

            //Add the Command to the Commandlist
            if(newCommand->CommandedUnit != NULL)
            {
                commandListObj.Add(newCommand);
            }
        }
        //if we have a system command
        else
        {
            commandListObj.Add(newCommand);
        }


        delete stripNext;
    }

}

void cControl::StripReceivedListClient()
{
    NetPackage* stripNext;
    Change* newChange;
    char newPackCore[NET_CORESIZE];
    char buf4[4];

    while((stripNext = netPackToStrip.getFirst()) != NULL)
    {
        newChange = new Change;

        for(int i=0;i<NET_CORESIZE;i++){newPackCore[i] = stripNext->packageCore[i];}

        //first 4 bytes the change type
        for(int i=0;i<4;i++){buf4[i] = newPackCore[i];}
        newChange->changeType = Char4ToInt(buf4);

        //bytes 5-8 by UnitID
        for(int i=4;i<8;i++){buf4[i-4] = newPackCore[i];}
        newChange->UnitID = Char4ToInt(buf4);

        //bytes 9-16 the time
        for(int i=8;i<12;i++){buf4[i-8] = newPackCore[i];}
        newChange->time = Char4ToInt(buf4);

        //bytes 17-20 & 21-24 by X and Y of the position
        for(int i=12;i<16;i++){buf4[i-12] = newPackCore[i];}
        newChange->posiX = Char4ToInt(buf4);
        for(int i=16;i<20;i++){buf4[i-16] = newPackCore[i];}
        newChange->posiY = Char4ToInt(buf4);

        //bytes 25-28 & 29-32 by X and Y of the goal (if present)
        for(int i=20;i<24;i++){buf4[i-20] = newPackCore[i];}
        newChange->goalX = Char4ToInt(buf4);
        for(int i=24;i<28;i++){buf4[i-24] = newPackCore[i];}
        newChange->goalY = Char4ToInt(buf4);

        switch(newChange->changeType)
        {
            //There is no additional information needed
            case CHANGE_NEW :
            {
                //store the Unittype in goalUnitID
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->goalUnitID = Char4ToInt(buf4);
                //the first Unit will change this signal; this means the client knows that the game has begun
                bStartSignal = true;
                break;
            }
            case CHANGE_REMOVE : {}
            case CHANGE_POSITION : {}
            case CHANGE_GOAL :
            {
                break;
            }
            case CHANGE_UNITGOAL :
            {
                //assign a UnitGoalID, to check for consistency
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->goalUnitID = Char4ToInt(buf4);
                break;
            }
            case CHANGE_MOVETYPE :
            {
                //assign a UnitGoalID, to check for consistency
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->moveType = Char4ToInt(buf4);
                break;
            }
            case CHANGE_MANA : {}
            case CHANGE_HEALTH :
            {
                //assign a UnitGoalID, to check for consistency
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->health = Char4ToInt(buf4);
                break;
            }
            case CHANGE_CREATE_ENT:
            {
                //assign a UnitGoalID, to check for consistency
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->moveType = Char4ToInt(buf4);
                break;
            }
            case CHANGE_CAST_ONCE: {}
            case CHANGE_ACTIVE_ABILITY:
            {
                for(int i=28;i<32;i++){buf4[i-28] = newPackCore[i];}
                newChange->moveType = Char4ToInt(buf4);
                break;
            }
            default:
            {
                //NetError: wrong sort of package
                Error("wrong Type of Package");
                break;
            }
        }

        //Check if the Unit with the corresponding ID has these coordinates
        if(pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit() != NULL && newChange->changeType != CHANGE_NEW)
        {
            if((pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit()->getID()) == newChange->UnitID)
            {
                    newChange->CommandedUnit = pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit();
            }
            else
            {
                newChange->CommandedUnit = pGameModel->getUnitByID(newChange->UnitID);
            }
        }
        else
        {
            newChange->CommandedUnit = pGameModel->getUnitByID(newChange->UnitID);
        }

        changeListObj.Add(newChange);

        delete stripNext;
    }
}

unsigned short int cControl::createNewPackID()
{
    lastPackID++;
    Error("PackID",lastPackID);
    return lastPackID;
}

void cControl::writeReplay()
{
    cChangeList* buffer;
    Change* storeNext;
    buffer = new cChangeList;
    while((storeNext = changeListObj.getFirst()) != NULL)
    {
        ReplayObj.storeChange(storeNext,TimeSinceStart);
        if(CONTROL_SETTINGS == MODE_CLIENT || CONTROL_SETTINGS == MODE_SERVER){buffer->Add(storeNext);}
    }
    while((storeNext = buffer->getFirst()) != NULL)
    {
        changeListObj.Add(storeNext);
    }
    delete buffer;
}

//this function reads the next command in the pipe from the replay file
void cControl::readReplay()
{
    Change* newChange;
    //newChange = new Change;
    newChange = ReplayObj.readChange(TimeSinceStart);
    while(newChange != NULL)
    {
        //Check if the Unit with the corresponding ID has these coordinates
        if(pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit() != NULL && newChange->changeType != CHANGE_NEW)
        {
            if((pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit()->getID()) == newChange->UnitID)
            {
                    newChange->CommandedUnit = pGameModel->getMap(newChange->posiX,newChange->posiY)->getContainUnit();
            }
            else
            {
                newChange->CommandedUnit = pGameModel->getUnitByID(newChange->UnitID);
            }
        }
        else
        {
            newChange->CommandedUnit = pGameModel->getUnitByID(newChange->UnitID);
        }
        changeListObj.Add(newChange);
        newChange = ReplayObj.readChange(TimeSinceStart);
    }
}

void cControl::DestroyUnit(cUnit* Unit)
{
    pGameModel->DestroyUnit(Unit);
    if(Unit->getOwner() == thisUser)
    {
        for(int i=0;i<=10;i++)
        {
            RemoveSelection(Unit,NULL,i);
        }
    }
}

int cControl::getUser()
{
    return thisUser;
}

bool cControl::checkRunning()
{
    return bRun;
}

void cControl::setRun(bool run)
{
    bRun = run;
}

bool cControl::CleanUpControl()
{
    return 1;
}
