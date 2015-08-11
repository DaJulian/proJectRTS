#include "cObstacle.h"

cObstacle::cObstacle()
{
    //Object properties
    type = 0;
    position = NULL;
    positionX = 0;
    positionY = 0;
//    actionType = ACTION_REST;
}

cObstacle::cObstacle(int typ,cTile* Posi)
{
    //Object properties
    type = typ;
    position = Posi;
    positionX = 0;
    positionY = 0;
//    actionType = ACTION_REST;
}
