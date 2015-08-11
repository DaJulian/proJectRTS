#include "cObject.h"

int cObject::getType()
{
    return type;
}
void cObject::setType(int typ)
{
    type = typ;
}

cObject::cObject()
{
    type = 0;
    positionX = 0;
    positionY = 0;
    position = NULL;
}
cObject::cObject(int typ,int x,int y)
{
    type = typ;
    positionX = x;
    positionY = y;
    position = NULL;
}

cTile* cObject::getPosition()
{
    return position;
}

void cObject::setPosition(cTile* pos)
{
    position = pos;
}


int cObject::getPositionX()
{
    return positionX;
}

int cObject::getPositionY()
{
    return positionY;
}

void cObject::SetPosition(int x,int y)
{
    positionX = x;
    positionY = y;
}
