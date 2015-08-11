#include "cPathKnot.h"

cPathKnot::cPathKnot()
{
    contain = NULL;
    pNext = NULL;
    pLast = NULL;
}

void cPathKnot::InitKnot(cTile* con,cPathKnot* next, cPathKnot* last)
{
    contain = con;
    pNext = next;
    pLast = last;
}

cTile* cPathKnot::getContain()
{
    return contain;
}
void cPathKnot::setContain(cTile* cont)
{
    contain = cont;
}

cPathKnot* cPathKnot::getSuccessor()
{
    return pNext;
}
void cPathKnot::setSucessor(cPathKnot* knot)
{
    pNext = knot;
}

cPathKnot* cPathKnot::getPredecessor()
{
    return pLast;
}
void cPathKnot::setPredecessor(cPathKnot* knot)
{
    pLast = knot;
}


cOpenListItem::cOpenListItem()
{
    fValue = 0;
    gValue = 0;
    pContain = NULL;
    posiInOpenList = 0;
}
cOpenListItem::cOpenListItem(float f,float g,cPathKnot* con,int posi)
{
    fValue = f;
    gValue = g;
    pContain = con;
    posiInOpenList = posi;
}

float cOpenListItem::getF()
{
    return fValue;
}
void cOpenListItem::setF(float f)
{
    fValue = f;
}

float cOpenListItem::getG()
{
    return gValue;
}
void cOpenListItem::setG(float g)
{
    gValue = g;
}


cPathKnot* cOpenListItem::getContain()
{
    return pContain;
}
void cOpenListItem::setContain(cPathKnot* cont)
{
    pContain = cont;
}

int cOpenListItem::getPosiInOpenList()
{
    return posiInOpenList;
}

void cOpenListItem::setPosiInOpenList(int posi)
{
    posiInOpenList = posi;
}



//    openListItem* pArray[WORLD_WIDTH][WORLD_HEIGHT];

void cOpenListArray::Init(cTile* goal)
{
    //Pathfining goal in order to calculate the hValue
    pGoal = goal;
    //all pointers in every playce to NULL
    for(int i=0;i<WORLD_HEIGHT;i++)
    {
        for(int j=0;j<WORLD_WIDTH;j++)
        {
            pArray[j][i] = NULL;
        }
    }
}

cOpenListItem* cOpenListArray::getItem(int x,int y)
{
    return pArray[x][y];
}
bool cOpenListArray::Check(int x,int y)
{
    if(pArray[x][y]==NULL){/*Error("openListArray check false");*/return false;}
    //Error("openListArray check true");
    return true;
}
cOpenListItem* cOpenListArray::Add(cPathKnot* candidate,float g)
{
    //Error("Adding Item to open List array");
    int relX,relY;
    float fValue,hValue;
    cTile* thisTile;
    thisTile = candidate->getContain();
    int x,y;
    x = thisTile->getX();
    y = thisTile->getY();
    //Calculate the h-Value(shortest distance)
    relX =  abs(x - pGoal->getX());
    relY =  abs(y - pGoal->getY());
    if(relX>=relY)
    {
        hValue = relX - relY + relY*1.414235623;
    }
    else
    {
        hValue = relY - relX + relX*1.414235623;
    }
/*    relX = x - pGoal->getX();
    relX = relX*relX;
    relY = y - pGoal->getY();
    relY = relY*relY;
    hValue = sqrt(relX + relY);*/

    if(hValue == 0){g=0;}
    fValue = g + hValue;

    ///DEBUG CHECK
    //Error("Add array check");

    pArray[x][y] = new cOpenListItem(fValue,g,candidate,-1);

    return pArray[x][y];
}

void cOpenListArray::Remove(cOpenListItem* toRemove)
{
    int x,y;
    x = toRemove->getContain()->getContain()->getX();
    y = toRemove->getContain()->getContain()->getY();
    delete pArray[x][y];
    pArray[x][y] = NULL;
}

void cOpenListArray::CleanUp()
{
    //Error("Clening up the open List Array");
    pGoal = NULL;
    for(int i=0;i<WORLD_HEIGHT;i++)
    {
        for(int j=0;j<WORLD_WIDTH;j++)
        {
            if(pArray[j][i]!=NULL){delete pArray[j][i];}
            pArray[j][i] = NULL;
        }
    }
     //Error("Done Clening up the open List Array");
}




void cOpenList::Init(cTile* goal)
{
    //Error("Initializing openList");
    //create the list as pointers of NULL
    for(int i=0;i<=WORLD_HEIGHT*WORLD_WIDTH;i++)
    {
        HeapArray[i] = NULL;
    }
    //The goal must be know to calculate the fValue
    pGoal = goal;
    //No elements in the list
    containedNumber = 0;
}
void cOpenList::Clear()
{
    //Error("clearing OpenList");
    for(int i=0;i<=WORLD_HEIGHT*WORLD_WIDTH;i++)
    {
        //Error("i : ",i);
        if(HeapArray[i]!=NULL){delete HeapArray[i];}
        HeapArray[i] = NULL;
    }
    //Error("done clearing open List");
    pGoal = NULL;
}

void cOpenList::Add(cOpenListItem* candidate)
{

    //Error("Adding Item to open list");
    //increase Number of Objects in the list
    containedNumber++;
    candidate->setPosiInOpenList(containedNumber);
    if(HeapArray[containedNumber]!=NULL){Error2("nonzero item adding somehting to openList");}
    HeapArray[containedNumber] = candidate;

    //sift it up until at right place
    while(siftUp(candidate));

    //Error("Adding item to open List is done");
}

