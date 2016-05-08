#include "cUnit.h"

cUnit::cUnit()
{

    //Object values
    type = 0;
    position = NULL;

    sortPriority=0;

    //Animation Values
    actionType = ACTION_REST;
    lastAction = ACTION_REST;
    for(int i=0;i<NMBR_ACTIONS;i++)
    {
        numberOfSteps[i] = 0;
        animDuration[i] = 0;
    }

    animStepNow = 0;
    animRemainingTime = 0;

    //Unit Values
    ID = 0;
    alive = true;
    for(int i=0;i<QUEUE_ACTION_LENGTH;i++) {actionQueue[i]=ACTION_REST; queueGoals[i] = NULL; queueGoalUnits[i] = NULL;}
    owner = 1;
    direction = 1;
    remainingTime = 0;
    movementSpeed = 1000;
    sightRange = 4;
    goal = NULL;
    offsetX = 0;
    offsetY = 0;
    lastX = 0;
    lastY = 0;
    waitingTicks = 0;
    pPathListHead = new cPathKnot;
    pPathListHead->setSucessor(NULL);
    pPathListHead->setPredecessor(NULL);
    pPathListHead->setContain(NULL);
    lockAction = false;

    for(int i=0;i<ABILITY_MAX;i++)
    {
        doEntity[i] = false;
        attackRange[i] = 0;
        Entitytype[i] = 0;
        attackSpeed[i] = 0;
    }
    attackDamage = 0;
    activeAbility = 0;
    NumberAbilities = 1;
    castOnce = -1;

    regenRemainTime = REG_TICK_LENGTH;
}

cUnit::cUnit(unsigned int ident,int typ,int own,cTile* posi,int Steps[NMBR_ACTIONS],int Duration[NMBR_ACTIONS],
             int Speed,int AattackSpeed[ABILITY_MAX],int Ahealth,int Adamage,int AsightRange,int AtauntRange,
             int atkrng[ABILITY_MAX],int Entity[ABILITY_MAX],int manCost[ABILITY_MAX],int abtlyNum,int man,
             int manreg,int healreg, int arm, int magarm,int sorprio,cWorldMap* map)
{
    //Object values
    type = typ;
    position = posi;

    sortPriority = sorprio;

    //Animation Values
    alive = true;
    selected = false;
    lockTarget = false;
    actionType = ACTION_REST;
    lastAction = ACTION_REST;
    for(int i=0;i<NMBR_ACTIONS;i++)
    {
        numberOfSteps[i] = Steps[i];
        animDuration[i] = Duration[i];
    }

    animStepNow = 0;
    animRemainingTime = 0;

    //Unit Values
    ID = ident;
    pWorldMap = map;
    for(int i=0;i<QUEUE_ACTION_LENGTH;i++) {actionQueue[i]=ACTION_REST; queueGoals[i] = NULL; queueGoalUnits[i] = NULL;}
    owner = own;
    direction = 1;
    remainingTime = 0;

    movementSpeed = Speed;
    sightRange = AsightRange;
    tauntRange = AtauntRange;

    maxMana = man;
    mana = man;
    manaReg = manreg;
    Armor = arm;
    MagicArmor = magarm;
    maxHealth = Ahealth;
    health = Ahealth;
    HealthReg = healreg;

    activeAbility = 0;
    NumberAbilities = abtlyNum;

    attackDamage = Adamage;

    for(int i=0; i< NumberAbilities;i++)
    {
        attackSpeed[i] = AattackSpeed[i];
        attackRange[i] = atkrng[i];
        Entitytype[i] = Entity[i];
        manaCost[i] = manCost[i];
    }
    goal = NULL;
    goalUnit = NULL;
    offsetX = 0;
    offsetY = 0;
    lastX = 0;
    lastY = 0;

    for(int i=0;i<ABILITY_MAX;i++)
    {
        doEntity[i] = false;
    }



    waitingTicks = 0;
    pPathListHead = new cPathKnot;
    pPathListHead->setSucessor(NULL);
    pPathListHead->setPredecessor(NULL);
    pPathListHead->setContain(NULL);

    regenRemainTime = REG_TICK_LENGTH;

    castOnce = -1;
}

int cUnit::getOwner()
{
    return owner;
}

void cUnit::setOwner(int own)
{
    owner = own;
}

int cUnit::getSightRange()
{
    return sightRange;
}
void cUnit::setSightRange(int range)
{
    sightRange = range;
}

void cUnit::MovementCommand(cTile* goa,int priority)
{
    Error("receiving movement commands");
    if(actionType != ACTION_DYING)
    {
        if(goa->getContainUnit() != NULL)
        {
            if(goa->getContainUnit()->getOwner() != owner && goa->getContainUnit()->getActionType() != ACTION_DYING)
            {
                AddCommandToQueue(ACTION_INTERCEPT,goa,goa->getContainUnit(),0,priority);
                lockTarget = true;
            }
            else
            {
                //TODO Change to guard??
                AddCommandToQueue(ACTION_MOVE,goa,NULL,0,priority);
            }
        }
        else
        {
            AddCommandToQueue(ACTION_MOVE,goa,NULL,0,priority);
        }
    }
}

void cUnit::AttackCommand(cTile* goa,int priority)
{
    Error("receiving attack commands");
    if(actionType != ACTION_DYING)
    {
        if(goa->getContainType() == OBJ_TYPE_UNIT)
        {
            AddCommandToQueue(ACTION_INTERCEPT,goa,goa->getContainUnit(),0,priority);
            lockTarget = true;
        }
        else
        {
            AddCommandToQueue(ACTION_ATTACKMOVE,goa,NULL,0,priority);
        }
    }
}
void cUnit::StopCommand()
{
    //TODO proper stopping (the last action must be performed; meaning that movement has to be changed)
    if(actionType != ACTION_DYING)
    {
        //goal = position;
        AddCommandToQueue(ACTION_REST,NULL,NULL,0,1);
    }
}
void cUnit::HoldPositionCommand(int priority)
{
    if(actionType != ACTION_DYING)
    {
        AddCommandToQueue(ACTION_HOLDPOSITION,NULL,NULL,0,priority);
    }
}

