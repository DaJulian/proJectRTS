#include "cControl.h"

bool cControl::InitControl()
{
    TimeSinceStart = 0;
    lastPackID = 0;

    //tell program to run
    bRun = true;
    bStartSignal = false;
    drawAoe =-1;

    //TODO READ FILES FOR SPEZIFICATIONS
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

    //Unit Settings; meaning the total number of objects existing in the game
    f.open(UNIT_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);
    f >> garbage >> TILE_TYPE_NUMBER
      >> garbage >> UNIT_TYPE_NUMBER
      >> garbage >> BUILDING_TYPE_NUMBER
      >> garbage >> OBSTACLE_TYPE_NUMBER
      >> garbage >> ENTITY_TYPE_NUMBER;

    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        f >> garbage >> UNIT_SPEC_FILE[i];
    }
    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f >> garbage >> BUILDING_SPEC_FILE[i];
    }
    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f >> garbage >> ENTITY_SPEC_FILE[i];
    }

    Error("TILE_TYPE_NUMBER ",TILE_TYPE_NUMBER);
    Error("UNIT_TYPE_NUMBER ",UNIT_TYPE_NUMBER);
    Error("OBSTACLE_TYPE_NUMBER ",OBSTACLE_TYPE_NUMBER);
    Error("ENTITIY_TYPE_NUMBER ",ENTITY_TYPE_NUMBER);

    f.close();

    //Initialize the unit settings
    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        f.open(UNIT_SPEC_FILE[i].c_str());
        f >> garbage >> garbage >> garbage >> garbage >> garbage;
        f >> garbage >> garbage >> UnitMovementSpeed[i];
        f >> garbage >> garbage >> health[i];
        f >> garbage >> garbage >> mana[i];
        f >> garbage >> garbage >> healthreg[i];
        f >> garbage >> garbage >> manareg[i];
        f >> garbage >> garbage >> damage[i];
        f >> garbage >> garbage >> sightRange[i];
        f >> garbage >> garbage >> Armor[i];
        f >> garbage >> garbage >> Magicarmor[i];
        f >> garbage >> garbage >> tauntRange[i];
        f >> garbage >> garbage >> sortPriority[i];
        f >> garbage >> garbage >> UnitCost[i];
        f >> garbage >> garbage >> BuildingTime[i];
        f >> garbage >> NumberOfAblilites[i];

        Error("Movement speed ",UnitMovementSpeed[i]);
        Error("health  ",health[i]);
        Error("mana",mana[i]);
        Error("healthreg",healthreg[i]);
        Error("manareg",manareg[i]);
        Error("damage",damage[i]);
        Error("sightRange",sightRange[i]);
        Error("Armor",Armor[i]);
        Error("Magicarmor",Magicarmor[i]);
        Error("tauntRange",tauntRange[i]);
        Error("NumberOfAblilites",NumberOfAblilites[i]);

        for(int j=0;j<NumberOfAblilites[i];j++)
        {
            f >> garbage >> garbage;
            f >> garbage >> garbage >> attackSpeed[i][j];
            f >> garbage >> garbage >> attackRange[i][j];
            f >> garbage >> garbage >> UnitEntityType[i][j];
            f >> garbage >> garbage >> ManaCost[i][j];
            f >> garbage;

        Error("attackSpeed",attackSpeed[i][j]);
        Error("attackRange",attackRange[i][j]);
        Error("ManaCost",ManaCost[i][j]);
        Error("UnitEntityType",UnitEntityType[i][j]);
        }

        f >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage >> garbage ;

        for(int j=0;j<NMBR_ACTIONS;j++)
        {
            f >> garbage;
            f >> garbage >> UnitAnimSteps[i][j];
            f >> garbage >> UnitAnimDuration[i][j];
            Error("Command ID j",j);
            Error("UnitAnimDuration[i][j]",UnitAnimDuration[i][j]);
        }
        f.close();
    }

    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f.open(BUILDING_SPEC_FILE[i].c_str());
        f >> garbage;
        f >> BuildingSizeX[i] >> BuildingSizeY[i];
        f >> garbage;
        for(int j=0;j<BUILDING_MAX_WIDTH;j++)
        {
            for(int k=0;k<BUILDING_MAX_HEIGHT;k++)
            {
                f >> BuildingLayoutStorgage[i][k][j];
            }
        }
        ///TODO READ STUFF
        f.close();
    }

    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f.open(ENTITY_SPEC_FILE[i].c_str());
        f >> garbage >> garbage >> garbage >> garbage >> garbage;
        f >> garbage >> garbage >> EntitynumModifiers[i];
        f >> garbage >> garbage >> AnimNumberOfSteps[i];
        f >> garbage >> garbage >> AnimDuration[i];
        f >> garbage >> garbage >> lifeTime[i];
        f >> garbage >> garbage >> startaway[i];

        Error(" ");
        Error(" Entity",i);
        Error("EntitynumModifiers[i]",EntitynumModifiers[i]);
        Error("AnimNumberOfSteps[i]",AnimNumberOfSteps[i]);
        Error("AnimDuration[i]",AnimDuration[i]);
        //Error("lifeTime[i]",lifeTime[i]);
        //Error("startaway[i]",startaway[i]);

        for(int j=0;j<EntitynumModifiers[i];j++)
        {
            f >> garbage >> garbage >> modifierType[i][j];
            f >> garbage >> garbage >> modifierValue[i][j];

            Error(" ");
            Error(" modifier",j);
            Error("modifierType[i][j]",modifierType[i][j]);
            Error("modifierValue[i][j]",modifierValue[i][j]);

        }
        f.close();
    }

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

    //safely assign initial values to the entity list
    NumberOfEntities = 0;
    for(int i=0;i< MAX_ENT;i++)
    {
        EntityList[i] = NULL;
    }
    screenJumpRelativeX = 0;
    screenJumpRelativeY = 0;

    //Initialize the pathFinder TODO more things then just map
    pathFindObj.Init();
    pathFindObj.setMap(&MapObj);

    return 1;
}

