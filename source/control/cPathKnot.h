#ifndef CPATHKNOT_H_INCLUDED
#define CPATHKNOT_H_INCLUDED

#include "../global.h"
#include "../tools.h"
#include "../model/cWorldMap.h"

class cTile;
class cWorldMap;

class cPathKnot
{
protected:
    cTile* contain;
    cPathKnot* pNext;
    cPathKnot* pLast;
public:
    cPathKnot();

    void InitKnot(cTile* con,cPathKnot* next, cPathKnot* last);

    cTile* getContain();
    void setContain(cTile* cont);

    cPathKnot* getSuccessor();
    void setSucessor(cPathKnot* knot);

    cPathKnot* getPredecessor();
    void setPredecessor(cPathKnot* knot);
};


class cOpenListItem
{
protected:
    float fValue;
    float gValue;
    int posiInOpenList;
    cPathKnot* pContain;
public:
    cOpenListItem();
    cOpenListItem(float f,float g,cPathKnot* con,int posi);

    float getF();
    void setF(float f);

    float getG();
    void setG(float g);

    int getPosiInOpenList();
    void setPosiInOpenList(int posi);

    cPathKnot* getContain();
    void setContain(cPathKnot* cont);
};

class cOpenListArray
{
protected:
    cTile* pGoal;
    cOpenListItem* pArray[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];
public:
    void Init(cTile* goal);

    cOpenListItem* getItem(int x,int y);
    bool Check(int x,int y);
    cOpenListItem* Add(cPathKnot* Contain,float newG);
    void Remove(cOpenListItem* toRemove);

    void CleanUp();
};

class cOpenList
{
protected:
    cOpenListItem* HeapArray[MAX_WORLD_HEIGHT*MAX_WORLD_WIDTH+1];
    cTile* pGoal;
    int containedNumber;
public:
    void Init(cTile* goal);
    void Clear();

    void Add(cOpenListItem* candidate);

    bool Update(cOpenListItem* check,float newG);

    bool siftUp(cOpenListItem* candidate);
    bool siftDown(cOpenListItem* candidate);

    cOpenListItem* getPriority();
};

class cClosedList
{
protected:
    cPathKnot* Members[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];
    //float fValues[WORLD_WIDTH][WORLD_HEIGHT];
    float gValues[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];
public:
    void Init();

    void Add(cPathKnot* candidate,float newG);
    bool Check(cTile* candidate);

    void CleanUp();
};

#endif // CPATHKNOT_H_INCLUDED
