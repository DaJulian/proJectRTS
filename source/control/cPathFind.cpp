#include "cPathFind.h"

void cPathFind::Init()
{
    closedList = new cClosedList;
    openList = new cOpenList;
    openListArray = new cOpenListArray;
}

void cPathFind::setMap(cWorldMap* Map)
{
    pWorldMap=Map;
}
void cPathFind::findPath(cPathKnot* pPathListHead,cTile* goal,cTile* posi)
{
    //!!!!!!!!!!!!!!!
    //TODO: Real Pathfinding algorithm
    //!!!!!!!!!!!!!!!
//    int xGoal,yGoal,xPosi,yPosi,relX,relY;
    cPathKnot* pathObj;
    cPathKnot* newObj;
    cOpenListItem* newOpenListItem;
    cOpenListItem* openListItem;
    int k;

    cTile* checkedContentTile;

    //TESTOUTPUT
    //Error("In findPath1");

    //Clear the List TODO more ellegantly
    while(pPathListHead->getSuccessor()!=NULL)
    {
        //TESTOUTPUT
        //Error("In findPath loop1");
        pathObj = pPathListHead->getSuccessor();
        while(pathObj->getSuccessor() != NULL)
        {
            pathObj = pathObj->getSuccessor();
        }
        pathObj->getPredecessor()->setSucessor(NULL);
        pathObj->setPredecessor(NULL);
        delete pathObj;
    }

    //TESTOUTPUT
    //Error("In findPath2");

    //Init the lists
    closedList->Init();
    openList->Init(posi);
    openListArray->Init(posi);

    //create the first pathKnot on the goal position
    newObj = new cPathKnot();
    newObj->InitKnot(goal,NULL,NULL);

    //Add to open list
    newOpenListItem = openListArray->Add(newObj,0);
    openList->Add(newOpenListItem);

    //Error("After initializing lists");

    //FOR SATETY
    k=0;
    while(k<10000)
    {
        //Error("Main pathfindloop");
        //get the best candidate from the open list
        openListItem = openList->getPriority();

        //relax it
        expandKnot(openListItem);
        //check if goal was reached
        pathObj = openListItem->getContain();
        openListArray->Remove(openListItem);

        checkedContentTile = pathObj->getContain();

        //Ending condition
        if(checkedContentTile == posi)
        {
            if(pathObj->getPredecessor()==NULL){break;}
            pathObj=pathObj->getPredecessor();
            int l = 1;
            while(l < 10000)
            {
                //Error2("Ending List run:",l);
                //Error2("x-Coord :",pathObj->getContain()->getX());
                //Error2("y-Coord :",pathObj->getContain()->getY());
                if(pathObj->getPredecessor() == NULL)
                {
                    //Connect it to the list
                    pPathListHead->setSucessor(pathObj);
                    pathObj->setPredecessor(pPathListHead);
                    break;
                }
                newObj = pathObj->getPredecessor();
                newObj->setSucessor(pathObj);
                pathObj = newObj;
                l++;
            }
            //End the list
            break;
        }
    }
    //Error("after pathfind retrieval");

//    openListArray->CleanUp();
    openList->Clear();
    closedList->CleanUp();


    //Error("End pathfind");
}

void cPathFind::expandKnot(cOpenListItem* candidate)
{
//    Error("Starting expand knot");
    int posX,posY;
    float thisG,nextG;
    cTile* neighbourTile;
    cOpenListItem* newItem;
    cPathKnot* newKnot;
    cPathKnot* candidateKnot;
    candidateKnot = candidate->getContain();
    thisG = candidate->getG();
    posX = candidateKnot->getContain()->getX();
    posY = candidateKnot->getContain()->getY();

    //Error("expand knot x", posX);
    //Error("expand knot y", posY);

    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            //do not consider the pixel itself
            if(i==0 && j==0){continue;}
            //do not consider pixel outside the map
            if(posX+i < 0 || posX+i >= WORLD_WIDTH || posY+j < 0 || posY+j >=WORLD_HEIGHT){continue;}

            //store the neighbouring tile
            neighbourTile = pWorldMap->getMap(posX+i,posY+j);
            //Error("analyzing tile x:",posX+i);
            //Error("analyzing tile y:",posY+j);

            //is it on the closed list; proceed only if it is not so
            if(closedList->Check(neighbourTile)==false)
            {
                //Is it on the open List => update TODO!!!!
                if(openListArray->Check(posX+i,posY+j)==true)
                {
                    newItem = openListArray->getItem(posX+i,posY+j);
                    if(newItem == NULL){Error2("openList array getItem is not definde");break;}
                    if(i!=0 && j!=0){nextG = 1.414235623;}
                    else            {nextG = 1;}
                    nextG += getTilePotential(newItem->getContain()->getContain());
                    nextG += thisG;
                    //Add it to the open List
                    if(openList->Update(newItem,nextG)==true)
                    {
                        //Error("updating pointers");
                        newItem->getContain()->setPredecessor(candidateKnot);
//                        Error("newKnot1 x:",newKnot->getContain()->getX() );
//                        Error("newKnot1 y:",newKnot->getContain()->getY() );
                        //candidate->setSucessor(newKnot);
                    }
                }
                //Is it new, add to openLIst
                else
                {
                    //Error("not in open list");
                    //Create a knew path knot in the list !!! Attention the list are only coupeled towards their predecessors !!!
                    newKnot = new cPathKnot;
                    newKnot->InitKnot(neighbourTile,NULL,candidateKnot);
//                    Error("newKnot2 x:",neighbourTile->getX() );
//                    Error("newKnot2 y:",neighbourTile->getY() );
                    //candidate->setSucessor(newKnot);
                    //calculate the next g value
                    if(i!=0 && j!=0){nextG = 1.414235623;}
                    else            {nextG = 1;}
                    //consider blockage
                    nextG += getTilePotential(newKnot->getContain());
                    nextG += thisG;

                    //Error("check");
                    //Add it to the open List
                    newItem = openListArray->Add(newKnot,nextG);
                    openList->Add(newItem);
                }
            }
        }

    }

    //Error("done expanding");
    //Add it to the closed List
    closedList->Add(candidateKnot,thisG);