bool cOpenList::Update(cOpenListItem* check,float newG)
{
    //Error2("Updating List newG:",newG);

    if(check->getG() > newG)
    {
        //Change F and G values
        check->setF(check->getF()-check->getG()+newG);
        check->setG(newG);

        //Check for right place in list and update it

        while(siftUp(check));

        //Error2("return true");
        return true;
    }
    else
    {
        //Error("return false");
        return false;
    }

}

cOpenListItem* cOpenList::getPriority()
{
    //Error("starting getPriority");
    cOpenListItem* pReturnValue;
    cOpenListItem* sifter;

    ///DEBUG CHECK
    //Error("getPriority Check 1");

    //get the first element from the list and store it
    pReturnValue = HeapArray[1];
    //put the last element into the first place
    HeapArray[1] = HeapArray[containedNumber];
    ///Check [up to now only problematic when removing last item from list
    //if(pReturnValue==HeapArray[1]){Error2("problems with the arrays in getPriority");}

    HeapArray[1]->setPosiInOpenList(1);
    //change total number in list
    HeapArray[containedNumber] = NULL;
    containedNumber--;

    ///DEBUG CHECK
    //Error("getPriority Check 2");

    //sift down this element
    sifter = HeapArray[1];
    if(sifter!=NULL)
    {
        while(siftDown(sifter));
    }

    ///DEBUG CHECK
    //Error2("get priority ",pReturnValue->getF());

    //return the first element
    return pReturnValue;
}

bool cOpenList::siftDown(cOpenListItem* candidate)
{
    ///DEBUG CHECK
    //Error("sift down starting");
    int candidPos,twoCandidPos;
    candidPos = candidate->getPosiInOpenList();
    //Error("posi in open List",candidPos);
    twoCandidPos = 2*candidPos;
    cOpenListItem* child;


    //already in the last tree
    if(twoCandidPos>containedNumber || twoCandidPos>WORLD_HEIGHT*WORLD_WIDTH){/*Error("return false 1");*/return false;}

    //Check which child is the smaller one if
    //both child-knots are available
    if(twoCandidPos < containedNumber)
    {
        if(HeapArray[twoCandidPos+1]->getF() < HeapArray[twoCandidPos]->getF())
        {
            twoCandidPos++;
        }
    }
    //store the cild item
    child = HeapArray[twoCandidPos];

    //Check the child item
    //if its f value is smaller then the candidates one; swap them
    if (child->getF() < candidate->getF())
    {
        ///DEBUG CHECK
        //Error("sift down going online");
        //swapping
        child->setPosiInOpenList(candidPos);
        candidate->setPosiInOpenList(twoCandidPos);
        HeapArray[twoCandidPos]=candidate;
        HeapArray[candidPos]=child;
        return true;
    }

    ///DEBUG CHECK
    //Error("return in the end");
    return false;
}

bool cOpenList::siftUp(cOpenListItem* candidate)
{
    ///DEBUG CHECK
        //Error("sift up starting");
    int candidPos,halfCandidPos;
    cOpenListItem* parent;
    candidPos = candidate->getPosiInOpenList();
    //already in prime place
    if(candidPos==1){/*Error("candid pos == 1");Error("return false mid");*/return false;}
    halfCandidPos=candidPos/2;
    //Error("candidPos" ,candidPos);
    //Error("halfcandidPos" , halfCandidPos);

    parent=HeapArray[halfCandidPos];

    //Check the fValues; swap if nececarry

    if(parent->getF() > candidate->getF())
    {
        parent->setPosiInOpenList(candidPos);
        candidate->setPosiInOpenList(halfCandidPos);
        HeapArray[candidPos]=parent;
        HeapArray[halfCandidPos]=candidate;
        return true;
    }
    //Error("return false end");
    return false;
}

/*
    cPathKnot* Members[WORLD_WIDTH][WORLD_HEIGHT];
    float fValues[WORLD_WIDTH][WORLD_HEIGHT];
    float gValues[WORLD_WIDTH][WORLD_HEIGHT];*/

void cClosedList::Init()
{
    //Error("Initializing closed List");
    for(int i=0;i<WORLD_WIDTH;i++)
    {
        for(int j=0;j<WORLD_HEIGHT;j++)
        {
            Members[i][j] = NULL;
            //fValues[i][j] = 0;
            gValues[i][j] = 0;
        }
    }
}

void cClosedList::Add(cPathKnot* candidate,float newG)
{
    //Error("closed List Add");
    cTile* candidTile;
    candidTile= candidate->getContain();
    Members[candidTile->getX()][candidTile->getY()] = candidate;
    gValues[candidTile->getX()][candidTile->getY()] = newG;
    //Error("X",candidTile->getX());
    //Error("Y",candidTile->getY());
    //Error("closed List Add done");
}
bool cClosedList::Check(cTile* candidate)
{
//    Error("closed List Check x:",candidate->getX());
//    Error("closed List Check y:",candidate->getY());
    if (Members[candidate->getX()][candidate->getY()] != NULL){/*Error("is in closed list");*/return true;}
    //Error("is not in closed list");
    return false;
}

void cClosedList::CleanUp()
{
    for(int i=0;i<WORLD_WIDTH;i++)
    {
        for(int j=0;j<WORLD_HEIGHT;j++)
        {
            Members[i][j] = NULL;
            //fValues[i][j] = 0;
            gValues[i][j] = 0;
        }
    }
}