void cControl::MapInit()
{
    //Initialize the map (TODO READ FILE)
    MapObj.InitMap(MAP_SETTINGS_FILE,&changeListObj,&MapObj);
}

void cControl::LoadStandardOpponent()
{

}

void cControl::InitUnits(int User,int InitSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX])
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
                    if(User ==1)
                    {
                        newChange->posiX = PLAYER_1_START_X+i;
                        newChange->posiY = PLAYER_1_START_Y+j;

                    }
                    if(User ==2)
                    {
                        newChange->posiX = PLAYER_2_START_X+i;
                        newChange->posiY = PLAYER_2_START_Y+j;

                    }

                    newChange->goalX = User;
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
                    if(User ==1)
                    {
                        newCommand->posiX = PLAYER_1_START_X+i;
                        newCommand->posiY = PLAYER_1_START_Y+j;

                    }
                    if(User ==2)
                    {
                        newCommand->posiX = PLAYER_2_START_X+i;
                        newCommand->posiY = PLAYER_2_START_Y+j;

                    }
                    newCommand->UnitID = InitSector[i][j];
                    newCommand->goalX = User;
                    newCommand->goalY = 0;
                    newCommand->priority = 0;
                    newCommand->time = 0;
                    commandListObj.Add(newCommand);
                }
            }
        }
    }
    newestID = ID;
    if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
    {
        cChangeList bufferList;
        Change* giveNext;
        while((giveNext = changeListObj.getFirst())!=NULL)
        {
            CreateUnit(giveNext->UnitID,giveNext->goalUnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
            bufferList.Add(giveNext);
        }
        while((giveNext = bufferList.getFirst())!=NULL)
        {
            changeListObj.Add(giveNext);
        }
    }

    ///TODO just a test
    CreateBuilding(0,6,6,1);

}