void cUnit::ChangeAbilityTypeCommand(int type,bool Ground,bool Self,cChangeList* pChList)
{
    if(actionType != ACTION_DYING && actionType != ACTION_FIGHT && (type<NumberAbilities) && (Ground == false))
    {
        if(Self == true)
        {
            lockTarget = true;
            AddCommandToQueue(ACTION_CAST,position,this,type,1);
        }
        else
        {changeActiveAbility(type,pChList);}
    }
}

void cUnit::CastCommand(int type, cTile* castGoal,bool Ground,bool Self,int priority)
{
    Error("Cast Command start");
    Error("Goal X ",castGoal->getX());
    Error("Goal Y ",castGoal->getY());
    lockTarget = true;
    if(actionType != ACTION_DYING && type < NumberAbilities)
    {
        //Instant cast; affects the user
        if(Entitytype[type] >= ENT_PROP_INSTANT)
        {

        }
        //will be targetet
        else
        {
            //targets the ground
            if(Ground == true)
            {
                Error("Adding ground cast command to queue");
                AddCommandToQueue(ACTION_CAST,castGoal,NULL,type,priority);
            }
            //targets an enemy unit
            else
            {
                if(Self == true)
                {
                    AddCommandToQueue(ACTION_CAST,position,this,type,priority);
                }
                else
                {
                    if(castGoal->getContainUnit() != NULL)
                    {
                        Error("Adding cast command to queue");
                        AddCommandToQueue(ACTION_CAST,castGoal,castGoal->getContainUnit(),type,priority);
                    }
                }
            }
        }
    }
}

void cUnit::MoveClient(Uint32 t)
{
    switch(actionType)
    {
        case ACTION_WAIT:{}
        case ACTION_HOLDPOSITION:{}
        case ACTION_REST:
        {
            calculateOffsets();
            offsetX = 0;
            offsetY = 0;
            break;
        }
        case ACTION_INTERCEPT:{}
        case ACTION_ATTACKMOVE:{}
        case ACTION_MOVE:
        {
            if(remainingTime > 0)
            {
                //does it move diagonally?
                if(lastX != position->getX() && lastY != position->getY())
                {
                    //if it does so move by sqrt(2) slower
                    remainingTime -= t/1.41421;
                    //CalculateOffsets
                    calculateOffsets();
                }
                else
                {
                    remainingTime -= t;
                    //CalculateOffsets
                    calculateOffsets();
                }
            }
            else
            {
                calculateOffsets();
                offsetX = 0;
                offsetY = 0;
            }
            break;
        }
        case ACTION_FIGHT:
        {
            if(remainingTime > 0){remainingTime -= t;}
            //calculateOffsets();
            calculateDirection();
            offsetX = 0;
            offsetY = 0;
            break;
        }
        case ACTION_DYING:
        {
            remainingTime -= t;
            calculateOffsets();
            offsetX = 0;
            offsetY = 0;
            break;
        }

    }
}

