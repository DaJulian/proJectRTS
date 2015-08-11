#include "cBattleGame.h"

void cBattleGame::InitSettingsBattleGame(int UMS[UNIT_TYPE_MAX],int he[UNIT_TYPE_MAX],int ma[UNIT_TYPE_MAX],int hr[UNIT_TYPE_MAX],
                        int mr[UNIT_TYPE_MAX],int da[UNIT_TYPE_MAX],int sR[UNIT_TYPE_MAX],int Ar[UNIT_TYPE_MAX],
                        int Marm[UNIT_TYPE_MAX],
                        int tR[UNIT_TYPE_MAX],int sP[UNIT_TYPE_MAX],int NOfAb[UNIT_TYPE_MAX],int UC[UNIT_TYPE_MAX],
                        int BTime[UNIT_TYPE_MAX],int aSpeed[UNIT_TYPE_MAX][ABILITY_MAX],int aRange[UNIT_TYPE_MAX][ABILITY_MAX],
                        int MCost[UNIT_TYPE_MAX][ABILITY_MAX],int UEType[UNIT_TYPE_MAX][ABILITY_MAX])
{
    for(int i=0;i<UNIT_TYPE_MAX;i++)
    {
        bGUnitMovementSpeed[i] = UMS[i];
        bGhealth[i] = he[i];
        bGmana[i] = ma[i];
        bGhealthreg[i] = hr[i];
        bGmanareg[i] = mr[i];
        bGdamage[i] =da[i];
        bGsightRange[i] = sR[i];
        bGArmor[i] = Ar[i];
        bGMagicarmor[i] = Marm[i];
        bGtauntRange[i] = tR[i];
        bGsortPriority[i] = sP[i];
        bGNumberOfAblilites[i] = NOfAb[i];
        bGUnitCost[i] = UC[i];
        bGBuildingTime[i] = BTime[i];

        for(int j=0;j<ENTITY_TYPE_MAX;j++)
        {
            bGattackSpeed[i][j] = aSpeed[i][j];
            bGattackRange[i][j] = aRange[i][j];
            bGManaCost[i][j] = MCost[i][j];
            bGUnitEntityType[i][j] = UEType[i][j];
        }
    }
}

void cBattleGame::InitBattleGame(int player,cWorldMap* pMap)
{
    bdoBattleGameScreen = true;
    ready = false;
    timer = BATTLE_GAME_TIME;
    BuyUnits = -1;
    SellUnits = -1;
    placeUnit = -1;
    placeUnitsX = -1;
    removeUnitX = -1;
    for(int i=0;i<MAX_UNITS;i++)
    {
        readyUnits[i] = 0;
    }

    std::fstream f;
    std::string garbage;
    f.open(BATTLE_GAME_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);

    f >> garbage >> money
      >> garbage >> START_SECTOR_X
      >> garbage >> START_SECTOR_Y
      >> garbage >> PLAYER_1_START_X
      >> garbage >> PLAYER_1_START_Y
      >> garbage >> PLAYER_2_START_X
      >> garbage >> PLAYER_2_START_Y;

    for(int i=0;i<START_SECTOR_X;i++)
    {
        for(int j=0;j<START_SECTOR_Y;j++)
        {
            startSector[i][j] = -1;
            if(player == 1)
            {
                if(pMap->getMap(PLAYER_1_START_X+i,PLAYER_1_START_Y+j)->getContainObstacle()!=0)
                {
                    startSector[i][j] = -2;
                }
            }
            if(player == 2)
            {
                if(pMap->getMap(PLAYER_2_START_X+i,PLAYER_2_START_Y+j)->getContainObstacle()!=0)
                {
                    startSector[i][j] = -2;
                }
            }

        }
    }
}

bool cBattleGame::DoFrameBattleGameScreen(Uint32 t,bool* setReady)
{
    timer -= t;
    if(timer <= 0)
    {
        ready = true;
    }
    if(BuyUnits > -1)
    {
        if(money >= bGUnitCost[BuyUnits])
        {
            readyUnits[BuyUnits]++;
            money -= bGUnitCost[BuyUnits];
        }
        BuyUnits = -1;
    }
    if(SellUnits > -1)
    {
        if(readyUnits[SellUnits] > 0)
        {
            readyUnits[SellUnits]--;
            money += bGUnitCost[SellUnits];
        }
        SellUnits = -1;
    }
    if(placeUnit > -1)
    {
        if(readyUnits[placeUnit] > 0 )
        {
           if(placeUnitsX >= 0)
            {
                if(startSector[placeUnitsX][placeUnitsY] == -1)
                {
                    startSector[placeUnitsX][placeUnitsY] = placeUnit;
                    readyUnits[placeUnit]--;
                }
            }
        }
        else
        {
            placeUnit = -1;
            //placeUnitsX = -1;
        }
    }
    placeUnitsX = -1;
    if(removeUnitX > 0)
    {
        if(startSector[removeUnitX][removeUnitY] != -1)
        {
            readyUnits[startSector[removeUnitX][removeUnitY]]++;
            startSector[removeUnitX][removeUnitY] = -1;
        }
    }
    removeUnitX = -1;
    if(*setReady == true)
    {
        ready = true;
    }
    return ready;
}
