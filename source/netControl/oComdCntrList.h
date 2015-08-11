#ifndef OCOMDCNTRLIST_H_INCLUDED
#define OCOMDCNTRLIST_H_INCLUDED

#include "../global.h"
#include "../model/cUnit.h"

class cUnit;

struct Change
{
    unsigned int UnitID;
    int changeType;
    //This is not sent, but it is easier to access the Unit on your local machine with this adress
    cUnit* CommandedUnit;
    int posiX;
    int posiY;
    int goalX;
    int goalY;
    unsigned int goalUnitID;
    int moveType;
    int health;
    Uint32 time;
};

struct ChangeListItem
{
    Change* contain;
    ChangeListItem* next;
};

class cChangeList
{
protected:
    ChangeListItem* pListHead;
public:
    cChangeList();
    void Add(Change* toAdd);
    Change* getFirst();
};

struct Command
{
    unsigned int UnitID;
    int commandType;
    //This is not sent, but it is easier to access the Unit on your local machine with this adress
    cUnit* CommandedUnit;
    int posiX;
    int posiY;
    int goalX;
    int goalY;
    int priority;
    Uint32 time;
};

struct CommandListItem
{
    Command* contain;
    CommandListItem* next;
};

class cCommandList
{
protected:
    CommandListItem* pListHead;
public:
    cCommandList();
    void Add(Command* toAdd);
    Command* getFirst();
};

#endif // OCOMDCNTRLIST_H_INCLUDED