void cUnit::Move(Uint32 t,cPathFind* pathFindObj,cChangeList* pChList)
{
    cPathKnot* PathObj;
    cPathKnot* PathObj2;
    int pathCheckID;

    if(goalUnit!=NULL && actionType != ACTION_DYING)
    {
        if(goalUnit->getActionType()==ACTION_DYING){goalUnit=NULL;changeMoveType(ACTION_ATTACKMOVE,pChList);lockTarget = false;castOnce=-1;}
        //if(goalUnit == this){goalUnit=NULL;changeMoveType(ACTION_MOVE,pChList);}
    }

    //Update action queue
    if(actionType == ACTION_REST)
    {
        lockAction = false;
        //lockTarget = false;
        updateActionQueue(pChList);
    }
    regenRemainTime -= t;
    if(regenRemainTime <= 0 && actionType != ACTION_DYING)
    {
        takeDamage(-HealthReg,pChList);
        useMana(-manaReg,pChList);
        regenRemainTime += REG_TICK_LENGTH;
    }
    //process the next action in the queue
    switch(actionType)
    {
        //unit just dying
        case ACTION_DYING:
        {
            //Unit sees nothing anymore
            sightRange = 1;
            //still performing the die-animation
            if(remainingTime > 0)
            {
                remainingTime -= t;
            }
            //remove the object TODO check everything
            else
            {
                //actionType = ACTION_REST;
                //removeUnit(pChList);#
                alive=false;
            }
            break;
        }
        //no command in queue
        case ACTION_REST:
        {
            doTauntCheck((float)tauntRange,pChList);
            //nothing to do yet

            break;
        }
        //only attack next fields
        case ACTION_HOLDPOSITION:
        {
            //do nothing TODO attack if enely in the next field
            doTauntCheck(1.5,pChList);
            break;
        }
        //waiting for the path to get free
        case ACTION_WAIT:
        {
            if(lockAction==false){updateActionQueue(pChList);}
            if(actionType != ACTION_WAIT){break;}
            //still waiting
            if(remainingTime > 0)
            {
                remainingTime -= t;
            }
            //check the path anew, choose an action
            else
            {
                if(doTauntCheck((float)tauntRange,pChList) == false)
                {
                    //Check if the path has to be calcutulated anew
                    pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                    if(pathCheckID != 0)
                    {
                        //If it is not free or not set, use the pathfinding algorithm
                        pathFindObj->findPath(pPathListHead,goal,position);
                    }
                    //Check how the path behaves
                    pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);

                    switch(pathCheckID)
                    {
                        //no path set
                        case -2:{}
                        //path lies between the wrong points
                        case -1:
                        {
                            //stop all movement
                            deletePath();
                            changeMoveType(ACTION_REST,pChList);
                            break;
                        }
                        //path completely free
                        case 0:{}
                        //path free at least for now
                        case 1:
                        {
                            changeMoveType(ACTION_MOVE,pChList);
                            break;
                        }
                        //blocked by a Unit
                        case 2:
                        {
                            if(waitingTicks > 0)
                            {
                                //changeMoveType(ACTION_WAIT,pCHList);
                                waitingTicks--;
                                remainingTime += movementSpeed;
                            }
                            else
                            {
                                changeMoveType(ACTION_REST,pChList);
                            }
                            break;

                        }
                        //blocked by an Obstacle
                        case 3:
                        {
                            //stop all movement and reject the path as it leads nowhere
                            changeMoveType(ACTION_REST,pChList);
                            deletePath();
                            break;
                        }
                    }
                }
            }
            //end ACTION_WAIT
            break;
        }
        //currently moving (or at least trying to move)
        case ACTION_MOVE :
        {
            if(remainingTime > 0)
            {
                //does it move diagonally?
                if(lastX != position->getX() && lastY != position->getY())
                {
                    //if it does so move by sqrt(2) slower
                    remainingTime -= t/1.41421;
                    //CalculateOffsets
                    calculateOffsets();
                }
                else
                {
                    remainingTime -= t;
                    //CalculateOffsets
                    calculateOffsets();
                }
            }
            else
            {
                offsetX = 0;
                offsetY = 0;
                if(goal == position)
                {
                    changeMoveType(ACTION_REST,pChList);
                    //TODO Rest mode for animations
                }
                else
                {
                    if(lockAction==false){updateActionQueue(pChList);}
                    if(castOnce != -1)
                    {
                        if(realDist(goal->getX(),goal->getY(),position->getX(),position->getY()) <= attackRange[castOnce])
                        {
                            changeMoveType(ACTION_FIGHT,pChList);
                        }
                    }
                    if(actionType != ACTION_MOVE){break;}
                    //Check if the path has to be calcutulated anew
                    pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                    if(pathCheckID != 0)
                    {
                        //If it is not free or not set, use the pathfinding algorithm
                        pathFindObj->findPath(pPathListHead,goal,position);
                    }
                    //Check how the path behaves
                    pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                    //Error2("CheckpathID",pathCheckID);
                    switch(pathCheckID)
                    {
                        //no path set
                        case -2: {}
                        //path lies between the wrong points
                        case -1:
                        {
                            //stop all movement
                            deletePath();
                            changeMoveType(ACTION_REST,pChList);
                            break;
                        }
                        //path completely free
                        case 0:{}
                        //path free for now; but somewhere obstructed
                        case 1:
                        {
                            PathObj = pPathListHead;

                            //get the last element of the list
                            while(PathObj->getSuccessor() != NULL)
                            {
                                PathObj = PathObj->getSuccessor();
                            }

                            //Change the position of the Object to the last step in the pathlist
                            changePosition(PathObj->getContain(),pChList);

                            //delete last step from the path
                            PathObj2 = PathObj->getPredecessor();
                            if(PathObj2 == NULL){Error("PathObj2 == NULL");}
                            delete PathObj;
                            PathObj2->setSucessor(NULL);

                            //calculateOffsets();

                            break;
                        }
                        //blocked by a Unit
                        case 2:
                        {
                            //stop movement and wait for GENEREAL_WAITING_TIME ticks
                            changeMoveType(ACTION_WAIT,pChList);
                            lastX = position->getX();
                            lastY = position->getY();
                            break;
                        }
                        //blocked by an Obstacle
                        case 3:
                        {
                            //stop all movement and reject the path as it leads nowhere
                            changeMoveType(ACTION_REST,pChList);
                            deletePath();
                            break;
                        }
                    }
                }
            }
            //CalculateOffsets
            //calculateOffsets();

            //end ACITON_MOVE
            break;
        }
        //currently doing an attackmove(is beeing interrupted by a taunt)
        case ACTION_ATTACKMOVE:
        {
            if(remainingTime > 0)
            {
                //does it move diagonally?
                if(lastX != position->getX() && lastY != position->getY())
                {
                    //if it does so move by sqrt(2) slower
                    remainingTime -= t/1.41421;
                    //CalculateOffsets
                    calculateOffsets();

                }
                else
                {
                    remainingTime -= t;
                    //CalculateOffsets
                    calculateOffsets();

                }
            }
            else
            {
                if(lockAction ==false){updateActionQueue(pChList);}
                if(actionType != ACTION_ATTACKMOVE){break;}
                offsetX = 0;
                offsetY = 0;
                if(doTauntCheck((float)tauntRange,pChList) == false)
                {
                    if(goal == position)
                    {
                        changeMoveType(ACTION_REST,pChList);
                    }
                    else
                    {
                        //Check if the path has to be calcutulated anew
                        pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                        if(pathCheckID != 0)
                        {
                            //If it is not free or not set, use the pathfinding algorithm
                            pathFindObj->findPath(pPathListHead,goal,position);
                        }
                        //Check how the path behaves
                        pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                        //Error2("CheckpathID",pathCheckID);
                        switch(pathCheckID)
                        {
                            //no path set
                            case -2: {}
                            //path lies between the wrong points
                            case -1:
                            {
                                //stop all movement
                                deletePath();
                                changeMoveType(ACTION_REST,pChList);
                                break;
                            }
                            //path completely free
                            case 0:{}
                            //path free for now; but somewhere obstructed
                            case 1:
                            {
                                PathObj = pPathListHead;

                                //get the last element of the list
                                while(PathObj->getSuccessor() != NULL)
                                {
                                    PathObj = PathObj->getSuccessor();
                                }

                                //Change the position of the Object to the last step in the pathlist
                                changePosition(PathObj->getContain(),pChList);

                                //delete last step from the path
                                PathObj2 = PathObj->getPredecessor();
                                if(PathObj2 == NULL){Error("PathObj2 == NULL");}
                                delete PathObj;
                                PathObj2->setSucessor(NULL);

                                calculateOffsets();

                                break;
                            }
                            //blocked by a Unit
                            case 2:
                            {
                                //stop movement and wait for GENEREAL_WAITING_TIME ticks
                                changeMoveType(ACTION_WAIT,pChList);
                                break;
                            }
                            //blocked by an Obstacle
                            case 3:
                            {
                                //stop all movement and reject the path as it leads nowhere
                                changeMoveType(ACTION_REST,pChList);
                                deletePath();
                                break;
                            }
                        }
                    }
                }

            }

            //end ACITON_ATTACKMOVE
            break;
        }
        //trying to intercept an enemy
        case ACTION_INTERCEPT:
        {
            //check if in sight TODO
            //check if goal is unit itself
            if(goalUnit != this)
            {
                if(remainingTime > 0)
                {
                    //does it move diagonally?
                    if(lastX != position->getX() && lastY != position->getY())
                    {
                        //if it does so move by sqrt(2) slower
                        remainingTime -= t/1.41421;
                        //CalculateOffsets
                        calculateOffsets();

                    }
                    else
                    {
                        remainingTime -= t;
                        //CalculateOffsets
                        calculateOffsets();
                    }
                }
                else
                {
                    Error("starting Intercept");
                    offsetX = 0;
                    offsetY = 0;
                    //the used castNumber; check wheter the single or the regular cast is used
                    int abiTypeLocal;
                    if(castOnce == -1){abiTypeLocal = activeAbility;}
                    else{abiTypeLocal = castOnce;}
                    //update the action queue if this action is not locked
                    if(lockAction==false){updateActionQueue(pChList);}
                    if(lockTarget==false){doTauntCheck((float)tauntRange,pChList);}
                    //if the actionType is no longer ACTION_INTERCEPT, exit this action (TODO dont use break)
                    if(actionType != ACTION_INTERCEPT){break;}
                    //check if we have enough mana to do the spell
                    if(manaCost[abiTypeLocal] > mana){changeMoveType(ACTION_REST,pChList);changeActiveAbility(0,pChList);break;}
                    Error("starting Intercept 2");
                    //the ability aims a unit make sure you are still walking there
                    //if(Entitytype[abiTypeLocal] < ENT_PROP_GROUND)
                    //{
                        //if((goalUnit==NULL)){actionType = ACTION_REST;changeMoveType(ACTION_REST,pChList);break;}
                        Error("sterbinger?");
                        if(goalUnit!=NULL)
                        {
                            changeGoal(goalUnit->getPosition(),pChList);
                            if(goalUnit->getHealth() <= 0)
                            {
                                goalUnit = NULL;
                                actionType = ACTION_REST;
                                changeMoveType(ACTION_REST,pChList);
                                break;
                            }
                        }
                    //}

                    //CheckDistance; if enemy is in range do the rangedAttack
                    if(realDist(position->getX(),position->getY(),goal->getX(),goal->getY()) <= attackRange[abiTypeLocal] )
                    {
                        changeMoveType(ACTION_FIGHT,pChList);
                    }
                    //If the enemy is out of range, try to walk closer;
                    else
                    {
                        //Check if the path has to be calcutulated anew
                        pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);
                        if(pathCheckID != 0)
                        {
                            //If it is not free or not set, use the pathfinding algorithm
                            pathFindObj->findPath(pPathListHead,goal,position);
                        }
                        //Check how the path behaves
                        pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);

                        switch(pathCheckID)
                        {
                            //no path set
                            case -2: {}
                            //path lies between the wrong points
                            case -1:
                            {
                                //stop all movement
                                deletePath();
                                changeMoveType(ACTION_REST,pChList);
                                break;
                            }
                            //path completely free
                            case 0:{}
                            //path free for now; but somewhere obstructed
                            case 1:
                            {
                                PathObj = pPathListHead;

                                //get the last element of the list
                                while(PathObj->getSuccessor() != NULL)
                                {
                                    PathObj = PathObj->getSuccessor();
                                }

                                //Change the position of the Object to the last step in the pathlist
                                changePosition(PathObj->getContain(),pChList);

                                //delete last step from the path
                                PathObj2 = PathObj->getPredecessor();
                                if(PathObj2 == NULL){Error("PathObj2 == NULL");}
                                delete PathObj;
                                PathObj2->setSucessor(NULL);

                                //Change timer
                                //remainingTime += movementSpeed;
                                calculateOffsets();

                                Error("Intercept ID 1");

                                break;
                            }
                            //blocked by a Unit
                            case 2:
                            {
                                Error("Intercept ID 2");
                                //Cycle through list
                                PathObj = pPathListHead->getSuccessor();
                                while(PathObj->getSuccessor() != NULL)
                                {
                                    PathObj = PathObj->getSuccessor();
                                }
                                //if the next Unit is the enemy unit to attack, do damage to it
                                if(goalUnit==NULL){break;}
                                if(PathObj->getContain()->getContainUnit() == goalUnit && goalUnit->getHealth() > 0)
                                {
                                    changeMoveType(ACTION_FIGHT,pChList);
                                    changeGoal(goalUnit->getPosition(),pChList);
                                    //goal=goalUnit->getPosition();
                                    break;
                                }

                                //if it is just another unit; wait TODO not let it kill the goal of the attack
                                else
                                {
                                    //stop movement and wait for GENEREAL_WAITING_TIME ticks
                                    changeMoveType(ACTION_WAIT,pChList);
                                    break;
                                }
                                break;


                            }
                            //blocked by an Obstacle
                            case 3:
                            {
                                //stop all movement and reject the path as it leads nowhere
                                changeMoveType(ACTION_REST,pChList);
                                deletePath();
                                break;
                            }
                        }
                    }

                }
                //calculateOffsets();
                break;


            }
            else
            {
                changeMoveType(ACTION_REST,pChList);
                changeUnitGoal(NULL,pChList);
                if(castOnce != -1){castOnce = -1;}
                break;
            }
        }
        //fighting against an enemy
        case ACTION_FIGHT:
        {
            //the used castNumber; check wheter the single or the regular cast is used
            int abiTypeLocal;
            if(castOnce == -1){abiTypeLocal = activeAbility;}
            else{abiTypeLocal = castOnce;}
            //update the action queue if this action is not locked
            if(lockAction==false){updateActionQueue(pChList);}
            //if the actionType is no longer action_fight, exit this action (TODO dont use break)
            if(actionType!=ACTION_FIGHT){break;}
            //check if we have enough mana to do the spell
            if(manaCost[abiTypeLocal] > mana){changeMoveType(ACTION_REST,pChList);changeActiveAbility(0,pChList);break;}

            //the ability aims a unit
            //if(Entitytype[abiTypeLocal] < ENT_PROP_GROUND)
            //{
            if((goalUnit!=NULL))//{actionType = ACTION_REST;changeMoveType(ACTION_REST,pChList);break;}
            {
                if(goalUnit->getPosition() != goal)
                {
                    if(realDist(goalUnit->getPosition()->getX(),goalUnit->getPosition()->getY(),
                                position->getX(),position->getY()) > attackRange[abiTypeLocal] )
                    {
                        remainingTime = 0;
                        if(doTauntCheck(tauntRange,pChList)==false){changeMoveType(ACTION_INTERCEPT,pChList);}
                        goal = goalUnit->getPosition();
                        break;
                    }
                    else
                    {
                        goal = goalUnit->getPosition();
                    }
                }
                if(goalUnit->getHealth() <= 0)
                {
                    goalUnit=NULL;
                    changeMoveType(ACTION_REST,pChList);
                    break;
                }
            }
            //}
            //we have a ability aiming to the ground
            else
            {

            }

            //still performing the attack animation
            if(remainingTime > 0)
            {
                calculateDirection();
                remainingTime -= t;
            }
            //attack again;calculate the path; if it is not 2
            else
            {
                if(realDist(position->getX(),position->getY(),goal->getX(),goal->getY()) <= attackRange[abiTypeLocal] )
                {
                    if(manaCost[abiTypeLocal] <= mana)
                    {
                        Error("cangeCreateEntity call");
                        //create the Enitity on the goal and use the mana needed
                        changeCreateEntity(abiTypeLocal,pChList);
                        useMana(manaCost[abiTypeLocal],pChList);
                        //this ability is active, so just attack again
                        if(castOnce == -1)
                        {
                            changeMoveType(ACTION_FIGHT,pChList);
                        }
                        //the standard ability is active
                        else
                        {
                            changeMoveType(ACTION_REST,pChList);
                            changeCastOnce(-1,pChList);
                        }
                    }

                }
                else
                {
                    //ranged Units will not attack if out of range (TODO dont use break)
                    if(attackRange[abiTypeLocal] > 0)
                    {
                        changeMoveType(ACTION_INTERCEPT,pChList);
                        break;
                    }
                    //deal damage
                    Error("dealing damage");
                    if(goalUnit != NULL)
                    {
                        if(goalUnit->getHealth() > 0)
                        {
                            int damage;
                            damage = attackDamage - goalUnit->getArmor();
                            if(damage <= 0) damage =1;
                            goalUnit->takeDamage(damage,pChList);
                        }
                        else
                        {
                            changeUnitGoal(NULL,pChList);
                            goalUnit = NULL;
                            changeMoveType(ACTION_REST,pChList);
                            break;
                        }
                    }

                    //get the position of the Unit
                    if(goalUnit!=NULL){goal=goalUnit->getPosition();}
                    //Calculate the path again(shouldnt be too far; as the unit just attacked it)
                    pathFindObj->findPath(pPathListHead,goal,position);
                    //check its ID
                    pathCheckID = pathFindObj->checkPath(pPathListHead,goal,position);

                    switch(pathCheckID)
                    {
                        //no path set
                        case -2: {}
                        //path lies between the wrong points
                        case -1:
                        {
                            //stop all movement
                            deletePath();
                            changeMoveType(ACTION_REST,pChList);
                            break;
                        }
                        //path completely free
                        case 0:{}
                        //path free for now; but somewhere obstructed
                        case 1:
                        {
                            PathObj = pPathListHead;

                            //get the last element of the list
                            while(PathObj->getSuccessor() != NULL)
                            {
                                PathObj = PathObj->getSuccessor();
                            }

                            //Change the position of the Object to the last step in the pathlist
                            changePosition(PathObj->getContain(),pChList);
                            changeMoveType(ACTION_INTERCEPT,pChList);

                            //delete last step from the path
                            PathObj2 = PathObj->getPredecessor();
                            if(PathObj2 == NULL){Error("PathObj2 == NULL");}
                            delete PathObj;
                            PathObj2->setSucessor(NULL);

                            calculateOffsets();

                            break;
                        }
                        //blocked by a Unit
                        case 2:
                        {
                            Error("Intercept ID 2");
                            //Cycle through list
                            PathObj = pPathListHead->getSuccessor();
                            while(PathObj->getSuccessor() != NULL)
                            {
                                PathObj = PathObj->getSuccessor();
                            }
                            //if the next Unit is the enemy unit to attack, do damage to it
                            if(goalUnit==NULL){break;}
                            if(PathObj->getContain()->getContainUnit() == goalUnit && goalUnit->getHealth() > 0)
                            {
                                changeMoveType(ACTION_FIGHT,pChList);
                                //remainingTime += attackSpeed;
                                break;
                            }

                            //if it is just another unit; wait TODO not let it kill the goal of the attack
                            else
                            {
                                //stop movement and wait for GENEREAL_WAITING_TIME ticks
                                changeMoveType(ACTION_WAIT,pChList);
                                break;
                            }


                        }
                        //blocked by an Obstacle
                        case 3:
                        {
                            //stop all movement and reject the path as it leads nowhere
                            changeMoveType(ACTION_REST,pChList);
                            deletePath();
                            break;
                        }
                    }
                }

            }
            break;
        }
    }
    //Error("move done action type ",actionType);
}

