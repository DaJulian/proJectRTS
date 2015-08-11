#include "oComdCntrList.h"

cChangeList::cChangeList()
{
    pListHead = NULL;
}

void cChangeList::Add(Change* toAdd)
{
    ChangeListItem* Item;
    Item = pListHead;
    if(Item != NULL)
    {
        while(Item->next != NULL)
        {
            Item = Item->next;
        }
        Item->next = new ChangeListItem;
        Item->next->contain = toAdd;
        Item->next->next = NULL;
    }
    else
    {
        pListHead = new ChangeListItem;
        pListHead->contain = toAdd;
        pListHead->next = NULL;
    }

}
Change* cChangeList::getFirst()
{
    ChangeListItem* Item;
    Change* returner;
    returner = NULL;
    if(pListHead != NULL)
    {
        returner = pListHead->contain;
        Item = pListHead->next;
        delete pListHead;
        pListHead = Item;
    }

    return returner;
}

cCommandList::cCommandList()
{
    pListHead = NULL;
}

void cCommandList::Add(Command* toAdd)
{
    CommandListItem* Item;
    Item = pListHead;
    if(Item != NULL)
    {
        while(Item->next != NULL)
        {
            Item = Item->next;
        }
        Item->next = new CommandListItem;
        Item->next->contain = toAdd;
        Item->next->next = NULL;
    }
    else
    {
        pListHead = new CommandListItem;
        pListHead->contain = toAdd;
        pListHead->next = NULL;
    }

}
Command* cCommandList::getFirst()
{
    CommandListItem* Item;
    Command* returner;
    returner = NULL;
    if(pListHead != NULL)
    {
        returner = pListHead->contain;
        Item = pListHead->next;
        delete pListHead;
        pListHead = Item;
    }

    return returner;
}
