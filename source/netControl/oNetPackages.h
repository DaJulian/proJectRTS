#ifndef ONETPACKAGES_H_INCLUDED
#define ONETPACKAGES_H_INCLUDED

#include "../global.h"
#include "../tools.h"

//struct NetPackageCore
//{
//    char content[NET_CORESIZE];
//};

struct NetPackage
{
    unsigned int packID;
    int owner;
    int resendtime;
    char packageCore[NET_CORESIZE];
};

struct NetPackageListItem
{
    NetPackage* contain;
    NetPackageListItem* sucessor;
};

class cNetPackageList
{
protected:
    NetPackageListItem* pListHead;
public:
    cNetPackageList();

    void Add(NetPackage* toAdd);
    int RemoveID(unsigned int ID);
    NetPackage* getLast();
    NetPackage* getFirst();

    NetPackage* checkTimer(int t,int* numberInList);
};

#endif // ONETPACKAGES_H_INCLUDED