//gets the first command from the queue, and updates the rest
void cUnit::updateActionQueue(cChangeList* pChList)
{
    //basic normal commands
    if(actionQueue[0] < 16)
    {
        if(actionQueue[0] != ACTION_REST)
        {
            Error("updating action queue ",actionQueue[0]);
            changeMoveType(actionQueue[0],pChList);
            changeGoal(queueGoals[0],pChList);
            changeCastOnce(castOnce,pChList);
            if(queueGoalUnits[0] != NULL){changeUnitGoal(queueGoalUnits[0],pChList);}
            for(int j=1;j<QUEUE_ACTION_LENGTH;j++)
            {
                actionQueue[j-1] = actionQueue[j];
                queueGoals[j-1] = queueGoals[j];
                queueGoalUnits[j-1] = queueGoalUnits[j];
                queueTypeModifs[j-1] = queueTypeModifs[j];
            }
            actionQueue[QUEUE_ACTION_LENGTH-1]=ACTION_REST;
            queueGoals[QUEUE_ACTION_LENGTH-1]=NULL;
            queueGoalUnits[QUEUE_ACTION_LENGTH-1]=NULL;
            queueTypeModifs[QUEUE_ACTION_LENGTH-1]=0;
            castOnce = -1;
            lockTarget = false;
        }
    }
    //pseudocommands
    else
    {
        switch(actionQueue[0])
        {
            case ACTION_CAST:
            {
                changeCastOnce(queueTypeModifs[0],pChList);
                changeGoal(queueGoals[0],pChList);
                Error("applying cast command ground");
                changeMoveType(ACTION_INTERCEPT,pChList);
                changeUnitGoal(queueGoalUnits[0],pChList);
                break;
            }
        }
        for(int j=1;j<QUEUE_ACTION_LENGTH;j++)
        {
            actionQueue[j-1] = actionQueue[j];
            queueGoals[j-1] = queueGoals[j];
            queueGoalUnits[j-1] = queueGoalUnits[j];
            queueTypeModifs[j-1] = queueTypeModifs[j];
        }
        actionQueue[QUEUE_ACTION_LENGTH-1]=ACTION_REST;
        queueGoals[QUEUE_ACTION_LENGTH-1]=NULL;
        queueGoalUnits[QUEUE_ACTION_LENGTH-1]=NULL;
        queueTypeModifs[QUEUE_ACTION_LENGTH-1]=0;
    }
}

