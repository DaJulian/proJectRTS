#ifndef CSELECTION_H_INCLUDED
#define CSELECTION_H_INCLUDED

#include "../model/cUnit.h"
#include "../model/cBuilding.h"
#include "../model/cTile.h"

class cSelection
{
protected:
    //Selected Unit
    cUnit* pContent;
    cBuilding* pBuildingContent;

    //Next and Last in List
    cSelection* pNext;
    cSelection* pLast;
public:
    cSelection();
    cSelection(cUnit* con, cBuilding* buicon,cSelection* suc,cSelection* pre);
    ~cSelection();

    cSelection* getPredecessor();
    void setPredecessor(cSelection* pre);

    cSelection* getSuccessor();
    void setSucessor(cSelection* suc);

    cUnit* getContent();
    void setContent(cUnit* un);

    cBuilding* getBuildingContent();
    void setBuildingContent(cBuilding* bui);

    void Destroy();
};

#endif // CSELECTION_H_INCLUDED
