#ifndef CPACKAGESENDER_H_INCLUDED
#define CPACKAGESENDER_H_INCLUDED

#include "../global.h"
#include "../tools.h"
#include "oNetPackages.h"

class cPackageSender
{
protected:


    //cNetPackageList* toSend;
    //cNetPackegeList* toUnpack;
public:
    virtual int InitNet(int sock,short int Owner);


};

#endif // CPACKAGESENDER_H_INCLUDED
