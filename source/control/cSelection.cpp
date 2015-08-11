#include "cSelection.h"

cSelection::cSelection()
{
    pContent = NULL;
    pLast = NULL;
    pNext = NULL;
}

cSelection::cSelection(cUnit* cont,cSelection* suc,cSelection* pre)
{
    pContent = cont;
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

void cSelection::Destroy()
{
    pContent = NULL;
    pNext = NULL;
    pLast = NULL;
}

cSelection::~cSelection()
{

}
