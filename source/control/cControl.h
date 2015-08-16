#ifndef CCONTROL_H_INCLUDED
#define CCONTROL_H_INCLUDED

#include "cSelectionControl.h"
#include "cReplay.h"
#include "cPathFind.h"
#include "../netControl/cNetControl.h"
#include "../view/cView.h"
#include "../model/cGameModel.h"

class cControl : public cSelectionControl , public cNetControl
{
protected:
    //Run switch; when turned off the framework should shut down
    bool bRun;

    bool bStartSignal;

    Uint32 TimeSinceStart;

    cGameModel* pGameModel;
    cPathFind pathFindObj;

    cCommandList commandListObj;
    cChangeList changeListObj;
    cNetPackageList netPackToSend;
    cNetPackageList netPackToStrip;

    cReplay ReplayObj;

    cView* pView;
    sMousePosition* pMousePosition;
    sUnitSelectionInputs* pUnitSelectionInputs;
    sScrollInputs* pScrollInputs;

    //Identifiing number of the player
    int thisUser;
    unsigned short int lastPackID;

    int screenJumpRelativeX,screenJumpRelativeY;
    int drawAoe;

    //This will be used to check if there is anyone able to cast at the moment the cast is received in the group
    bool checkNewSpell;
public:
    bool InitControl(cView* pViewObj, cGameModel* pGameModelObj);

    void MapInit();
    void InitUnits(int InitSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX]);
    void LoadStandardOpponent();

    /*bool DoControl(Uint32 t,int scrX,int scrY,bool* TermGame,bool* SelectTile, bool* SelectMiniPic, int PicNumber,
                   SDL_Rect* SelectRect,bool* selectSpecificUnit, SDL_Rect* SpecificUnitRect,int specX,int specY,bool* doMove,bool* doAttack,bool* doStop,int moveX,int moveY,
                   bool* getGroup,bool* setGroup,bool* jumpGroup, int GroupNumber, bool* ShiftDown,bool *doCycleSel, int* activateCommand, int *doCast);*/
    bool DoControl(Uint32 t, bool* TermGame);

    bool CleanUpControl();

    bool checkCastable(cUnit* Caster,int slot);

    //functions that apply the commands or changes stored on the local machine or sent via TCP/IP to the actual game
    void applyChanges();
    void giveCommands();

    //stores given Commands by the local user to be sent via TCP/IP
    void storeCommand(cUnit* Un,int Commandtype,int goalX, int goalY,int priority);

    //functions to access the replay
    void writeReplay();
    void readReplay();

    //packing the List of Changes or Controls to NetPackages
    void PackChangeList();
    void PackCommandList();
    //unpacking the Netpackages to Changes or Commands on the corresponding List
    void StripReceivedListServer();
    void StripReceivedListClient();

    unsigned short int createNewPackID();

    bool checkRunning();
    void setRun(bool run);

    void DestroyUnit(cUnit* Unit);

    int getUser();
};

#endif // CCONTROL_H_INCLUDED