/*  Wird ein Knoten "candidate" abschließend  expandiert,
    so werden seine Nachfolgeknoten in die Open List eingefügt und "candidate" in die Closed List aufgenommen.
    Die Vorgängerzeiger der Nachfolgeknoten werden auf "candidate" gesetzt.
    Ist ein Nachfolgeknoten bereits auf der Closed List,
    so wird er nicht erneut in die Open List eingefügt und auch sein Vorgängerzeiger nicht geändert.
    Ist ein Nachfolgeknoten bereits auf der Open List,
    so wird der Knoten nur aktualisiert (f-Wert und Vorgängerzeiger), wenn der neue Weg dorthin kürzer ist als der bisherige.*/
}

float cPathFind::getTilePotential(cTile* checkTile)
{
    float potential = 0;
    //consider blockage
    switch(checkTile->getContainType())
    {
        //No content, free field
        case 0:
        {
            break;
        }
        //Content is a Unit;
        case OBJ_TYPE_BUILDING_WITH_UNIT:
        case OBJ_TYPE_UNIT:
        {
            //add a big potential for resting units; but a lower one for moving units
            if(checkTile->getContainUnit()->getActionType() == (ACTION_REST && ACTION_WAIT))
            {
                potential += 100;
            }
            else
            {
                potential += 4;
            }
            break;
        }
        //Content is an Obstacle
        case OBJ_TYPE_OBSTACLE:
        {
            //add an "infinite" potential
            potential += 10000;
            break;
        }
        case OBJ_TYPE_BUILDING_WALL:
        {
            potential += 1000;
            break;
        }
    }
    return potential;
}

//Check Path identiefiers:
/*
negative return values indicate real bugs; positive ones only special types of obstructions of the path (part of collision detecition)
return:|   meaning(you can move when return == (0 || 1)
-------------------------------
-2     |   the pathlist is empty
-1     |   the pathlist does not start with the movementgoal
 0     |   the path is completely unobstructed
 1     |   the path is obstructed but not on last item in list
 2     |   the path is in the last entry obstructed by a Unit
 3     |   the path is in the last entry obstructed by an Obstacle
*/
int cPathFind::checkPath(cPathKnot* pPathListHead,cTile* goal,cTile* position)
{
    int pathFree;
    pathFree = 0;

    cPathKnot* pathObj;
    pathObj = pPathListHead;
    pathObj = pathObj->getSuccessor();

    //Check for more serious problems
    if(pathObj == NULL){/*Error2("Checkpath listHeadError");*/return -2;}
    if(pathObj->getContain() != goal){/*Error2("Checkpath Containerror");*/return -1;}

    //Cycle through list
    while(pathObj->getSuccessor() != NULL)
    {
        //Check if the path is blocked
        if(pathObj->getContain()->getContainType() != 0){pathFree = 1;}
        pathObj = pathObj->getSuccessor();
    }
    //Check last object (then one that is beeing moved to)
    if(pathObj->getContain()->getContainType() == OBJ_TYPE_UNIT){pathFree = 2;}
    if(pathObj->getContain()->getContainType() == OBJ_TYPE_BUILDING_WITH_UNIT){pathFree = 2;}
    if(pathObj->getContain()->getContainType() == OBJ_TYPE_OBSTACLE){pathFree = 3;}
    if(pathObj->getContain()->getContainType() == OBJ_TYPE_BUILDING_WALL){pathFree = 3;}

    //Check if last in List is next to the position
    //abbreviations
    int relX,relY;
    relX = pathObj->getContain()->getX() - position->getX();
    relY = pathObj->getContain()->getY() - position->getY();
    if(1 >= relX && relX >= -1 && 1 >= relY && relY >= -1)
    {
        //Error2("pathFree: ",pathFree);
        return pathFree;
    }
    else
    {
        //Error2("End ");
        return -1;
    }
}
