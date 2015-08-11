#ifndef CBATTLEGAME_H_INCLUDED
#define CBATTLEGAME_H_INCLUDED

#include "../global.h"
#include "../tools.h"
#include "../model/cWorldMap.h"

class cBattleGame
{
protected:
    int timer;
    int money;
    int startSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX];
    bool bdoBattleGameScreen;
    bool ready;
    int readyUnits[MAX_UNITS];
    int BuyUnits;
    int SellUnits;
    int placeUnitsX;
    int placeUnitsY;
    int placeUnit;
    int removeUnitX;
    int removeUnitY;

    //Settings from Control
    int bGUnitMovementSpeed[UNIT_TYPE_MAX];
    int bGhealth[UNIT_TYPE_MAX];
    int bGmana[UNIT_TYPE_MAX];
    int bGhealthreg[UNIT_TYPE_MAX];
    int bGmanareg[UNIT_TYPE_MAX];
    int bGdamage[UNIT_TYPE_MAX];
    int bGsightRange[UNIT_TYPE_MAX];
    int bGArmor[UNIT_TYPE_MAX];
    int bGMagicarmor[UNIT_TYPE_MAX];
    int bGtauntRange[UNIT_TYPE_MAX];
    int bGsortPriority[UNIT_TYPE_MAX];
    int bGNumberOfAblilites[UNIT_TYPE_MAX];
    int bGUnitCost[UNIT_TYPE_MAX];
    int bGBuildingTime[UNIT_TYPE_MAX];

    int bGattackSpeed[UNIT_TYPE_MAX][ABILITY_MAX];
    int bGattackRange[UNIT_TYPE_MAX][ABILITY_MAX];
    int bGManaCost[UNIT_TYPE_MAX][ABILITY_MAX];
    int bGUnitEntityType[UNIT_TYPE_MAX][ABILITY_MAX];

    int bGUnitAnimSteps[UNIT_TYPE_MAX][NMBR_ACTIONS];
    int bGUnitAnimDuration[UNIT_TYPE_MAX][NMBR_ACTIONS];

public:
    void InitSettingsBattleGame(int UMS[UNIT_TYPE_MAX],int he[UNIT_TYPE_MAX],int ma[UNIT_TYPE_MAX],int hr[UNIT_TYPE_MAX],
                        int mr[UNIT_TYPE_MAX],int da[UNIT_TYPE_MAX],int sR[UNIT_TYPE_MAX],int Ar[UNIT_TYPE_MAX],
                        int Marm[UNIT_TYPE_MAX],
                        int tR[UNIT_TYPE_MAX],int sP[UNIT_TYPE_MAX],int NOfAb[UNIT_TYPE_MAX],int UC[UNIT_TYPE_MAX],
                        int BTime[UNIT_TYPE_MAX],int aSpeed[UNIT_TYPE_MAX][ABILITY_MAX],int aRange[UNIT_TYPE_MAX][ABILITY_MAX],
                        int MCost[UNIT_TYPE_MAX][ABILITY_MAX],int UEType[UNIT_TYPE_MAX][ABILITY_MAX]);

    void InitBattleGame(int User,cWorldMap* pMap);
    bool DoFrameBattleGameScreen(Uint32 t,bool* setReady);
    void FinalizeBattleGamePreparations();
};

#endif // CBATTLEGAME_H_INCLUDED