//adds a command to the queue priorities: 0 kill the list, instantly take the new command for granted; 1 add to the beginning of the list;
// 2 kill list; set command as first on the list
void cUnit::AddCommandToQueue(int type,cTile* goa,cUnit* goaUni,int typeModif,int priority)
{
    //Error("adding command to queue priority ",priority);
    switch(priority)
    {
        //kill the list, only this command exitst
        case 0:
        {
            for(int i=0;i<QUEUE_ACTION_LENGTH;i++)
            {
                //push commands back
                actionQueue[i] = ACTION_REST;
                queueGoals[i] = NULL;
                queueGoalUnits[i] = NULL;
                queueTypeModifs[i] = 0;
            }
            actionType = type;
            goal = goa;
            goalUnit = goaUni;
            break;
        }
        //kill the list; add to command to beginning of list // change goal
        case 1:
        {
            for(int i=0;i<QUEUE_ACTION_LENGTH;i++)
            {
                //push commands back
                actionQueue[i] = ACTION_REST;
                queueGoals[i] = NULL;
                queueGoalUnits[i] = NULL;
                queueTypeModifs[i] = 0;
            }
            actionQueue[0] = type;
            queueGoals[0] = goa;
            queueGoalUnits[0] = goaUni;
            queueTypeModifs[0] = typeModif;
            if(goal != NULL){goal = position;}
            if(goalUnit != NULL){goalUnit = position->getContainUnit();}
            //goalUnit = NULL;
            break;
        }
        //add to the beginning of the list
        case 2:
        {
            for(int i=QUEUE_ACTION_LENGTH-1;i>=0;i--)
            {
                //push commands back
                actionQueue[i+1] = actionQueue[i];
                queueGoals[i+1] = queueGoals[i];
                queueGoalUnits[i+1] = queueGoalUnits[i];
                queueTypeModifs[i+1] = queueTypeModifs[i];
            }
            actionQueue[0] = type;
            queueGoals[0] = goa;
            queueGoalUnits[0] = goaUni;
            queueTypeModifs[0] = typeModif;
            break;
        }
        //add to the end of the list
        default:
        {
            for(int i=0;i<QUEUE_ACTION_LENGTH;i++)
            {
                if(actionQueue[i] == ACTION_REST)
                {
                    actionQueue[i] = type;
                    queueGoals[i] = goa;
                    queueGoalUnits[i] = goaUni;
                    queueTypeModifs[i] = type;
                }
            }
        }
    }
    //Error("added command to queue type ",type);


}

