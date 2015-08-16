#ifndef CBATTLEGAME_H_INCLUDED
#define CBATTLEGAME_H_INCLUDED

#include "../global.h"
#include "../tools.h"
#include "../model/cGameModel.h"
#include "../view/cInput.h"

/*struct sBattleGameInputs
{
    //battle game input Variables
    bool clickBattleGameReady;

    int BuyUnits;
    int SellUnits;
    int placeUnit;
    int placeUnitX;
    int placeUnitY;
    int removeUnitX;
    int removeUnitY;
};*/

struct sBattleGameModel
{
    bool ready;
    int readyUnits[MAX_UNITS];
    int startSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX];
    int money;
    int timer;
};

class cBattleGame
{
protected:
    bool bdoBattleGameScreen;

    sBattleGameInputs* pBattleGameInputs;
    sBattleGameModel battleGameModel;

    sUnitSpecifications* pUnitSpecifications;

public:
    void InitSettingsBattleGame(sUnitSpecifications* pUnitSpec);

    void InitBattleGame(int User ,cWorldMap* pMap, sBattleGameInputs* batinp);
    bool DoFrameBattleGameScreen(Uint32 t);

    sBattleGameModel* getBattleGameModel();

    void FinalizeBattleGamePreparations();

    bool checkRun();
    void setRun(bool ru);
};

#endif // CBATTLEGAME_H_INCLUDED
