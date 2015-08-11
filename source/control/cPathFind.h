#ifndef CPATHFIND_H_INCLUDED
#define CPATHFIND_H_INCLUDED

#include "cPathKnot.h"

class cWorldMap;
class cOpenListItem;
class cOpenList;
class cOpenListArray;
class cClosedList;

class cPathFind
{
protected:
    cWorldMap* pWorldMap;
    cClosedList* closedList;
    cOpenList* openList;
    cOpenListArray* openListArray;
public:
    void Init();
    void setMap(cWorldMap* Map);
    void findPath(cPathKnot* pPathListHead,cTile* goal,cTile* posi);
    int checkPath(cPathKnot* pPathListHead,cTile* goal,cTile* position);

    //Methods for pathfinding
    void expandKnot(cOpenListItem* candidate);
    float getTilePotential(cTile* checkTile);
};

#endif // CPATHFIND_H_INCLUDED
