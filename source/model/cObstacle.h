#ifndef COBSTACLE_H_INCLUDED
#define COBSTACLE_H_INCLUDED

#include "cObject.h"

class cObstacle : public cObject
{
protected:
public:
    cObstacle();
    cObstacle(int typ,cTile* Posi);
};

#endif // COBSTACLE_H_INCLUDED