bool cControl::DoControl(Uint32 t,int scrX,int scrY,bool* TermGame,bool* SelectTile, bool* SelectMiniPic, int PicNumber,
                         SDL_Rect* SelectRect,bool* selectSpecificUnit, SDL_Rect* SpecificUnitRect,int specX,int specY,bool* doMove,bool* doAttack,bool* doStop,int moveX,int moveY,
                         bool* getGroup,bool* setGroup,bool* jumpGroup, int GroupNumber, bool* ShiftDown,bool *doCycleSel, int* activateCommand, int* doCast)
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

    if(*doCycleSel == true)
    {
        cyclePriorization(0);
        *doCycleSel = false;
    }

    //Check if the new cast command can actually be cast right now; if not undo the command
    if(*doCast != -1 && checkNewSpell == true)
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
                if(checkCastable(toMove->getContent(),*doCast))
                {
                    checkDone = true;
                    break;
                }
            }
            toMove = toMove->getSuccessor();
        }
        checkNewSpell = false;
        if(checkDone == false) *doCast = -1;
    }
    //reset the checking variable
    if(*doCast == -1) checkNewSpell = true;

    //tell aoe Draw range
    drawAoe = -1;
    if(selectedNumber[0] > 0 && *doCast != -1)
    {
        cSelection* toMove;
        toMove = getFirstPriorizedUnit(0);
        if(toMove->getContent()->getNumberOfAblilities() > *doCast)
        {
            if(modifierType[toMove->getContent()->getEnttype(*doCast)][0] >= ENT_PROP_AOE)
            {
                drawAoe = modifierValue[toMove->getContent()->getEnttype(*doCast)][0];
            }
        }
    }

    //Do a selection
    if(*SelectTile == true)
    {
        //Working variables Rectangle sizes in units of tiles
        int sx,sy,sh,sw;

        //determine the position of the rectangle in units of tiles NOTE: first adding sy(sx) and then subtracting after you have it in units of tiles yields the real eidth in tiles
        sx = SelectRect->x;
        sy = SelectRect->y;
        sh = SelectRect->h + sy;
        sw = SelectRect->w + sx;
        sx = (sx - (sx % TILE_WIDTH)) / TILE_WIDTH;
        sy = (sy - (sy % TILE_HEIGHT)) / TILE_HEIGHT;
        sh = (sh - (sh % TILE_HEIGHT)) / TILE_HEIGHT;
        sh = sh - sy + 1;
        sw = (sw - (sw % TILE_WIDTH)) / TILE_WIDTH;
        sw = sw - sx + 1;
        //remove a bug that cause a collapse when selection to close to the corner
        if(scrX+sx+sw > WORLD_WIDTH){sw = WORLD_WIDTH-sx-scrX;}
        if(scrY+sy+sh > WORLD_HEIGHT){sh = WORLD_HEIGHT-sy-scrY;}

        if(*ShiftDown == false) ClearSelection(0);
        //Build a list from all the tiles in the selec
        for(int i=0;i<sw;i++)
        {
            for(int j=0;j<sh;j++)
            {
                if(*ShiftDown == true) RemoveSelectionByTile(MapObj.getMap(scrX+sx+i,scrY+sy+j),0);
                AddSelectionByTile(MapObj.getMap(scrX+sx+i,scrY+sy+j),thisUser,0);
            }
        }
        sortSelection(0);

        //Mark selection as done
        *SelectTile = false;
    }

    if(*selectSpecificUnit == true)
    {
        Error("specific selectX", specX);
        Error("specific selectY", specY);
        if(MapObj.getMap(specX,specY)->getContainUnit() != NULL)
        {
            int sx,sy,sh,sw;
            int ThisType = MapObj.getMap(specX,specY)->getContainUnit()->getType();
            Error("ThisType",ThisType);

            //determine the position of the rectangle in units of tiles NOTE: first adding sy(sx) and then subtracting after you have it in units of tiles yields the real eidth in tiles
            sx = SpecificUnitRect->x;
            sy = SpecificUnitRect->y;
            sh = SpecificUnitRect->h + sy;
            sw = SpecificUnitRect->w + sx;
            sx = (sx - (sx % TILE_WIDTH)) / TILE_WIDTH;
            sy = (sy - (sy % TILE_HEIGHT)) / TILE_HEIGHT;
            sh = (sh - (sh % TILE_HEIGHT)) / TILE_HEIGHT;
            sh = sh - sy + 1;
            sw = (sw - (sw % TILE_WIDTH)) / TILE_WIDTH;
            sw = sw - sx + 1;
            //remove a bug that cause a collapse when selection to close to the corner
            if(scrX+sx+sw > WORLD_WIDTH){sw = WORLD_WIDTH-sx-scrX;}
            if(scrY+sy+sh > WORLD_HEIGHT){sh = WORLD_HEIGHT-sy-scrY;}

            if(*ShiftDown == false) ClearSelection(0);
            //Build a list from all the tiles in the selec
            for(int i=0;i<sw;i++)
            {
                for(int j=0;j<sh;j++)
                {
                    if(MapObj.getMap(scrX+sx+i,scrY+sy+j)->getContainUnit() != NULL)
                    {
                        if(MapObj.getMap(scrX+sx+i,scrY+sy+j)->getContainUnit()->getType() == ThisType)
                        {
                            if(*ShiftDown == true) RemoveSelectionByTile(MapObj.getMap(scrX+sx+i,scrY+sy+j),0);
                            AddSelectionByTile(MapObj.getMap(scrX+sx+i,scrY+sy+j),thisUser,0);
                        }
                    }
                }
            }
            sortSelection(0);
        }
        *selectSpecificUnit = false;
    }

    //Remove the commands if no unit is selected
    if(selectedNumber[0] == 0 && (*doAttack == true || *doCast != -1 ))
    {
        *doAttack = false;
        *doCast = -1;
    }

    //Give Commands TODO multiple possible commands
    if(*doMove == true && *doCast == -1)
    {
        if(moveX >= WORLD_WIDTH){moveX = WORLD_WIDTH-1;}
        if(moveY >= WORLD_HEIGHT){moveY = WORLD_HEIGHT-1;}
        cSelection* toMove;
        toMove = pListHead[0]->getSuccessor();
        while(toMove != NULL)
        {
            Error("giving movement commands");
            if(toMove->getContent()!=NULL)
            {
                if(toMove->getContent()->getOwner() == thisUser)
                {
                    if(*doAttack == true)
                    {
                        storeCommand(toMove->getContent(),COMMAND_ATTACK,moveX, moveY,1);
                        //toMove->getContent()->AttackCommand(MapObj.getMap(moveX,moveY),1);
                    }
                    else
                    {
                        storeCommand(toMove->getContent(),COMMAND_MOVE,moveX, moveY,1);
                        //toMove->getContent()->MovementCommand(MapObj.getMap(moveX,moveY),1);
                    }
                }
            }
            toMove = toMove->getSuccessor();
        }
        *doMove = false;
        *doAttack = false;
    }

    //apply an actual cast command;
    if(*doMove == true && *doCast != -1 && *doAttack == false)
    {
        Error("Checking for closest unit");
        if(moveX >= WORLD_WIDTH){moveX = WORLD_WIDTH-1;}
        if(moveY >= WORLD_HEIGHT){moveY = WORLD_HEIGHT-1;}
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
                    if(checkCastable(candidate->getContent(),*doCast))
                    {
                        sample = realDist(moveX,moveY,candidate->getContent()->getPosition()->getX()
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
            Error("storing castcommand to ",moveX);
            Error("storing castcommand to ",moveY);
            if(*doCast == 0)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_0,moveX, moveY,1);
                *doCast = -1;
            }
            if(*doCast == 1)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_1,moveX, moveY,1);
                *doCast = -1;
            }
            if(*doCast == 2)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_2,moveX, moveY,1);
                *doCast = -1;
            }
            if(*doCast == 3)
            {
                storeCommand(toMove->getContent(),COMMAND_CAST_3,moveX, moveY,1);
                *doCast = -1;
            }
        }
        *doMove = false;
        *doCast = -1;
    }

    //give all selected Units a Stop Command
    if(*doStop == true)
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
        *doStop = false;
    }

    //Group Settings
    if(*getGroup == true)
    {
        if(*ShiftDown == false)
        {
            copySelection(GroupNumber,0);
        }
        else
        {
            AddSelectionFromGroup(GroupNumber,0);
            Error("Added selection from Group");
            sortSelection(0);
        }
        *getGroup = false;
    }
    if(*setGroup == true)
    {
        if(*ShiftDown == false)
        {
            copySelection(0,GroupNumber);
        }
        else
        {
            AddSelectionFromGroup(0,GroupNumber);
            Error("Added selection to Group");
            sortSelection(GroupNumber);
        }
        *setGroup = false;
    }
    if(*jumpGroup == true)
    {
        if(selectedNumber[GroupNumber] > 0) screenJumpRelativeX = CenterMassX(GroupNumber) - SCR_TILE_WIDTH/2 - scrX;
        if(selectedNumber[GroupNumber] > 0) screenJumpRelativeY = CenterMassY(GroupNumber) - SCR_TILE_HEIGHT/2 -scrY;
        *jumpGroup = false;
    }
    if(*SelectMiniPic == true)
    {
        Error("selecting MiniPic Number ",PicNumber);
        if(selectedNumber[0] >= PicNumber)
        {
            cSelection* sel;
            cUnit* Uni;
            sel = pListHead[0];
            for(int i=0;i< PicNumber;i++)
            {
                Error("jumping through selection",i);
                sel=sel->getSuccessor();
            }
            Uni = sel->getContent();
            ClearSelection(0);
            AddSelection(Uni,NULL,0);
        }
        else{ClearSelection(0);}
        *SelectMiniPic = false;
    }
    if(*activateCommand >= 0)
    {
        Error("activate Command",*activateCommand);
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
                    Error("store activate Command",*activateCommand);
                    storeCommand(toMove->getContent(),COMMAND_ACTIVATE_ABILITY,0,0,*activateCommand);
                    //toMove->getContent()->StopCommand();
                }
            }
            Error("tack");
            toMove = toMove->getSuccessor();
        }
        *activateCommand = -1;
    }

    ///TODO adapt to Changelist
    //Move all objects TODO here animations of the world attacks and so on, as each tile is checked
	for(int i=0;i< WORLD_HEIGHT;i++)
	{
	    for(int j=0;j< WORLD_WIDTH;j++)
	    {
	        cUnit* Unit;
            Unit = MapObj.getMap(j,i)->getContainUnit();
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
                        DestoryUnit(Unit);
                    }
                    int entTyp;
                    for(int k=0;k<Unit->getNumberOfAblilities();k++)
                    {
                        if((entTyp = Unit->checkEntity(k)) >= 0)
                        {
                            Error("preparing create Entity");
                            createEntity(entTyp,Unit->getPosition()->getX(),Unit->getPosition()->getY(),Unit->getGoal(),Unit->getUnitGoal());
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
	for(int i=0;i< NumberOfEntities; i++)
	{
	    if(EntityList[i]->checkActive() == false)
	    {
            removeEntity(i);
	    }
        else
        {
            EntityList[i]->Animate(t);
            if(CONTROL_SETTINGS == MODE_SERVER || CONTROL_SETTINGS == MODE_SINGPLAY)
            {
                EntityList[i]->Move(t,&changeListObj);
            }
            else
            {
                EntityList[i]->MoveClient(t,&changeListObj);
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
                CreateUnit(giveNext->UnitID,giveNext->goalUnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
                break;
            }
            case CHANGE_REMOVE :
            {
                cUnit* Unit;
                Unit = giveNext->CommandedUnit;
                DestoryUnit(Unit);
                break;
            }
            case CHANGE_POSITION :
            {
                giveNext->CommandedUnit->applyPositionChange(MapObj.getMap(giveNext->goalX,giveNext->goalY),&changeListObj);
                break;
            }
            case CHANGE_GOAL :
            {
                giveNext->CommandedUnit->setGoal(MapObj.getMap(giveNext->goalX,giveNext->goalY));
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
                    giveNext->CommandedUnit->setUnitGoal(MapObj.getMap(giveNext->goalX,giveNext->goalY)->getContainUnit());
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
                if(MapObj.getMap(giveNext->goalX,giveNext->goalY)->getContainUnit() != NULL)
                {
                    createEntity(giveNext->CommandedUnit->getEnttype(giveNext->moveType),giveNext->posiX,giveNext->posiY
                                ,MapObj.getMap(giveNext->goalX,giveNext->goalY)
                                ,MapObj.getMap(giveNext->goalX,giveNext->goalY)->getContainUnit());
                }
                else
                {
                    createEntity(giveNext->CommandedUnit->getEnttype(giveNext->moveType),giveNext->posiX,giveNext->posiY
                             ,MapObj.getMap(giveNext->goalX,giveNext->goalY),NULL);
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
                ID = checkFreeUnitID();
                Error("ID",ID);
                if(ID != -1)
                {
                    Change* newChange;
                    CreateUnit(ID,giveNext->UnitID,giveNext->posiX,giveNext->posiY,giveNext->goalX);
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
                giveNext->CommandedUnit->MovementCommand(MapObj.getMap(giveNext->goalX,giveNext->goalY),giveNext->priority);
                break;
            }
            case COMMAND_ATTACK:
            {
                giveNext->CommandedUnit->AttackCommand(MapObj.getMap(giveNext->goalX,giveNext->goalY),giveNext->priority);
                break;
            }
            case COMMAND_ACTIVATE_ABILITY:
            {
                if(UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority] >= 0 &&
                   UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority] < ENTITY_TYPE_MAX )
                   {
                       if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_GROUND) Ground = true;
                       if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                   }

                giveNext->CommandedUnit->ChangeAbilityTypeCommand(giveNext->priority,Ground,Self,&changeListObj);
                break;
            }
            case COMMAND_CAST_0:
            {
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][0]][0] >= ENT_PROP_GROUND) Ground = true;
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(0,MapObj.getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_1:
            {
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][1]][0] >= ENT_PROP_GROUND) Ground = true;
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                Error("giving cast command 1");
                giveNext->CommandedUnit->CastCommand(1,MapObj.getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_2:
            {
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][2]][0] >= ENT_PROP_GROUND) Ground = true;
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(2,MapObj.getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
            case COMMAND_CAST_3:
            {
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][3]][0] >= ENT_PROP_GROUND) Ground = true;
                if(modifierType[UnitEntityType[giveNext->CommandedUnit->getType()][giveNext->priority]][0] >= ENT_PROP_SELF) {Ground = false;Self=true;}
                giveNext->CommandedUnit->CastCommand(3,MapObj.getMap(giveNext->goalX,giveNext->goalY),Ground,Self,giveNext->priority);
                break;
            }
        }
        //Clear the Command, as it is no longer on the list
        delete giveNext;
    }
}

