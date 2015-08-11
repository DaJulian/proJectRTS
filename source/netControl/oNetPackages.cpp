#include "oNetPackages.h"

cNetPackageList::cNetPackageList()
{
    pListHead = NULL;
}

void cNetPackageList::Add(NetPackage* toAdd)
{
    NetPackageListItem* Item;
    Item = pListHead;
    if(Item!=NULL)
    {
        while(Item->sucessor != NULL)
        {
            Item = Item->sucessor;
        }
        Item->sucessor = new NetPackageListItem;
        Item->sucessor->contain = toAdd;
        Item->sucessor->sucessor = NULL;
    }
    else
    {
        pListHead = new NetPackageListItem;
        pListHead->contain = toAdd;
        pListHead->sucessor = NULL;
    }

}

//0 means not found; 1 means has been removed
int cNetPackageList::RemoveID(unsigned int ID)
{
    NetPackageListItem* Item;
    NetPackageListItem* deleter;
    Item = pListHead;
    if(Item!=NULL)
    {
        if(Item->contain->packID == ID)
        {
            deleter = Item;
            Error("Item sucessor");
            pListHead = Item->sucessor;
            delete deleter;
            return 1;
        }
        while(Item->sucessor != NULL)
        {
            if(Item->sucessor->contain->packID == ID)
            {
                deleter = Item->sucessor;
                Item->sucessor = deleter->sucessor;
                delete deleter;
                return 1;
            }
            Item = Item->sucessor;
        }

    }
    else
    {
        return 0;
    }
    return 0;
}

NetPackage* cNetPackageList::getFirst()
{
    ///DEBUGING
    Error("NetPackage getFirst");
    NetPackageListItem* Item;
    NetPackage* returner;
    returner = NULL;
    if(pListHead != NULL)
    {
        returner = pListHead->contain;
        Item = pListHead->sucessor;
        delete pListHead;
        pListHead = Item;
    }
    ///DEBUGING
    Error("NetPackage getFirst ends");

    return returner;
}
NetPackage* cNetPackageList::getLast()
{
    NetPackageListItem* Item;
    Item = pListHead;
    if(Item!=NULL)
    {
        while(Item->sucessor != NULL)
        {
            Item = Item->sucessor;
        }
        return Item->contain;
    }
    else
    {
        return NULL;
    }
}

NetPackage* cNetPackageList::checkTimer(int t,int* numberInList)
{
    NetPackageListItem* Item;
    Item = pListHead;
    for(int i=1;i<*numberInList;i++)
    {
        if(Item->sucessor != NULL)
        {
            Item = Item->sucessor;
        }
        else
        {
            return NULL;
        }
    }

    while(Item != NULL)
    {
        *numberInList++;
        Item->contain->resendtime -= t;
        if(Item->contain->resendtime < 0)
        {
            Item->contain->resendtime = RESENDINGSPEED;
        }
        return Item->contain;
    }
    return NULL;
}