bool cUnit::doTauntCheck(float rang,cChangeList* pChList)
{
    cUnit* checkUnit = NULL;
    cUnit* attackUnit = NULL;
    int kmin,kmax,lmin,lmax;
    float minimum = rang;
    int roundup;
    roundup = rang + 0.99;
    kmin = position->getX() - roundup;
    kmax = position->getX() + roundup;
    lmin = position->getY() - roundup;
    lmax = position->getY() + roundup;
    if(kmin<0){kmin=0;}
    if(kmax>=WORLD_WIDTH){kmax=WORLD_WIDTH-1;}
    if(lmin<0){lmin=0;}
    if(lmax>=WORLD_WIDTH){lmax=WORLD_WIDTH-1;}

    //check all tiles that might be in tauntRange
    for(int k=kmin;k<=kmax;k++)
    {
        for(int l=lmin;l<=lmax;l++)
        {
            //check if the real distance is really small enough
            if(realDist(k,l,position->getX(),position->getY())<=minimum)
            {
                //check if there is an enemy
                checkUnit = pWorldMap->getMap(k,l)->getContainUnit();
                if(checkUnit!= NULL)
                {
                    if(checkUnit->getOwner() != owner && checkUnit->getActionType() != ACTION_DYING)
                    {
                        //Check nearest Unit
                        minimum = realDist(k,l,position->getX(),position->getY());
                        attackUnit=checkUnit;
                    }
                }
            }
        }
    }
    if(attackUnit != NULL)
    {
        changeUnitGoal(attackUnit,pChList);
        ///TODO with time 0 howto?
        changeMoveType(ACTION_INTERCEPT,pChList);
        ///DEBUG
        //AddCommandToQueue(ACTION_INTERCEPT,NULL,attackUnit,3);
        return true;
    }
    return false;
}

void cUnit::calculateOffsets()
{
    offsetX = 0;
    offsetY = 0;
    //Check for the side
    //to the left
    if(lastX > position->getX())
    {
        offsetX = remainingTime*TILE_WIDTH / movementSpeed;
        //up
        if(lastY > position->getY())
        {
            offsetY = remainingTime*TILE_HEIGHT / movementSpeed;
            direction = 8;
        }
        //down
        if(lastY < position->getY())
        {
            direction = 6;
            offsetY = -remainingTime*TILE_HEIGHT / movementSpeed;
        }
        //same y
        if(lastY == position->getY())
        {
            direction = 7;
        }
    }
    //to the right
    if(lastX < position->getX())
    {
        offsetX = -remainingTime*TILE_WIDTH / movementSpeed;
        //up
        if(lastY > position->getY())
        {
            offsetY = remainingTime*TILE_HEIGHT / movementSpeed;
            direction = 2;
        }
        //down
        if(lastY < position->getY())
        {
            direction = 4;
            offsetY = -remainingTime*TILE_HEIGHT / movementSpeed;
        }
        //same y
        if(lastY == position->getY())
        {
            direction = 3;
        }
    }
    //same x
    if(lastX == position->getX())
    {
        //up
        if(lastY > position->getY())
        {
            offsetY = remainingTime*TILE_HEIGHT / movementSpeed;
            direction = 1;
        }
        //down
        if(lastY < position->getY())
        {
            direction = 5;
            offsetY = -remainingTime*TILE_HEIGHT / movementSpeed;
        }
        //same y
    }
}