void cControl::DestoryUnit(cUnit* Unit)
{
    Error("killing unit");
    Unit->getPosition()->setContain(NULL,NULL);
    if(Unit->getOwner() == thisUser)
    {
        for(int i=0;i<=10;i++)
        {
            RemoveSelection(Unit,NULL,i);
        }
    }
    IDListed[Unit->getID()] = NULL;
    delete Unit;
}

//routines to create Units on the map, dependent on S/C
void cControl::CreateUnit(unsigned int ID,unsigned int type, int xPosi,int yPosi,int owner)
{
    cUnit* newUnit;
    newUnit = new cUnit(ID,type,owner,MapObj.getMap(xPosi,yPosi),UnitAnimSteps[type],UnitAnimDuration[type]
                        ,UnitMovementSpeed[type],attackSpeed[type],health[type],damage[type],sightRange[type]
                        ,tauntRange[type],attackRange[type], UnitEntityType[type],ManaCost[type],NumberOfAblilites[type]
                        ,mana[type],manareg[type],healthreg[type],Armor[type],Magicarmor[type],sortPriority[type],&MapObj);
    //put it onto the ID List
    ///TODO dynamic ID system
    if(ID < MAX_UNITS)
    {
        IDListed[ID] = newUnit;
    }
    //put it onto the map
    MapObj.getMap(xPosi,yPosi)->setContain(newUnit,NULL);
}

