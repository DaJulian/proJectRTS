#include "cBattleGame.h"

void cBattleGame::InitSettingsBattleGame(sUnitSpecifications* pUnitSpec)
{
    pUnitSpecifications = pUnitSpec;
    Error("initialized Unit specifications battle game");
}

void cBattleGame::InitBattleGame(int player,cWorldMap* pMap, sBattleGameInputs* batinp)
{
    Error("initializing battle game");
    bdoBattleGameScreen = true;
    battleGameModel.ready = false;
    battleGameModel.timer = BATTLE_GAME_TIME;
    pBattleGameInputs = batinp;
    pBattleGameInputs->BuyUnits = -1;
    pBattleGameInputs->SellUnits = -1;
    pBattleGameInputs->placeUnit = -1;
    pBattleGameInputs->placeUnitX = -1;
    pBattleGameInputs->removeUnitX = -1;

    pBattleGameInputs = batinp;

    for(int i=0;i<MAX_UNITS;i++)
    {
        battleGameModel.readyUnits[i] = 0;
    }

    std::fstream f;
    std::string garbage;
    f.open(BATTLE_GAME_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);

    f >> garbage >> battleGameModel.money
      >> garbage >> START_SECTOR_X
      >> garbage >> START_SECTOR_Y
      >> garbage >> PLAYER_1_START_X
      >> garbage >> PLAYER_1_START_Y
      >> garbage >> PLAYER_2_START_X
      >> garbage >> PLAYER_2_START_Y;

    Error("Player 2 start y", PLAYER_2_START_Y);

    for(int i=0;i<START_SECTOR_X;i++)
    {
        for(int j=0;j<START_SECTOR_Y;j++)
        {
            battleGameModel.startSector[i][j] = -1;
            if(player == 1)
            {
                if(pMap->getMap(PLAYER_1_START_X+i,PLAYER_1_START_Y+j)->getContainObstacle()!=0)
                {
                    battleGameModel.startSector[i][j] = -2;
                }
            }
            if(player == 2)
            {
                if(pMap->getMap(PLAYER_2_START_X+i,PLAYER_2_START_Y+j)->getContainObstacle()!=0)
                {
                    battleGameModel.startSector[i][j] = -2;
                }
            }

        }
    }
    Error("done initializing battle game");
}

bool cBattleGame::DoFrameBattleGameScreen(Uint32 t)
{
    battleGameModel.timer -= t;
    if(battleGameModel.timer <= 0)
    {
        battleGameModel.ready = true;
    }
    if(pBattleGameInputs->BuyUnits > -1)
    {
        if(battleGameModel.money >= pUnitSpecifications->UnitCost[pBattleGameInputs->BuyUnits])
        {
            battleGameModel.readyUnits[pBattleGameInputs->BuyUnits]++;
            battleGameModel.money -= pUnitSpecifications->UnitCost[pBattleGameInputs->BuyUnits];
        }
        pBattleGameInputs->BuyUnits = -1;
    }
    if(pBattleGameInputs->SellUnits > -1)
    {
        if(battleGameModel.readyUnits[pBattleGameInputs->SellUnits] > 0)
        {
            battleGameModel.readyUnits[pBattleGameInputs->SellUnits]--;
            battleGameModel.money += pUnitSpecifications->UnitCost[pBattleGameInputs->SellUnits];
        }
        pBattleGameInputs->SellUnits = -1;
    }
    if(pBattleGameInputs->placeUnit > -1)
    {
        if(battleGameModel.readyUnits[pBattleGameInputs->placeUnit] > 0 )
        {
           if(pBattleGameInputs->placeUnitX >= 0)
            {
                if(battleGameModel.startSector[pBattleGameInputs->placeUnitX][pBattleGameInputs->placeUnitY] == -1)
                {
                    battleGameModel.startSector[pBattleGameInputs->placeUnitX][pBattleGameInputs->placeUnitY] = pBattleGameInputs->placeUnit;
                    battleGameModel.readyUnits[pBattleGameInputs->placeUnit]--;
                }
            }
        }
        else
        {
            pBattleGameInputs->placeUnit = -1;
            //placeUnitsX = -1;
        }
    }
    pBattleGameInputs->placeUnitX = -1;
    if(pBattleGameInputs->removeUnitX > 0)
    {
        if(battleGameModel.startSector[pBattleGameInputs->removeUnitX][pBattleGameInputs->removeUnitY] != -1)
        {
            battleGameModel.readyUnits[battleGameModel.startSector[pBattleGameInputs->removeUnitX][pBattleGameInputs->removeUnitY]]++;
            battleGameModel.startSector[pBattleGameInputs->removeUnitX][pBattleGameInputs->removeUnitY] = -1;
        }
    }
    pBattleGameInputs->removeUnitX = -1;
    if(pBattleGameInputs->clickBattleGameReady)
    {
        battleGameModel.ready = true;
    }

    return battleGameModel.ready;
}

sBattleGameModel* cBattleGame::getBattleGameModel()
{
    return &battleGameModel;
}

bool cBattleGame::checkRun()
{
    return bdoBattleGameScreen;
}
void cBattleGame::setRun(bool ru)
{
    bdoBattleGameScreen = ru;
}