void cUnit::takeDamage(int dmg,cChangeList* pChList)
{
    //health -= dmg;
    Error("health", health);
    if(health-dmg <= 0)
    {
        changeMoveType(ACTION_DYING,pChList);
        changeHealth(0,pChList);
    }
    else
    {
        if(health-dmg > maxHealth)
        {
            changeHealth(maxHealth,pChList);
        }
        else
        {
            changeHealth(health - dmg,pChList);
        }
    }

}
void cUnit::useMana(int man,cChangeList* pChList)
{
    int chng;
    chng = mana - man;
    if(chng >= 0 && chng <= maxMana)
    {
        changeMana(chng,pChList);
    }
}

void cUnit::deletePath()
{
    //Error("deleting Path");
    cPathKnot* toDelete;
    cPathKnot* toCheck;
    toDelete = pPathListHead->getSuccessor();
    if(toDelete != NULL)
    {
        while(toDelete->getSuccessor() != NULL)
        {
            //Error("   loop");
            toCheck = toDelete->getSuccessor();
            delete toDelete;
            toCheck->setPredecessor(NULL);
            toDelete = toCheck;
        }
    }
    pPathListHead->setSucessor(NULL);
}

void cUnit::changeActiveAbility(int abi,cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_ACTIVE_ABILITY;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = 0;
    newChange->goalY = 0;
    newChange->goalUnitID = 0;

    newChange->health = 0;

    newChange->moveType = abi;

    newChange->time = 0;

    pChList->Add(newChange);

    Error("changeActiveAbility", abi);

    setActiveAbility(abi);
}

void cUnit::removeUnit(cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->CommandedUnit = this;
    newChange->UnitID = ID;

    newChange->changeType = CHANGE_REMOVE;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = 0;
    newChange->goalY = 0;
    newChange->goalUnitID = 0;

    newChange->health = 0;

    newChange->moveType = ACTION_REST;

    newChange->time = 0;

    pChList->Add(newChange);
}

void cUnit::changePosition(cTile* pos, cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_POSITION;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = pos->getX();
    newChange->goalY = pos->getY();
    newChange->goalUnitID = 0;

    newChange->health = 0;

    newChange->moveType = ACTION_REST;

    newChange->time = 0;

    pChList->Add(newChange);

    position->setContain(NULL,NULL);
    pos->setContain(this,NULL);
    lastX = position->getX();
    lastY = position->getY();
    position = pos;
    remainingTime += movementSpeed;
    calculateOffsets();
}

void cUnit::changeGoal(cTile* goa,cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_GOAL;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = goa->getX();
    newChange->goalY = goa->getY();
    newChange->goalUnitID = 0;

    newChange->health = 0;

    newChange->moveType = ACTION_REST;

    newChange->time = 0;

    pChList->Add(newChange);

    goal = goa;
}

void cUnit::changeUnitGoal(cUnit* goaUn,cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_UNITGOAL;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    if(goaUn != NULL)
    {
        newChange->goalX = goaUn->getPosition()->getX();
        newChange->goalY = goaUn->getPosition()->getY();
        newChange->goalUnitID = goaUn->getID();
    }
    else
    {
        newChange->goalX = MAX_WORLD_WIDTH+1;
        newChange->goalY = MAX_WORLD_HEIGHT+1;
        newChange->goalUnitID = 0;
    }

    newChange->health = 0;

    newChange->moveType = ACTION_REST;

    newChange->time = 0;

    pChList->Add(newChange);

    goalUnit = goaUn;
}

void cUnit::changeMoveType(int type, cChangeList* pChList)
{
    if(actionType != ACTION_DYING)
    {
        Change* newChange;
        newChange = new Change;
        newChange->UnitID = ID;
        newChange->CommandedUnit = this;

        newChange->changeType = CHANGE_MOVETYPE;

        newChange->posiX = position->getX();
        newChange->posiY = position->getY();

        newChange->goalX = 0;
        newChange->goalY = 0;
        newChange->goalUnitID = 0;

        newChange->health = 0;

        newChange->moveType = type;

        newChange->time = 0;

        pChList->Add(newChange);

        //apply this change already locally
        applyMovementChange(type);
    }
}

void cUnit::changeHealth(int heal, cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_HEALTH;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = 0;
    newChange->goalY = 0;
    newChange->goalUnitID = 0;

    newChange->health = heal;

    newChange->time = 0;

    pChList->Add(newChange);

    health = heal;
}

void cUnit::changeCreateEntity(int slot, cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_CREATE_ENT;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = goal->getX();
    newChange->goalY = goal->getY();
    if(goalUnit != NULL) newChange->goalUnitID = goalUnit->getID();

    newChange->moveType = slot;

    newChange->time = 0;

    pChList->Add(newChange);
    Error("createEntity call");
    createEntity(slot);
}

void cUnit::changeMana(int man, cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_MANA;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = 0;
    newChange->goalY = 0;
    newChange->goalUnitID = 0;

    newChange->health = man;

    newChange->time = 0;
    pChList->Add(newChange);
    mana = man;
}

void cUnit::changeCastOnce(int cOne, cChangeList* pChList)
{
    Change* newChange;
    newChange = new Change;
    newChange->UnitID = ID;
    newChange->CommandedUnit = this;

    newChange->changeType = CHANGE_CAST_ONCE;

    newChange->posiX = position->getX();
    newChange->posiY = position->getY();

    newChange->goalX = 0;
    newChange->goalY = 0;
    newChange->goalUnitID = 0;

    newChange->moveType = cOne;
    if(cOne == -1) {newChange->moveType = 100000;}

    newChange->time = 0;
    pChList->Add(newChange);

    castOnce = cOne;
}

void cUnit::createEntity(int slot)
{
    doEntity[slot]= true;
}

