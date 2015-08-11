#include "cSelectionControl.h"

cSelectionControl::cSelectionControl()
{
    for(int i=0;i<=10;i++)
    {
        pListHead[i] = new cSelection();
        selectedNumber[i] = 0;
        priorizedNumber[i] = 1;
        selectedTypes[i] = 1;
    }

}

bool cSelectionControl::AddSelection(cUnit* Sel,int group)
{
    //Working pointers
    cSelection* newSelection;
    cSelection* LastInList;

    //cycle until end of list
    LastInList = pListHead[group];
    for(int i=1;i<=selectedNumber[group];i++)
    {
        LastInList = LastInList->getSuccessor();
        //Element should not be 0
        if(LastInList==NULL){Error("Selection Error");return false;}
    }

    //Create a new List element
    newSelection = new cSelection(Sel,NULL,LastInList);
    LastInList->setSucessor(newSelection);

    //increase list Number
    selectedNumber[group]++;

    if(group ==0)Sel->setSelected(true);

    return 1;
}

bool cSelectionControl::RemoveSelection(cUnit* Sel,int group)
{
    //Working pointers
    cSelection* BeforeEntry;
    cSelection* AfterEntry;
    cSelection* toDelete;

    //cycle until Item that will be deleted
    BeforeEntry = pListHead[group];
    while(true)
    {
        //take sucessor
        toDelete = BeforeEntry->getSuccessor();
        if(toDelete == NULL){Error("UnSelection Error");return false;}
        //check if it is the searched item
        if(toDelete->getContent() == Sel)
        {
            AfterEntry = toDelete->getSuccessor();
            break;
        }

        //jump to the next one
        BeforeEntry = toDelete;
    }

    //close List again
    BeforeEntry->setSucessor(AfterEntry);
    if(AfterEntry != NULL){AfterEntry->setPredecessor(BeforeEntry);}

    //Remove toDelete
    toDelete->Destroy();
    delete toDelete;

    //decrease list Number
    selectedNumber[group]--;

    if(group == 0){Sel->setSelected(false);}

    return 1;
}

bool cSelectionControl::AddSelectionByTile(cTile* Tile,int owner,int group)
{
    //Working pointers
    cSelection* newSelection;
    cSelection* LastInList;

    if(Tile->getContainUnit()!=NULL)
    {
        if(Tile->getContainUnit()->getOwner() == owner)
        {
            //cycle until end of list
            LastInList = pListHead[group];
            for(int i=1;i<=selectedNumber[group];i++)
            {
                LastInList = LastInList->getSuccessor();
                //Element should not be 0
                if(LastInList==NULL){Error("Selection Error");return false;}
            }

            //Create a new List element
            newSelection = new cSelection(Tile->getContainUnit(),NULL,LastInList);
            LastInList->setSucessor(newSelection);

            //increase list Number
            selectedNumber[group]++;

            //TEST
            if(group == 0){Tile->getContainUnit()->setSelected(true);}
        }
    }

    //Error("Added a selection by Tile");

    return 1;
}

bool cSelectionControl::RemoveSelectionByTile(cTile* Tile,int group)
{
    //Working pointers
    cSelection* BeforeEntry;
    cSelection* AfterEntry;
    cSelection* toDelete;

    if(Tile->getContainUnit()!=NULL)
    {
        //cycle until Item that will be deleted
        BeforeEntry = pListHead[group];
        while(true)
        {
            //take sucessor
            toDelete = BeforeEntry->getSuccessor();
            if(toDelete == NULL){Error("UnSelection Error");return false;}
            //check if it is the searched item
            if(toDelete->getContent() == Tile->getContainUnit())
            {
                AfterEntry = toDelete->getSuccessor();
                break;
            }

            //jump to the next one
            BeforeEntry = toDelete;
        }

        //close List again
        BeforeEntry->setSucessor(AfterEntry);
        if(AfterEntry != NULL){AfterEntry->setPredecessor(BeforeEntry);}

        //Remove toDelete
        toDelete->Destroy();
        delete toDelete;

        //decrease list Number
        selectedNumber[group]--;
    }

    return 1;
}

bool cSelectionControl::ClearSelection(int group)
{
    //Working pointers
    cSelection* NextInList;
    cSelection* toDelete;
    //Start checking for existance
    NextInList = pListHead[group];
    toDelete = NextInList->getSuccessor();
    while(toDelete != NULL)
    {
        //TEST
        if(group == 0){toDelete->getContent()->setSelected(false);}
        //Store next Item
        NextInList=toDelete->getSuccessor();
        //Delete the Item
        toDelete->Destroy();
        delete toDelete;
        //take the stored as next
        toDelete = NextInList;
    }

    //Set List to be empty
    selectedNumber[group] = 0;
    pListHead[group]->setSucessor(NULL);
    priorizedNumber[group] = 1;
    selectedTypes[group] = 1;

    //TESTOUTPUT
    Error("ClearedSelection");

    return 1;
}