void cControl::CreateBuilding(int type,int xPosi, int yPosi, int owner)
{
    cBuilding* newBuilding;
    buildingLayout lay;
    for(int i=0;i<BuildingSizeX[type];i++)
    {
        for(int j=0;j<BuildingSizeY[type];j++)
        {
            lay.layoutSetting[i][j] = BuildingLayoutStorgage[type][i][j];
        }
    }
    Error2("BuildingSizeX[type] ",BuildingSizeX[type]);
    Error2("BuildingSizeY[type] ",BuildingSizeY[type]);
    newBuilding = new cBuilding(type, owner, BuildingSizeX[type], BuildingSizeY[type]);

    newBuilding->initBuilding(&MapObj, xPosi, yPosi, lay);
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
            if(MapObj.getMap(newCommand->posiX,newCommand->posiY)->getContainUnit() != NULL)
            {
                if((MapObj.getMap(newCommand->posiX,newCommand->posiY)->getContainUnit()->getID()) == newCommand->UnitID)
                {
                        newCommand->CommandedUnit = MapObj.getMap(newCommand->posiX,newCommand->posiY)->getContainUnit();
                }
                else
                {
                    newCommand->CommandedUnit = getUnitByID(newCommand->UnitID);
                }
            }
            else
            {
                newCommand->CommandedUnit = getUnitByID(newCommand->UnitID);
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
        if(MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit() != NULL && newChange->changeType != CHANGE_NEW)
        {
            if((MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit()->getID()) == newChange->UnitID)
            {
                    newChange->CommandedUnit = MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit();
            }
            else
            {
                newChange->CommandedUnit = getUnitByID(newChange->UnitID);
            }
        }
        else
        {
            newChange->CommandedUnit = getUnitByID(newChange->UnitID);
        }

        changeListObj.Add(newChange);

        delete stripNext;
    }
}

