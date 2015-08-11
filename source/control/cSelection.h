#ifndef CSELECTION_H_INCLUDED
#define CSELECTION_H_INCLUDED

#include "../model/cUnit.h"
#include "../model/cTile.h"

class cSelection
{
protected:
    //Selected Unit
    cUnit* pContent;

    //Next and Last in List
    cSelection* pNext;
    cSelection* pLast;
public:
    cSelection();
    cSelection(cUnit* con,cSelection* suc,cSelection* pre);
    ~cSelection();

    cSelection* getPredecessor();
    void setPredecessor(cSelection* pre);

    cSelection* getSuccessor();
    void setSucessor(cSelection* suc);

    cUnit* getContent();
    void setContent(cUnit* un);

    void Destroy();
};

#endif // CSELECTION_H_INCLUDED