void cSelectionControl::copySelection(int sourceGroup,int targetGroup)
{
    cSelection* sel;
    ClearSelection(targetGroup);
    sel = pListHead[sourceGroup];
    for(int i=1;i <= selectedNumber[sourceGroup]; i++)
    {
        sel = sel->getSuccessor();
        AddSelection(sel->getContent(),targetGroup);
    }
    priorizedNumber[targetGroup] = priorizedNumber[sourceGroup];
    selectedTypes[targetGroup] = selectedTypes[sourceGroup];
}

void cSelectionControl::AddSelectionFromGroup(int sourceGroup,int targetGroup)
{
    cSelection* sel;
    sel = pListHead[sourceGroup];
    //add all elements
    for(int i=1;i <= selectedNumber[sourceGroup]; i++)
    {
        sel = sel->getSuccessor();
        RemoveSelection(sel->getContent(),targetGroup);
        AddSelection(sel->getContent(),targetGroup);
    }
}

void cSelectionControl::sortSelection(int group)
{
    if(selectedNumber[group] > 1)
    {
        //Create an array to work with from the list
        cSelection* sel[selectedNumber[group]+1];
        int order[selectedNumber[group]+1];
        int buf;
        sel[0] = pListHead[group];
        order[0] = 0;
        for(int i=1;i<=selectedNumber[group];i++)
        {
            sel[i] = sel[i-1]->getSuccessor();
            order[i] = i;
        }
        for(int i=2;i<=selectedNumber[group];i++)
        {
            for(int j=i;j>1;j--)
            {
                if(sel[order[j-1]]->getContent()->getSortPriority() < sel[order[j]]->getContent()->getSortPriority())
                {
                    buf = order[j-1];
                    order[j-1] = order[j];
                    order[j] = buf;
                }
                else
                {
                    break;
                }
            }
        }
        for(int i=0;i<selectedNumber[group];i++)
        {
            sel[order[i]]->setSucessor(sel[order[i+1]]);
        }
        sel[order[selectedNumber[group]]]->setSucessor(NULL);

        sel[0] = pListHead[group]->getSuccessor();

        //check how many types you have; store this number
        selectedTypes[group] =1;
        for(int i=1;i<selectedNumber[group];i++)
        {
            if(sel[0]->getSuccessor()->getContent()->getType() != sel[0]->getContent()->getType()) selectedTypes[group]++;
            sel[0] = sel[0]->getSuccessor();
        }
        if(priorizedNumber[group] > selectedTypes [group]) priorizedNumber[group] = 1;
    }
}

void cSelectionControl::cyclePriorization(int group)
{
    priorizedNumber[group]++;
    if(priorizedNumber[group] > selectedTypes[group])
    {
        priorizedNumber[group] = 1;
    }
}

cSelection* cSelectionControl::getFirstPriorizedUnit(int group)
{
    cSelection* sel = pListHead[0]->getSuccessor();
    //get the first priorized unit
    int typecycle = 0;
    int oldtype = -1;
    while(sel!=NULL)
    {
        if(sel->getContent()->getType() != oldtype)
        {
            typecycle++;
            oldtype = sel->getContent()->getType();
        }
        if(typecycle == priorizedNumber[group])
        {
            return sel;
        }
        sel=sel->getSuccessor();
    }
    return NULL;
}

int cSelectionControl::CenterMassX(int group)
{
    Error("Center of Mass X");
    cSelection* sel;
    cUnit* uni;
    uni = NULL;
    int MassX = 0;
    sel = pListHead[group];
    Error("before loop");
    for(int i=0;i <= selectedNumber[group]; i++)
    {
        Error("get Sucessor");
        sel = sel->getSuccessor();
        uni = NULL;
        Error("test");
        if(sel != NULL) uni = sel->getContent();
        if(uni != NULL) MassX += uni->getPosition()->getX();
    }
    if(selectedNumber[group] > 0){MassX = MassX / selectedNumber[group];}
    else{MassX = 0;}
    return MassX;
}

int cSelectionControl::CenterMassY(int group)
{
    Error("Center of Mass Y");
    cSelection* sel;
    cUnit* uni;
    uni = NULL;
    int MassY = 0;
    sel = pListHead[group];
    Error("before loop");
    for(int i=0;i <= selectedNumber[group]; i++)
    {
        Error("get Sucessor");
        sel = sel->getSuccessor();
        uni = NULL;
        Error("test");
        if(sel != NULL) uni = sel->getContent();
        if(uni != NULL) MassY += uni->getPosition()->getY();
    }
    if(selectedNumber[group] > 0){MassY = MassY / selectedNumber[group];}
    else{MassY = 0;}
    return MassY;
}
