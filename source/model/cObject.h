#ifndef COBJECT_H_INCLUDED
#define COBJECT_H_INCLUDED

#include "../global.h"

class cTile;
class cPathKnot;
class cPathFind;
class cWorldMap;

class cObject
{
protected:
    int type;

    int positionX,positionY;
    cTile* position;
public:
    cObject();
    cObject(int typ,int x,int y);

    int getType();
    void setType(int typ);


    int getPositionX();
    int getPositionY();
    void SetPosition(int x,int y);

    cTile* getPosition();
    void setPosition(cTile* pos);
};

#endif // COBJECT_H_INCLUDED