cUnit* cControl::getUnitByID(unsigned int ID)
{
    if(ID < MAX_UNITS)
    {
        return IDListed[ID];
    }
    else
    {
        return NULL;
    }
    return NULL;
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
        if(MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit() != NULL && newChange->changeType != CHANGE_NEW)
        {
            if((MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit()->getID()) == newChange->UnitID)
            {
                    newChange->CommandedUnit = MapObj.getMap(newChange->posiX,newChange->posiY)->getContainUnit();
            }
            else
            {
                newChange->CommandedUnit = getUnitByID(newChange->UnitID);
            }
        }
        else
        {
            newChange->CommandedUnit = getUnitByID(newChange->UnitID);
        }
        changeListObj.Add(newChange);
        newChange = ReplayObj.readChange(TimeSinceStart);
    }
}

void cControl::createEntity(int Enttype,int srx, int sry, cTile* goalTile, cUnit* goalUnit)
{
    Error("Create Entity");
    cEntity* newEnt;
    int distanceX,distanceY;
    int goalX,goalY,rangeInt;
    float range;
    goalX = goalTile->getX();
    goalY = goalTile->getY();

    distanceX = (srx - goalX)*TILE_WIDTH;// + goalUnit->getOffsetX();
    distanceY = (sry - goalY)*TILE_WIDTH;// + goalUnit->getOffsetY();

    //check if we have an AOE spell;
    if(modifierType[Enttype][0] >= ENT_PROP_AOE && modifierType[Enttype][0] < ENT_PROP_SELF)
    {
        //if so get its range and find pixels within range
        range = sqrt(modifierValue[Enttype][0]);
        rangeInt = int(range);
        for(int i=-rangeInt;i<=rangeInt;i++)
        {
            for(int j=-rangeInt;j<=rangeInt;j++)
            {
                //if in range and if on the map create the entity there
                if((realDist(goalX,goalY,goalX+i,goalY+j) <= range)
                   && goalX+i >= 0 && goalY+j >= 0 && goalX+i < WORLD_WIDTH && goalY+j < WORLD_HEIGHT)
                {
                    distanceX = (srx - goalX-i)*TILE_WIDTH;
                    distanceY = (sry - goalY-j)*TILE_HEIGHT;
                    newEnt = new cEntity(srx,sry,MapObj.getMap(goalX+i,goalY+j),NULL,lifeTime[Enttype],Enttype,EntitynumModifiers[Enttype],
                                    modifierType[Enttype],modifierValue[Enttype],AnimNumberOfSteps[Enttype],AnimDuration[Enttype]);
                    newEnt->Init(&MapObj,startaway[Enttype],distanceX,distanceY,1);
                    NumberOfEntities++;
                    EntityList[NumberOfEntities-1] = newEnt;
                }
            }
        }
    }
    else
    {
        newEnt = new cEntity(srx,sry,goalTile,goalUnit,lifeTime[Enttype],Enttype,EntitynumModifiers[Enttype],
                         modifierType[Enttype],modifierValue[Enttype],AnimNumberOfSteps[Enttype],AnimDuration[Enttype]);
        newEnt->Init(&MapObj,startaway[Enttype],distanceX,distanceY,1);
        NumberOfEntities++;
        EntityList[NumberOfEntities-1] = newEnt;
    }

}

void cControl::removeEntity(int num)
{
    cEntity* remEnt;
    remEnt = EntityList[num];
    NumberOfEntities--;
    for(int i=num;i<NumberOfEntities;i++)
    {
        EntityList[i] = EntityList[i+1];
    }
    delete remEnt;
}

int cControl::checkFreeUnitID()
{
    int ID=newestID;
    for(int i=0;i<MAX_UNITS;i++)
    {
        if(IDListed[ID] == NULL)
        {
            return ID;
        }
        ID++;
        if(ID == MAX_UNITS) ID=0;
    }
    return -1;
}


bool cControl::CleanUpControl()
{
    //Clean the map
    MapObj.ClearMap();

    return 1;
}
