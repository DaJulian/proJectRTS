#ifndef COUTPUT_H_INCLUDED
#define COUTPUT_H_INCLUDED

#include "../model/cWorldMap.h"
#include "../control/cSelection.h"
#include "cSurface.h"
#include "../model/cEntity.h"
#include "../model/cBuilding.h"

class cOutput
{
    protected:
        //Screen SUrface
    	SDL_Surface* mScreen;

        //Strings to the GrapicFiles
        std::string TileGraphicFile[TILE_TYPE_MAX];
        std::string TileMinimapGraphicFile[TILE_TYPE_MAX];
        std::string UnitGraphicFile[UNIT_TYPE_MAX][NMBR_ACTIONS][MAX_NUMBER_PLAYERS+1];
        std::string BuildingGraphicFile[BUILDING_TYPE_MAX][MAX_NUMBER_PLAYERS+1][2];
        std::string EntityGraphicFile[ENTITY_TYPE_MAX];
        std::string UnitPictureFile[UNIT_TYPE_MAX][MAX_NUMBER_PLAYERS+1];
        std::string UnitMiniPicFile[UNIT_TYPE_MAX][MAX_NUMBER_PLAYERS+1][2];
        std::string UnitMinimapGraphicFile[MAX_NUMBER_PLAYERS+1];
        std::string AbilityPicFile[ENTITY_TYPE_MAX];
        std::string GridAnimFile;
        std::string StandardPicFile[4];
        std::string ObstacleGraphicFile[OBSTACLE_TYPE_MAX];
        std::string ObstacleMinimapGraphicFile[OBSTACLE_TYPE_MAX];
        std::string HudGraphicFile;
        std::string selectionFrameFile;
        std::string aoeMarkerFile;
        std::string sightMaskGraphicFile;
        std::string sightMaskMinimapGraphicFile;
        std::string cursorGraphicFile[CURSOR_STATES];
        SDL_Surface* HudGraphic;
        SDL_Surface* selectionFrame;
        SDL_Surface* aoeMarker;
        //Tile Graphic Surfaces
        SDL_Surface* TileGraphics[TILE_TYPE_MAX];
        SDL_Surface* TileMinimapGraphics[TILE_TYPE_MAX];
        SDL_Surface* cursorGraphic[CURSOR_STATES];
        SDL_Surface* EntityGraphic[ENTITY_TYPE_MAX];
        //Unit Graphics
        SDL_Surface* UnitGraphics[UNIT_TYPE_MAX][NMBR_ACTIONS][MAX_NUMBER_PLAYERS+1];
        SDL_Surface* BuildingGraphics[BUILDING_TYPE_MAX][MAX_NUMBER_PLAYERS+1][2];

        //Unit Picture
        SDL_Surface* UnitPicture[UNIT_TYPE_MAX][MAX_NUMBER_PLAYERS+1];
        //Unit minipic
        SDL_Surface* UnitMiniPic[UNIT_TYPE_MAX][MAX_NUMBER_PLAYERS+1][2];
        SDL_Surface* UnitMinimapGraphic[MAX_NUMBER_PLAYERS+1];
        SDL_Surface* AbilityPic[ENTITY_TYPE_MAX];
        SDL_Surface* StandardPic[4];
        SDL_Surface* GridAnim;
        //Obstacle Graphics
        SDL_Surface* ObstacleGraphics[OBSTACLE_TYPE_MAX];
        SDL_Surface* ObstacleMinimapGraphics[TILE_TYPE_MAX];
        //Graphics for the fog of war
        SDL_Surface* sightMaskGraphics;
        SDL_Surface* sightMaskMinimapGraphics;

        SDL_Surface* mMinimap;

        //Map Object
        cWorldMap* pMapObj;

        //fog of war mask
        int sightMask[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];

        //this Users ID
        int thisOwner;

        //The Grid Light Animation
        int GridAnimTime;
        int GridAnimSteps;
        int GridAnimStepNow;

        //when to show healthbars
        int healthBarSettings;

        //Graphics for Battle Game screen:
        std::string BattleGameBackgroundFile;
        std::string EmptySectorFile;
        std::string blockedSectorFile;
        std::string cursorGraphicBattleGameFile;
        std::string forbiddenMaskFile;
        std::string readyButtonFile[2];

        SDL_Surface* BattleGameBackground;
        SDL_Surface* EmptySector;
        SDL_Surface* blockedSector;
        SDL_Surface* cursorGraphicBattleGame;
        SDL_Surface* forbiddenMask;
        SDL_Surface* readyButton[2];
    public:
        cOutput();

        bool drawFrame(Uint32 tick,int MouseX,int Mmousey,int cursorType,int scrX, int scrY,bool* bDrawSelectionRect
                       ,SDL_Rect* SelectionRect,cSelection* selListHead,int selectedNumber, int selectedBuildings,int typeNumbers,
                       int priorizedNumber,cEntity* EntityList[MAX_ENT],int numEnt,int drawAoe);
        void drawFrameBattleGameScreen(Uint32 ticks,int mouseX,int mouseY,int Units[MAX_UNITS],int placeNum
                                       ,bool clickReady,int startSector[START_SECTOR_X_MAX][START_SECTOR_Y_MAX]
                                       ,int placeUnit);

        bool InitVideo(cWorldMap* pMap,int own);
        bool CleanUpVideo();

        void InitMinimap();

        void drawMap(int screenX,int screenY);
        void drawBackgroundAndMinimap(int screenX,int screenY);
        void drawUnitsToMinimap();
        void drawObjectsToScreen(int screenX,int screenY);
        void drawScreenRectangleToMinimap(int screenX,int screenY);
        void drawSightmask(int screenX,int screenY);

        void drawEntities(cEntity* EntityList[MAX_ENT],int numEnt,int screenX, int screenY);

        void drawSelectionRectangle(SDL_Rect* Rect);

        void drawHealthBars(cUnit* Unit,int dX, int dY);

        void drawUnitHud(Uint32 tick,cSelection* selListHead,int selectedUnitNumber,int typeNumbers,int priorizedNumber);
        void drawUnitDescription(cUnit* selectedUnit);
        void drawUnitPictureAndDescription(Uint32 tick,cSelection* selListHead, int priorizedNumber);
        void drawUnitMinipictures(cSelection* selListHead,int selectedUnitNumber,int typeNumbers,int priorizedNumber);
        void drawGrid(cUnit* selectedUnit,Uint32 t);
        void applyDrawAoe(int posX,int posY,int rangeSq);

        void drawBuildingHud();

        void initSightMask();
        void processSightMask();
        bool checkSightPath(int goalX, int goalY, int sourceX, int sourceY);
        int checkSightPathDifferenceSign(int goal, int source);
};

#endif // COUTPUT_H_INCLUDED
