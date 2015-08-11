#ifndef CSELECTIONCONTROL_H_INCLUDED
#define CSELECTIONCONTROL_H_INCLUDED

#include "cSelection.h"
#include "../tools.h"

class cSelectionControl
{
protected:
    cSelection* pListHead[11];
    int selectedNumber[11];
    int selectedTypes[11];
    int priorizedNumber[11];
public:
    cSelectionControl();

    bool AddSelection(cUnit* Sel,int group);
    bool RemoveSelection(cUnit* Sel,int group);

    bool ClearSelection(int group);

    bool AddSelectionByTile(cTile* Til,int owner,int group);
    bool RemoveSelectionByTile(cTile* Til,int group);

    void sortSelection(int group);

    void cyclePriorization(int group);

    cSelection* getFirstPriorizedUnit(int group);

    void copySelection(int sourceGroup,int targetGroup);
    void AddSelectionFromGroup(int sourceGroup,int targetGroup);

    int CenterMassX(int group);
    int CenterMassY(int group);
};

#endif // CSELECTIONCONTROL_H_INCLUDED
