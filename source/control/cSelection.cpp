#include "cSelection.h"

cSelection::cSelection()
{
    pContent = NULL;
    pBuildingContent = NULL;
    pLast = NULL;
    pNext = NULL;
}

cSelection::cSelection(cUnit* cont, cBuilding* contbui,cSelection* suc,cSelection* pre)
{
    pContent = cont;
    pBuildingContent = contbui;
    pLast = pre;
    pNext = suc;
}

cUnit* cSelection::getContent()
{
    return pContent;
}

void cSelection::setContent(cUnit* cont)
{
    pContent = cont;
}

cSelection* cSelection::getSuccessor()
{
    return pNext;
}

void cSelection::setSucessor(cSelection* suc)
{
    pNext = suc;
}

cSelection* cSelection::getPredecessor()
{
    return pLast;
}

void cSelection::setPredecessor(cSelection* pre)
{
    pLast = pre;
}

cBuilding* cSelection::getBuildingContent()
{
    return pBuildingContent;
}
void cSelection::setBuildingContent(cBuilding* bui)
{
    pBuildingContent = bui;
}

void cSelection::Destroy()
{
    pContent = NULL;
    pNext = NULL;
    pLast = NULL;
}

cSelection::~cSelection()
{

}