void cUnit::applyMovementChange(int type)
{
    actionType = type;
    switch(type)
    {
        case ACTION_REST:
        {
            remainingTime = 0;
            break;
        }
        case ACTION_DYING:
        {
            remainingTime = animDuration[ACTION_DYING];
            break;
        }
        case ACTION_HOLDPOSITION:
        {
            remainingTime = 0;
            break;
        }
        case ACTION_WAIT:
        {
            waitingTicks = GENERAL_WAITING_TIME;
            remainingTime += movementSpeed;
            break;
        }
        case ACTION_MOVE:
        {
            //remainingTime += movementSpeed;
            remainingTime = 0;
            break;
        }
        case ACTION_ATTACKMOVE:
        {
            //remainingTime += movementSpeed;
            remainingTime = 0;
            break;
        }
        case ACTION_INTERCEPT:
        {
            //remainingTime += movementSpeed;
            remainingTime = 0;
            break;
        }
        case ACTION_FIGHT:
        {
            if(castOnce == -1){remainingTime += attackSpeed[activeAbility];}
            else{remainingTime += attackSpeed[castOnce];}
            break;
        }
        case ACTION_INTERCEPT_ZERO:
        {
            actionType = ACTION_INTERCEPT;
            remainingTime = 0;
            break;
        }
    }

}

void cUnit::applyPositionChange(cTile* pos,cChangeList* pChList)
{
    if(pos->getContainType() == 0)
    {
        position->setContain(NULL,NULL);
        pos->setContain(this,NULL);
        lastX = position->getX();
        lastY = position->getY();
        position = pos;
        if(CONTROL_SETTINGS == ( MODE_SERVER || MODE_SINGPLAY )) remainingTime += movementSpeed;
        else remainingTime = movementSpeed;
        calculateOffsets();
    }
    else
    {
        //changePosition(pos,pChList);
    }
}

void cUnit::calculateDirection()
{
    if(goal!=NULL)
    {
        //to the right
        if(goal->getX() > position->getX())
        {
            //down
            if(goal->getY() > position->getY())
            {
                direction = 4;
            }
            //up
            if(goal->getY() < position->getY())
            {
                direction = 2;
            }
            //same y
            if(goal->getY() == position->getY())
            {
                direction = 3;
            }
        }
        //to the left
        if(goal->getX() < position->getX())
        {
            //down
            if(goal->getY() > position->getY())
            {
                direction = 6;
            }
            //up
            if(goal->getY() < position->getY())
            {
                direction = 8;
            }
            //same y
            if(goal->getY() == position->getY())
            {
                direction = 7;
            }
        }
        //same x
        if(goal->getX() == position->getX())
        {
            //down
            if(goal->getY() > position->getY())
            {
                direction = 5;
            }
            //up
            if(goal->getY() < position->getY())
            {
                direction = 1;
            }
        }
    }
}


unsigned int cUnit::getID()
{
    return ID;
}

int cUnit::getDirection()
{
    return direction;
}
void cUnit::setDirection(int dir)
{
    direction = dir;
}

int cUnit::getOffsetX()
{
    return offsetX;
}
int cUnit::getOffsetY()
{
    return offsetY;
}

int cUnit::getHealth()
{
    return health;
}
void cUnit::setHealth(int heal)
{
    health = heal;
}

int cUnit::getAttackDamage()
{
    return attackDamage;
}
void cUnit::setAttackDamage(int dmg)
{
    attackDamage = dmg;
}

int cUnit::getAttackRange(int slot)
{
    return attackRange[slot];
}
void cUnit::setAttackRange(int ran,int slot)
{
    attackRange[slot] = ran;
}

int cUnit::getTauntRange()
{
    return tauntRange;
}
void cUnit::setTauntRange(int ran)
{
    tauntRange= ran;
}

bool cUnit::checkAlive()
{
    return alive;
}

bool cUnit::checkSelected()
{
    return selected;
}

void cUnit::setSelected(bool sel)
{
    selected = sel;
}

void cUnit::setGoal(cTile* goa)
{
    goal = goa;
}
void cUnit::setUnitGoal(cUnit* uni)
{
    goalUnit = uni;
}

int cUnit::getArmor()
{
    return Armor;
}
int cUnit::getMagicArmor()
{
    return MagicArmor;
}

int cUnit::getMaxHealth()
{
    return maxHealth;
}
void cUnit::setMaxHealth(int heal)
{
    maxHealth = heal;
}
int cUnit::getMana()
{
    return mana;
}
int cUnit::getMaxMana()
{
    return maxMana;
}
void cUnit::setMana(int man)
{
    mana = man;
}

int cUnit::checkEntity(int slot)
{
    if(doEntity[slot] == true)
    {
        doEntity[slot] = false;
        return Entitytype[slot];
    }
    else
    {
        return -1;
    }
}

int cUnit::getNumberOfAblilities()
{
    return NumberAbilities;
}

void cUnit::setSpeed(int sped)
{
    movementSpeed = sped;
}
int cUnit::getSpeed()
{
    return movementSpeed;
}

cTile* cUnit::getGoal()
{
    return goal;
}
cUnit* cUnit::getUnitGoal()
{
    return goalUnit;
}

int cUnit::getEnttype(int slot)
{
    return Entitytype[slot];
}

int cUnit::getCastOnce()
{
    return castOnce;
}

void cUnit::setCastOnce(int cOne)
{
    castOnce = cOne;
}

int cUnit::getActiveAbility()
{
    return activeAbility;
}

void cUnit::setActiveAbility(int abi)
{
    if(activeAbility < NumberAbilities){activeAbility = abi;}
}

int cUnit::getSortPriority()
{
    return sortPriority;
}

bool cUnit::checkAbility(int slot)
{
    if(slot < NumberAbilities)
    {
        if(mana >= manaCost[slot])
        {
            if(castOnce == -1)
            {
                return true;
            }
        }
    }
    return false;
}

cUnit::~cUnit()
{
    //remove the path objects
    deletePath();
    delete pPathListHead;
}

/*cUnitList::cUnitList()
{
    pContent = NULL;
    pLast = NULL;
    pNext = NULL;
}

cUnitList::cUnitList(cUnit* cont,cUnitList* suc,cUnitList* pre)
{
    pContent = cont;
    pLast = pre;
    pNext = suc;
}

cTile* cUnitList::getContent()
{
    return pContent;
}

void cUnitList::setContent(cUnit* cont)
{
    pContent = cont;
}

cUnitList* cUnitList::getSuccessor()
{
    return pNext;
}

void cUnitList::setSucessor(cUnitList* suc)
{
    pNext = suc;
}

cUnitList* cUnitList::getPredecessor()
{
    return pLast;
}

void cUnitList::setPredecessor(cUnitList* pre)
{
    pLast = pre;
}*/
