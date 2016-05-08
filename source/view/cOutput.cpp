#include "cOutput.h"

cOutput::cOutput()
{
    //Set the pointers to 0 for sake of safety
    mScreen = NULL;
    //all the Surfaces to NULL
    for(int i=0;i<TILE_TYPE_MAX;i++)
    {
        TileGraphics[i] = NULL;
    }
    for(int j=0;j<NMBR_ACTIONS;j++)
    {
        for(int i=0;i<UNIT_TYPE_MAX;i++)
        {
            for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
            {
                UnitGraphics[i][j][k] = NULL;
            }
        }
    }

    pGameModel = NULL;
    activeScreen = SCREEN_BATTLE_GAME;
}

bool cOutput::InitVideo(cGameModel* pMod, sBattleGameModel* BaMo, int own
                        , sMousePosition* moupo, sScrollInputs* scrollolol, sBattleGameInputs* Bagainp, sUnitSelectionInputs* UselInp)
{
    //TODO
    thisOwner = own;
    //document
    Error("Initializing output");

    pMousePosition = moupo;
    pScrollInputs = scrollolol;
    pBattleGameInputs = Bagainp;
    pUnitSelectionInputs = UselInp;

    healthBarSettings = HEALTHBARS_ALWAYS;

    std::string garbage;
    std::fstream f;
	f.open(GRAPHIC_SETTINGS_FILE.c_str(), std::ios::in | std::ios::app);

    //Set Video Settings
    f >> garbage >> SCREEN_WIDTH
      >> garbage >> SCREEN_HEIGHT
      >> garbage >> COLOR_DEPTH;

    //Set width in tilelengths (TODO depending on the screen format)
	SCR_TILE_WIDTH  = SCREEN_WIDTH / TILE_WIDTH;
    SCR_TILE_HEIGHT = (SCREEN_HEIGHT - 240) / TILE_HEIGHT;

    //calculate minimap tile widths/heights and set Minimap position

    if(WORLD_WIDTH<=60 && WORLD_HEIGHT<=60)
    {
        MINIMAP_TILE_WIDTH = 4;
        MINIMAP_TILE_HEIGHT = 4;
        MINIMAP_WIDTH = WORLD_WIDTH*MINIMAP_TILE_WIDTH ;
        MINIMAP_HEIGHT = WORLD_HEIGHT*MINIMAP_TILE_HEIGHT;
        MINIMAP_X = 4   + (60 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        MINIMAP_Y = 524 + (60 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
        BATTLE_GAME_MINIMAP_X = 780 + (60 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        BATTLE_GAME_MINIMAP_Y = 524 + (60 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
    }
    if(WORLD_WIDTH>60 || WORLD_HEIGHT> 60)
    {
        MINIMAP_TILE_WIDTH = 3;
        MINIMAP_TILE_HEIGHT = 3;
        MINIMAP_WIDTH = WORLD_WIDTH*MINIMAP_TILE_WIDTH ;
        MINIMAP_HEIGHT = WORLD_HEIGHT*MINIMAP_TILE_HEIGHT;
        MINIMAP_X = 4   + (80 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        MINIMAP_Y = 524 + (80 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
        BATTLE_GAME_MINIMAP_X = 780 + (80 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        BATTLE_GAME_MINIMAP_Y = 524 + (80 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
    }
    if(WORLD_WIDTH>80 || WORLD_HEIGHT> 80)
    {
        MINIMAP_TILE_WIDTH = 2;
        MINIMAP_TILE_HEIGHT = 2;
        MINIMAP_WIDTH = WORLD_WIDTH*MINIMAP_TILE_WIDTH ;
        MINIMAP_HEIGHT = WORLD_HEIGHT*MINIMAP_TILE_HEIGHT;
        MINIMAP_X = 4   + (120 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        MINIMAP_Y = 524 + (120 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
        BATTLE_GAME_MINIMAP_X = 780 + (120 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        BATTLE_GAME_MINIMAP_Y = 524 + (120 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
    }
    if(WORLD_WIDTH>120 || WORLD_HEIGHT> 120)
    {
        MINIMAP_TILE_WIDTH = 1;
        MINIMAP_TILE_HEIGHT = 1;
        MINIMAP_WIDTH = WORLD_WIDTH*MINIMAP_TILE_WIDTH ;
        MINIMAP_HEIGHT = WORLD_HEIGHT*MINIMAP_TILE_HEIGHT;
        MINIMAP_X = 4   + (240 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        MINIMAP_Y = 524 + (240 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
        BATTLE_GAME_MINIMAP_X = 780 + (240 - WORLD_WIDTH)*MINIMAP_TILE_WIDTH/2 ;
        BATTLE_GAME_MINIMAP_Y = 524 + (240 - WORLD_HEIGHT)*MINIMAP_TILE_HEIGHT/2;
    }
    Error("MINIMAP_TILE_WIDTH",MINIMAP_TILE_WIDTH);


    //Set Minimap size

    std::string candidate[4];

    f >> garbage >> HudGraphicFile;
    f >> garbage >> selectionFrameFile;
    f >> garbage >> aoeMarkerFile;
    f >> garbage >> StandardPicFile[0];
    f >> garbage >> StandardPicFile[1];
    f >> garbage >> StandardPicFile[2];
    f >> garbage >> StandardPicFile[3];
    f >> garbage >> GridAnimFile;
    f >> garbage >> GridAnimSteps;
    f >> garbage;
    for(int j=0;j<CURSOR_STATES;j++)
    {
        f >> garbage >> cursorGraphicFile[j];
    }

    f >> garbage >> sightMaskGraphicFile;
    for(int j= 0; j<4;j++)
        {
            f >> candidate[j];
        }
        if(WORLD_WIDTH<=60 && WORLD_HEIGHT<=60)
        {
            sightMaskMinimapGraphicFile = candidate[0];
        }
        if(WORLD_WIDTH>60 || WORLD_HEIGHT> 60)
        {
            sightMaskMinimapGraphicFile = candidate[1];
        }
        if(WORLD_WIDTH>80 || WORLD_HEIGHT> 80)
        {
            sightMaskMinimapGraphicFile = candidate[2];
        }
        if(WORLD_WIDTH>120 || WORLD_HEIGHT> 120)
        {
            sightMaskMinimapGraphicFile = candidate[3];
        }


    for(int i=0;i<TILE_TYPE_NUMBER;i++)
    {
        f >> garbage >> TileGraphicFile[i];

        for(int j= 0; j<4;j++)
        {
            f >> candidate[j];
        }
        if(WORLD_WIDTH<=60 && WORLD_HEIGHT<=60)
        {
            TileMinimapGraphicFile[i] = candidate[0];
        }
        if(WORLD_WIDTH>60 || WORLD_HEIGHT> 60)
        {
            TileMinimapGraphicFile[i] = candidate[1];
        }
        if(WORLD_WIDTH>80 || WORLD_HEIGHT> 80)
        {
            TileMinimapGraphicFile[i] = candidate[2];
        }
        if(WORLD_WIDTH>120 || WORLD_HEIGHT> 120)
        {
            TileMinimapGraphicFile[i] = candidate[3];
        }
    }



    for(int i=0;i<OBSTACLE_TYPE_NUMBER;i++)
    {
        f >> garbage >> ObstacleGraphicFile[i];
        for(int j= 0; j<4;j++)
        {
            f >> candidate[j];
        }
        if(WORLD_WIDTH<=60 && WORLD_HEIGHT<=60)
        {
            ObstacleMinimapGraphicFile[i] = candidate[0];
        }
        if(WORLD_WIDTH>60 || WORLD_HEIGHT> 60)
        {
            ObstacleMinimapGraphicFile[i] = candidate[1];
        }
        if(WORLD_WIDTH>80 || WORLD_HEIGHT> 80)
        {
            ObstacleMinimapGraphicFile[i] = candidate[2];
        }
        if(WORLD_WIDTH>120 || WORLD_HEIGHT> 120)
        {
            ObstacleMinimapGraphicFile[i] = candidate[3];
        }
    }

    f >> garbage;
    for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
    {
        for(int j= 0; j<4;j++)
        {
            f >> candidate[j];
        }
        if(WORLD_WIDTH<=60 && WORLD_HEIGHT<=60)
        {
            UnitMinimapGraphicFile[k] = candidate[0];
        }
        if(WORLD_WIDTH>60 || WORLD_HEIGHT> 60)
        {
            UnitMinimapGraphicFile[k] = candidate[1];
        }
        if(WORLD_WIDTH>80 || WORLD_HEIGHT> 80)
        {
            UnitMinimapGraphicFile[k] = candidate[2];
        }
        if(WORLD_WIDTH>120 || WORLD_HEIGHT> 120)
        {
            UnitMinimapGraphicFile[k] = candidate[3];
        }
    }

    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        f >> garbage;
        for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
        {
            f >> garbage;
            for(int j=0;j<NMBR_ACTIONS;j++)
            {
                f  >> UnitGraphicFile[i][j][k];
            }
            f  >> UnitPictureFile[i][k];
            f  >> UnitMiniPicFile[i][k][0] >> UnitMiniPicFile[i][k][1];
        }
    }
    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        f >> garbage;
        for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
        {
            f >> garbage >> BuildingGraphicFile[i][k][0];
            f >> BuildingGraphicFile[i][k][1];
        }
    }

    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        f >> garbage >> EntityGraphicFile[i] >> AbilityPicFile[i];
    }

    f >> garbage >> BattleGameBackgroundFile;
    f >> garbage >> EmptySectorFile;
    f >> garbage >> blockedSectorFile;
    f >> garbage >> cursorGraphicBattleGameFile;
    f >> garbage >> forbiddenMaskFile;
    f >> garbage >> readyButtonFile[0];
    f >> garbage >> readyButtonFile[1];

    Error("read all graphic settings files");

    //Initialize the Video Mode
    mScreen = 0;
	mScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH, /*SDL_ANYFORMAT |*/SDL_SWSURFACE | SDL_DOUBLEBUF /*| SDL_FULLSCREEN*/);
	if (mScreen == NULL)
	{
		Error("Video Modus could not be initiallized: %s\n");
		Error(SDL_GetError());
		exit (1);
	}
	atexit (SDL_Quit);

	//hiding real curor
    SDL_ShowCursor(0);

    //Initialize the pointer on the map
    pGameModel = pMod;
    pBattleGameModel = BaMo;

    if((HudGraphic = cSurface::Load(HudGraphicFile)) == NULL) {Error("Fehler beim Laden der HUD Graphic");return false;}
    if((selectionFrame = cSurface::Load(selectionFrameFile)) == NULL) {Error("Fehler beim Laden der selectionFrame");return false;}
    if((aoeMarker = cSurface::Load(aoeMarkerFile)) == NULL) {Error("Fehler beim Laden der selectionFrame");return false;}
    for(int i=0;i<4;i++)
    {
        if((StandardPic[i] = cSurface::Load(StandardPicFile[i])) == NULL) {Error("Fehler beim Laden der selectionFrame");return false;}
    }
    if((GridAnim = cSurface::Load(GridAnimFile)) == NULL) {Error("Fehler beim Laden der HUD Graphic");return false;}


    for(int j=0;j<CURSOR_STATES;j++)
    {
        if((cursorGraphic[j] = cSurface::Load(cursorGraphicFile[j])) == NULL) {Error("Fehler beim Laden eines Cursorbildes ",j);return false;}
    }

    //Load Unit Graphics
    for(int j=0;j<NMBR_ACTIONS;j++)
    {
        for(int i=0;i<UNIT_TYPE_NUMBER;i++)
        {
            for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
            {
                if((UnitGraphics[i][j][k] = cSurface::Load(UnitGraphicFile[i][j][k])) == NULL) {Error("Fehler beim Laden eines Unit Bildes: Type",i);Error("Action",j);return false;}
            }
        }
    }

    for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
    {
        if((UnitMinimapGraphic[k] = cSurface::Load(UnitMinimapGraphicFile[k])) == NULL) {Error("Fehler beim Laden eines UnitMiniPic Bildes: Type",k);return false;}
    }

    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
        {
            if((UnitPicture[i][k] = cSurface::Load(UnitPictureFile[i][k])) == NULL) {Error("Fehler beim Laden eines UnitPicture Bildes: Type",i);return false;}
            if((UnitMiniPic[i][k][0] = cSurface::Load(UnitMiniPicFile[i][k][0])) == NULL) {Error("Fehler beim Laden eines UnitMiniPic Bildes: Type",i);return false;}
            if((UnitMiniPic[i][k][1] = cSurface::Load(UnitMiniPicFile[i][k][1])) == NULL) {Error("Fehler beim Laden eines UnitMiniPic Bildes: Type",i);return false;}
        }
    }

    for(int i=0;i<BUILDING_TYPE_NUMBER;i++)
    {
        for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
        {
            if((BuildingGraphics[i][k][0] = cSurface::Load(BuildingGraphicFile[i][k][0]))==NULL){Error("load Error Building Graphics",i);return false;}
            if((BuildingGraphics[i][k][1] = cSurface::Load(BuildingGraphicFile[i][k][1]))==NULL){Error("load Error Building Graphics",i);return false;}
        }
    }

    //Load Entity Grphics
    for(int i=0;i<ENTITY_TYPE_NUMBER;i++)
    {
        if((EntityGraphic[i] = cSurface::Load(EntityGraphicFile[i])) == NULL) {Error("Fehler beim Laden eines EntityGraphic Bildes: Type",i);return false;}
        if((AbilityPic[i] = cSurface::Load(AbilityPicFile[i])) == NULL) {Error("Fehler beim Laden eines EntityGraphic Bildes: Type",i);return false;}
    }
    //Load Tile Graphics
    for(int i=0;i<TILE_TYPE_NUMBER;i++)
    {
        if((TileGraphics[i] = cSurface::Load(TileGraphicFile[i])) == NULL) {Error("Fehler beim Laden eines Tile Bildes: Type",i);return false;}
        if((TileMinimapGraphics[i] = cSurface::Load(TileMinimapGraphicFile[i])) == NULL) {Error("Fehler beim Laden eines Tile Minimap Bildes: Type",i);return false;}
    }

    //Load Obstacle Graphics
    for(int i=0;i<OBSTACLE_TYPE_NUMBER;i++)
    {
        if((ObstacleGraphics[i] = cSurface::Load(ObstacleGraphicFile[i])) == NULL) {Error("Fehler beim Laden eines Obstacle Bildes: Type",i);return false;}
        if((ObstacleMinimapGraphics[i] = cSurface::Load(ObstacleMinimapGraphicFile[i])) == NULL) {Error("Fehler beim Laden eines Obstacle Minmap Bildes: Type",i);return false;}
    }

    //build surfaces for fog of war
    if((sightMaskGraphics = cSurface::Load(sightMaskGraphicFile)) == NULL) {Error("Fehler beim bauen der fog of war grafik");return false;}
    //if((sightMaskGraphics = cSurface::CreateRGBSurf(TILE_WIDTH,TILE_HEIGHT,COLOR_DEPTH,255,255,255,0)))
    if((sightMaskMinimapGraphics = cSurface::Load(sightMaskMinimapGraphicFile)) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}

    if((BattleGameBackground = cSurface::Load(BattleGameBackgroundFile)) == NULL) {Error("Fehler beim bauen der fog of war grafik");return false;}
    if((EmptySector = cSurface::Load(EmptySectorFile)) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}
    if((blockedSector = cSurface::Load(blockedSectorFile)) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}
    if((cursorGraphicBattleGame = cSurface::Load(cursorGraphicBattleGameFile)) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}
    if((forbiddenMask = cSurface::Load(forbiddenMaskFile)) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}
    if((readyButton[0] = cSurface::Load(readyButtonFile[0])) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}
    if((readyButton[1] = cSurface::Load(readyButtonFile[1])) == NULL) {Error("Fehler beim bauen der fog of war minimapgrafik");return false;}

    if((mMinimap = cSurface::CreateRGBSurf(MINIMAP_WIDTH,MINIMAP_HEIGHT,COLOR_DEPTH,0,0,0,0)) == NULL){Error("Error initiating minimap surface");}
    InitMinimap();

    GridAnimStepNow = 0;
    GridAnimTime = GRID_ANIMATION_TICKLENGTH;

    activeScreen = SCREEN_BATTLE_GAME;

	return 1;
}
bool cOutput::drawFrame(Uint32 ticks)
{
    switch(activeScreen)
    {
        case SCREEN_BATTLE_GAME:
        {
            drawFrameBattleGameScreen(ticks);
            break;
        }
        case SCREEN_MAP_UNIT:
        {
            drawFrameUnitSelection(ticks);
            break;
        }
    }
    return 1;
}


void cOutput::drawFrameUnitSelection(Uint32 ticks)
{
    drawMap();
    drawEntities();
    if(pUnitSelectionInputs->doDrawRect){ drawSelectionRectangle(&(pUnitSelectionInputs->DrawRect)); }

    if(selectedNumber > 0)
    {
        drawUnitHud(ticks);
    }
    else if(selectedBuildings > 0)
    {
        ///TODO
        //drawBuildingHud();
    }

    if(drawAoe != -1)
    {
        applyDrawAoe((pMousePosition->X)/TILE_WIDTH,(pMousePosition->Y)/TILE_HEIGHT,drawAoe);
    }
    // Draw the cursor
    cSurface::Draw(mScreen,cursorGraphic[pUnitSelectionInputs->cursorType],pMousePosition->X+CURSOR_OFFSET_X,pMousePosition->Y+CURSOR_OFFSET_Y);

    //Show the new screen
    SDL_Flip(mScreen);
}

void cOutput::drawUnitHud(Uint32 ticks)
{
    drawUnitPictureAndDescription(ticks);
    drawUnitMinipictures();
    ///TODO draw general Grid for all units
    //drawGrid()
}

void cOutput::drawUnitPictureAndDescription(Uint32 ticks)
{
    if(selectionListHead->getSuccessor() != NULL)
    {
        cSelection* sel = selectionListHead->getSuccessor();
        //get the first priorized unit
        int typecycle = 0;
        int oldtype = -1;
        while(sel!=NULL)
        {
            Error("cycle to draw description and grid;", sel->getContent()->getType());
            if(sel->getContent()->getType() != oldtype)
            {
                typecycle++;
                oldtype = sel->getContent()->getType();
            }
            if(typecycle == priorizedNumber)
            {
                drawUnitDescription(sel->getContent());
                //Draw the Grid
                drawGrid(sel->getContent(),ticks);
                break;
            }
            sel=sel->getSuccessor();
        }
    }
}

//Draw the minimap
void cOutput::InitMinimap()
{
    cTile* position;
    cObstacle* Obstacle;
    int tileType,containType;
    SDL_Surface* Minmap;
    //Draw background + Minimap
    for(int i=0;i < WORLD_WIDTH;i++)
    {
        for(int j=0;j < WORLD_HEIGHT;j++)
        {
            position = pGameModel->getMap(i,j);
            //read the tiletype at this location
            tileType = position->Gettype();
            containType = position->getContainType();

            switch(containType)
            {
                case 0:
                {
                    Minmap = TileMinimapGraphics[tileType];
                    break;
                }
                case OBJ_TYPE_OBSTACLE:
                {
                    Obstacle = position->getContainObstacle();
                    Minmap = ObstacleMinimapGraphics[Obstacle->getType()];
                    break;
                }
            }
            //Draw Minimap
            cSurface::Draw(mMinimap,Minmap,MINIMAP_TILE_WIDTH*i,MINIMAP_TILE_HEIGHT*j);
        }
    }
}

void cOutput::drawMap()
{
    cSurface::Draw(mScreen,HudGraphic,0,0);
    drawBackgroundAndMinimap();
    processSightMask();
    drawUnitsToMinimap();
    drawObjectsToScreen();
    drawScreenRectangleToMinimap();
    drawSightmask();
}

void cOutput::drawBackgroundAndMinimap()
{
    int tileType;
    int containType;
    cTile* position;
    SDL_Surface* Tile;
    SDL_Surface* Minmap;
    cUnit* Unit;
    cObstacle* Obstacle;

    for(int i=0;i < WORLD_WIDTH;i++)
    {
        for(int j=0;j < WORLD_HEIGHT;j++)
        {
            sightMask[i][j]=-2;
            position = pGameModel->getMap(i,j);
            //read the tiletype at this location
            tileType = position->Gettype();
            containType = position->getContainType();

            switch(containType)
            {
                case OBJ_TYPE_BUILDING_ACCESS:
                case 0:
                {
                    Minmap = TileMinimapGraphics[tileType];
                    sightMask[i][j]=-2;
                    break;
                }
                //case OBJ_TYPE_BUILDING_WALL:
                case OBJ_TYPE_OBSTACLE:
                {
                    Obstacle = position->getContainObstacle();
                    Minmap = ObstacleMinimapGraphics[Obstacle->getType()];
                    sightMask[i][j]=-3;
                    break;
                }
                case OBJ_TYPE_BUILDING_WITH_UNIT:
                case OBJ_TYPE_UNIT:
                {
                    Unit = position->getContainUnit();
                    if(Unit->getOwner() == thisOwner)
                    {
                        sightMask[i][j] = position->getContainUnit()->getSightRange();
                    }
                    else
                    {
                        sightMask[i][j]=-3;
                    }

                    //TODO DRAW A COLOR TO THE MINIMAP
                    Minmap = TileMinimapGraphics[tileType];

                    break;
                }
            }
            //Draw Minimap
            cSurface::Draw(mScreen,Minmap,MINIMAP_X + MINIMAP_TILE_WIDTH*i,MINIMAP_Y + MINIMAP_TILE_HEIGHT*j);

            //if on the screen draw a tile there
            if(i>=pScrollInputs->screenX && i < pScrollInputs->screenX+SCR_TILE_WIDTH && j>=pScrollInputs->screenY && j<pScrollInputs->screenY+SCR_TILE_HEIGHT )
            {
                //Graphic of the corresponding field
                Tile = TileGraphics[tileType];
                //Draw it at the right spot
                cSurface::Draw(mScreen,Tile,(i-pScrollInputs->screenX)*TILE_WIDTH,(j-pScrollInputs->screenY)*TILE_HEIGHT);
            }

        }
    }
}

void cOutput::drawUnitsToMinimap()
{
    cUnit* Unit;
    int unitowner;
    SDL_Surface* Minmap;

    for(int i=0;i < WORLD_WIDTH;i++)
    {
        for(int j=0;j < WORLD_HEIGHT;j++)
        {
            if(sightMask[i][j] >= -1)
            {
                switch(pGameModel->getMap(i,j)->getContainType())
                {
                    case OBJ_TYPE_UNIT:
                    {
                        //read the Unit
                        Unit = pGameModel->getMap(i,j)->getContainUnit();
                        //read the obj and actiontype
                        unitowner  = Unit->getOwner();
                        Minmap = UnitMinimapGraphic[unitowner];
                        cSurface::Draw(mScreen,Minmap,MINIMAP_X + MINIMAP_TILE_WIDTH*i,MINIMAP_Y + MINIMAP_TILE_HEIGHT*j);
                   }
                }
            }
        }
    }
}

void cOutput::drawObjectsToScreen()
{
    SDL_Surface* Object;
    cUnit* Unit;
    cObstacle* Obstacle;
    cBuilding* Building;
    int objtype,actiontype,unitowner;
    int offsetX,offsetY,drawToX,drawToY,takeX,takeY,normPosX,normPosY;

    //Draw Objects
    for(int i=pScrollInputs->screenX;i < pScrollInputs->screenX+SCR_TILE_WIDTH;i++)
    {
        for(int j=pScrollInputs->screenY;j < pScrollInputs->screenY+SCR_TILE_HEIGHT;j++)
        {
            switch(pGameModel->getMap(i,j)->getContainType())
            {
                //empty
                case 0:
                {
                    break;
                }
                case OBJ_TYPE_BUILDING_ACCESS:
                case OBJ_TYPE_BUILDING_WALL:
                {
                    Building = pGameModel->getMap(i,j)->getContainBuilding();
                    objtype = Building->getType();
                    unitowner = Building->getOwner();

                    normPosX = i-Building->getPositionX();
                    normPosY = j-Building->getPositionY();

                    if(Building->checkAccess(thisOwner))
                    {
                        Object = BuildingGraphics[objtype][unitowner][1];
                    }
                    else
                    {
                        Object = BuildingGraphics[objtype][unitowner][0];
                    }
                    takeX = normPosX*TILE_WIDTH;
                    takeY = normPosY*TILE_WIDTH;
                    drawToX = (i-pScrollInputs->screenX)*TILE_WIDTH;
                    drawToY = (j-pScrollInputs->screenY)*TILE_HEIGHT;
                    cSurface::Draw(mScreen,Object,drawToX,drawToY,takeX,takeY,TILE_WIDTH,TILE_HEIGHT);
                    Error2("Drawing Building ",drawToX);

                    break;
                }
                case OBJ_TYPE_BUILDING_WITH_UNIT:
                {
                    if(sightMask[i][j] >= -1)
                    {
                        Building = pGameModel->getMap(i,j)->getContainBuilding();
                        objtype = Building->getType();
                        unitowner = Building->getOwner();

                        normPosX = i-Building->getPositionX();
                        normPosY = j-Building->getPositionY();

                        if(Building->checkAccess(thisOwner))
                        {
                            Object = BuildingGraphics[objtype][unitowner][1];
                        }
                        else
                        {
                            Object = BuildingGraphics[objtype][unitowner][0];
                        }
                        takeX = normPosX*TILE_WIDTH;
                        takeY = normPosY*TILE_WIDTH;
                        drawToX = (i-pScrollInputs->screenX)*TILE_WIDTH;
                        drawToY = (j-pScrollInputs->screenY)*TILE_HEIGHT;
                        cSurface::Draw(mScreen,Object,drawToX,drawToY,takeX,takeY,TILE_WIDTH,TILE_HEIGHT);
                    }
                }
                //Check for an Unit
                case OBJ_TYPE_UNIT:
                {
                    if(sightMask[i][j] >= -1)
                    {
                        //read the Unit
                        Unit = pGameModel->getMap(i,j)->getContainUnit();
                        //read the obj and actiontype
                        objtype    = Unit->getType();
                        actiontype = Unit->getActionType();
                        unitowner  = Unit->getOwner();

                        //get the corresponding graphic from the list
                        Object = UnitGraphics[objtype][actiontype][unitowner];

                        //draw it to the screen according to its animationstep
                        offsetX = Unit->getOffsetX();
                        offsetY = Unit->getOffsetY();
                        drawToX = (i-pScrollInputs->screenX)*TILE_WIDTH+offsetX;
                        drawToY = (j-pScrollInputs->screenY)*TILE_HEIGHT+offsetY;
                        takeX = (Unit->getDirection()-1)*TILE_WIDTH;
                        takeY = (Unit->getAnimStepNow())*TILE_HEIGHT;
                        cSurface::Draw(mScreen,Object,drawToX,drawToY,takeX,takeY,TILE_WIDTH,TILE_HEIGHT);
                        //Draw a selection frame if the Unit is selected
                        if(Unit->checkSelected())
                        {cSurface::Draw(mScreen,selectionFrame,drawToX,drawToY,0,0,TILE_WIDTH,TILE_HEIGHT);}
                        //Drawing healthbars; also checking if this needs to be done
                        drawHealthBars(Unit,drawToX,drawToY);

                    }
                    break;
                }
                //Check for an Obstacle
                case OBJ_TYPE_OBSTACLE:
                {
                    //read the Unit
                    Obstacle = pGameModel->getMap(i,j)->getContainObstacle();
                    //read the obj and actiontype
                    objtype    = Obstacle->getType();

                    //get the corresponding graphic from the list
                    Object = ObstacleGraphics[objtype];

                    //draw it to the screen TODO rotation and animationstep
                    drawToX = (i-pScrollInputs->screenX)*TILE_WIDTH;
                    drawToY = (j-pScrollInputs->screenY)*TILE_HEIGHT;
                    cSurface::Draw(mScreen,Object,drawToX,drawToY);
                    break;
                }
            }
        }
    }
}

void cOutput::processSightMask()
{
    bool obstructed;
    for(int j=0;j<WORLD_HEIGHT;j++)
    {
        for(int i=0;i<WORLD_WIDTH;i++)
        {
            if(sightMask[i][j] > 0)
            {
                int kmin = i - sightMask[i][j];
                int lmin = j - sightMask[i][j];
                if(kmin<0){kmin=0;}
                if(lmin<0){lmin=0;}
                int kmax = i + sightMask[i][j];
                int lmax = j + sightMask[i][j];
                if(kmax>=WORLD_WIDTH){kmax=WORLD_WIDTH-1;}
                if(lmax>=WORLD_HEIGHT){lmax=WORLD_HEIGHT-1;}
                for(int k=kmin;k<=kmax;k++)
                {
                    for(int l=lmin;l<=lmax;l++)
                    {
                        if(realDist(k,l,i,j) <= sightMask[i][j])
                        {
                            obstructed = checkSightPath(k,l,i,j);
                            if(obstructed == false)
                            {
                                if(sightMask[k][l] < -1){sightMask[k][l] += 2;}
                            }
                        }
                    }
                }
            }
        }
    }
}

bool cOutput::checkSightPath(int goalX, int goalY, int sourceX, int sourceY)
{
    int xSign,ySign;
    int xDistance,yDistance;
    int xCheck,yCheck;
    float totalDistance,xCheckFloat,yCheckFloat;
    int iterations;
    //bool obstructed = false;
    //Error2("goalX ", goalX);
    //Error2("goalY ", goalY);
    //Error2("sourceX ", sourceX);
    //Error2("sourceY ", sourceY);

    xSign = checkSightPathDifferenceSign(goalX, sourceX);
    ySign = checkSightPathDifferenceSign(goalY, sourceY);
    xDistance = (goalX-sourceX)*xSign;
    yDistance = (goalY-sourceY)*ySign;
    totalDistance = sqrt((goalX-sourceX)*(goalX-sourceX) + (goalY-sourceY)*(goalY-sourceY));
    iterations = int(totalDistance*4);
    xCheckFloat = float(sourceX) + 0.5;
    yCheckFloat = float(sourceY) + 0.5;
    for(int i=0;i<iterations;i++)
    {
        xCheckFloat = xCheckFloat + 0.25*xDistance/totalDistance*xSign;
        yCheckFloat = yCheckFloat + 0.25*yDistance/totalDistance*ySign;
        xCheck = int(xCheckFloat);
        yCheck = int(yCheckFloat);
        //Error2("xCheck ",xCheck);
        //Error2("yCheck ",yCheck);
        if(xCheck == goalX && yCheck == goalY){return false;}

        if(sightMask[xCheck][yCheck] == -1 || sightMask[xCheck][yCheck] == -3)
        {
            return true;
        }
    }
    return false;
}

int cOutput::checkSightPathDifferenceSign(int goal, int source)
{
    if(goal > source)
    {
        return +1;
    }
    if(goal < source)
    {
        return -1;
    }
    if(goal == source)
    {
        return 0;
    }
    return 0;
}

void cOutput::drawScreenRectangleToMinimap()
{
    SDL_Rect minimaprect;

    minimaprect.x = MINIMAP_X + MINIMAP_TILE_WIDTH*pScrollInputs->screenX;
    minimaprect.y = MINIMAP_Y + MINIMAP_TILE_HEIGHT*pScrollInputs->screenY;
    minimaprect.w = SCR_TILE_WIDTH*MINIMAP_TILE_WIDTH;
    minimaprect.h = SCR_TILE_HEIGHT*MINIMAP_TILE_HEIGHT;

    cSurface::DrawRectangle(mScreen,&minimaprect,150,1);
}

void cOutput::drawSightmask()
{
    SDL_Surface* Minmap;
    SDL_Surface* Tile;
    for(int i=0;i < WORLD_WIDTH;i++)
    {
        for(int j=0;j < WORLD_HEIGHT;j++)
        {
            if(sightMask[i][j] < -1)
            {
                //Draw sightmask on Minimap
                Minmap = sightMaskMinimapGraphics;
                cSurface::Draw(mScreen,Minmap,MINIMAP_X + MINIMAP_TILE_WIDTH*i,MINIMAP_Y + MINIMAP_TILE_HEIGHT*j);
                //if on the screen draw a tile there
                if(i>=pScrollInputs->screenX && i < pScrollInputs->screenX+SCR_TILE_WIDTH
                   && j>=pScrollInputs->screenY && j<pScrollInputs->screenY+SCR_TILE_HEIGHT )
                {
                    //Graphic of the corresponding field
                    Tile = sightMaskGraphics;
                    //Draw it at the right spot
                    cSurface::Draw(mScreen,Tile,(i-pScrollInputs->screenX)*TILE_WIDTH,(j-pScrollInputs->screenY)*TILE_HEIGHT);
                }
            }

        }
    }
}

void cOutput::drawSelectionRectangle(SDL_Rect* rect)
{
    //SDL_FillRect(mScreen,rect,256);
    cSurface::DrawRectangle(mScreen,rect,0,2);
}

void cOutput::drawHealthBars(cUnit* Unit,int dX, int dY)
{
    bool applyHB;

    //TODO make different modes
    switch(healthBarSettings)
    {
        case HEALTHBARS_ALWAYS :
        {
            applyHB = true;
            break;
        }
        case HEALTHBARS_SELECTED :
        {
            if(Unit->checkSelected())
            {
                applyHB = true;
            }
            else
            {
                applyHB = false;
            }
            break;
        }
    }

    if(applyHB)
    {
        int ratio;
        ratio = (Unit->getHealth() *HEALTHBAR_WIDTH) / Unit->getMaxHealth();

        //define the size for the life that is left
        SDL_Rect rect;
        rect.x = dX + HEALTHBAR_X;
        rect.y = dY + HEALTHBAR_Y;
        rect.w = ratio;
        rect.h = HEALTHBAR_HEIGHT;
        //fill the rectangle with the health that is left; calculate the right color
        int color;
        int strength;
        //TODO depending on color-depth
        strength = ratio * 255 / HEALTHBAR_WIDTH;
        color = (256-strength) * 256*256 + strength * 256;
        SDL_FillRect(mScreen,&rect,color);
        //define the black rect;
        rect.x = dX + HEALTHBAR_X + ratio;
        rect.y = dY + HEALTHBAR_Y;
        rect.w = HEALTHBAR_WIDTH-ratio;
        rect.h = HEALTHBAR_HEIGHT;
        //Draw it
        SDL_FillRect(mScreen,&rect,0);
        if(Unit->getMaxMana() > 0)
        {
            ratio = (Unit->getMana() *HEALTHBAR_WIDTH) / Unit->getMaxMana();
            //define the size for the mana that is left
            SDL_Rect rect;
            rect.x = dX + HEALTHBAR_X;
            rect.y = dY + HEALTHBAR_Y + HEALTHBAR_HEIGHT;
            rect.w = ratio;
            rect.h = HEALTHBAR_HEIGHT;
            //fill the rectangle with the health that is left; calculate the right color
            int color;
            int strength;
            //TODO depending on color-depth
            strength = ratio * 255 / HEALTHBAR_WIDTH;
            color = (256-strength)* 256*256 + 0 * 256 + strength;
            SDL_FillRect(mScreen,&rect,color);
            SDL_FillRect(mScreen,&rect,color);
            //define the black rect;
            rect.x = dX + HEALTHBAR_X + ratio;
            rect.y = dY + HEALTHBAR_Y + HEALTHBAR_HEIGHT;
            rect.w = HEALTHBAR_WIDTH-ratio;
            rect.h = HEALTHBAR_HEIGHT;
            //Draw it
            SDL_FillRect(mScreen,&rect,0);
        }

    }
}

void cOutput::drawUnitDescription(cUnit* selectedUnit)
{
    SDL_Surface* picture;
    if(selectedUnit != NULL)
    {
        //basic picture
        int thistype;
        thistype = selectedUnit->getType();
        picture = UnitPicture[thistype][selectedUnit->getOwner()];
        cSurface::Draw(mScreen,picture,UNIT_PICTURE_X,UNIT_PICTURE_Y);

        //healthbars
        int ratio;
        ratio = (selectedUnit->getHealth() *UNIT_PIC_HEALTHBAR_HEIGHT) / selectedUnit->getMaxHealth();

        //define the size for the life that is left
        SDL_Rect rect;
        rect.x = UNIT_PICTURE_X + UNIT_PIC_HEALTHBAR_X ;
        rect.y = UNIT_PICTURE_Y + UNIT_PIC_HEALTHBAR_Y+(UNIT_PIC_HEALTHBAR_HEIGHT-ratio);
        rect.w = UNIT_PIC_HEALTHBAR_WIDTH;
        rect.h = ratio;
        //fill the rectangle with the health that is left; calculate the right color
        int color;
        int strength;
        //TODO depending on color-depth
        strength = ratio * 255 / UNIT_PIC_HEALTHBAR_HEIGHT;
        color = (256-strength) * 256*256 + strength * 256;
        SDL_FillRect(mScreen,&rect,color);
        if(selectedUnit->getMaxMana() > 0)
        {
            ratio = (selectedUnit->getMana() *UNIT_PIC_HEALTHBAR_HEIGHT) / selectedUnit->getMaxMana();

            //define the size for the life that is left
            SDL_Rect rect;
            rect.x = UNIT_PICTURE_X + UNIT_PIC_MANABAR_X ;
            rect.y = UNIT_PICTURE_Y + UNIT_PIC_MANABAR_Y+(UNIT_PIC_MANABAR_HEIGHT-ratio);
            rect.w = UNIT_PIC_MANABAR_WIDTH;
            rect.h = ratio;
            //fill the rectangle with the health that is left; calculate the right color
            int color;
            int strength;
            //TODO depending on color-depth
            strength = ratio * 255 / UNIT_PIC_HEALTHBAR_HEIGHT;
            color = (256-strength)* 256*256 + 0 * 256 + strength;
            SDL_FillRect(mScreen,&rect,color);
        }
    }
}

void cOutput::drawUnitMinipictures()
{
    cSelection* thisSel;
    thisSel = selectionListHead;
    SDL_Surface* minpic;
    cUnit* thisUnit;
    int nowX,nowY;
    int color;
    //integers to check how far we have already cycled;
    int typecycle = 0;
    int oldtype = -1;
    int prioOrNot = 0;
    // which picture do we draw
    SDL_Rect rect;
    rect.w = MINIPIC_WIDTH;
    rect.h = MINIPIC_HEIGHT;
    //cycle through the units
    for(int i=0;i<selectedNumber && i<MINIPIC_COLUMNS*MINIPIC_ROWS;i++)
    {
        int j = i%MINIPIC_COLUMNS;
        //calculate the x where the minipic will be drawn to
        nowX = MINIPIC_X + MINIPIC_WIDTH * j;
        nowY = MINIPIC_Y + MINIPIC_HEIGHT * ((i-j)/MINIPIC_COLUMNS);
        thisSel = thisSel->getSuccessor();
        if(thisSel != NULL){thisUnit = thisSel->getContent();}
        if(thisUnit!= NULL)
        {
            if(thisUnit->getType() != oldtype)
            {
                oldtype = thisUnit->getType();
                typecycle++;
                if(typecycle == priorizedNumber)
                {
                    prioOrNot = 1;
                }
                else
                {
                    prioOrNot = 0;
                }
            }
            minpic = UnitMiniPic[thisUnit->getType()][thisUnit->getOwner()][prioOrNot];
            rect.x = nowX;
            rect.y = nowY;
            color = (thisUnit->getHealth() *255) / thisUnit->getMaxHealth();
            color = (256-color) * 256*256 + color * 256;
            //calcualte the damage color
            SDL_FillRect(mScreen,&rect,color);
            //Draw the right minipic
            cSurface::Draw(mScreen,minpic,nowX,nowY);
            ///TODO Draw the greyvalue to further visualize damage
        }
    }
}

void cOutput::applyDrawAoe(int posX,int posY,int rangeSq)
{
    float range = sqrt(rangeSq);
    int rangeInt = int(range);
    Error("DrawAoe X",posX);
    Error("DrawAoe Y",posY);
    for(int i=-rangeInt;i<=rangeInt;i++)
    {
        for(int j=-rangeInt;j<=rangeInt;j++)
        {
            if(realDist(i,j,0,0)<=range
               && (posX+i>0) && (posY+j>0) && (posX+i<SCR_TILE_WIDTH) && (posY+j<SCR_TILE_HEIGHT))
               {
                   cSurface::Draw(mScreen,aoeMarker,(posX+i)*TILE_WIDTH + 1,(posY+j)*TILE_HEIGHT + 1);
               }
        }
    }
}

void cOutput::drawGrid(cUnit* selectedUnit,Uint32 t)
{
    //int UnitType = selectedUnit->getType();
    int printX,printY,takeX,takeY;
    int abilities = selectedUnit->getNumberOfAblilities();
    //Draw Ability1
    printX = GRID_X + GRID_HALFSPACE_X;
    printY = GRID_Y + GRID_HALFSPACE_Y;
    cSurface::Draw(mScreen,AbilityPic[selectedUnit->getEnttype(0)],printX,printY);
    //Draw Ability2
    if(abilities>=2)
    {
        printX = GRID_X + 3*GRID_HALFSPACE_X + GRID_PIC_WIDTH;
        printY = GRID_Y + GRID_HALFSPACE_Y;
        cSurface::Draw(mScreen,AbilityPic[selectedUnit->getEnttype(1)],printX,printY);
    }
    //Draw Ability3
    if(abilities>=3)
    {
        printX = GRID_X + 5*GRID_HALFSPACE_X + 2*GRID_PIC_WIDTH;
        printY = GRID_Y + GRID_HALFSPACE_Y;
        cSurface::Draw(mScreen,AbilityPic[selectedUnit->getEnttype(2)],printX,printY);
    }
    if(abilities>=4)
    {
        printX = GRID_X + 7*GRID_HALFSPACE_X + 3*GRID_PIC_WIDTH;
        printY = GRID_Y + GRID_HALFSPACE_Y;
        cSurface::Draw(mScreen,AbilityPic[selectedUnit->getEnttype(3)],printX,printY);
    }
    //Draw Ability4
    //TODO change size
    //Draw Attack
    printX = GRID_X + GRID_HALFSPACE_X;
    printY = GRID_Y + 3*GRID_HALFSPACE_Y + GRID_PIC_HEIGHT;
    cSurface::Draw(mScreen,StandardPic[0],printX,printY);
    //Draw Stop
    printX = GRID_X + 3*GRID_HALFSPACE_X + GRID_PIC_WIDTH;
    printY = GRID_Y + 3*GRID_HALFSPACE_Y + GRID_PIC_HEIGHT;
    cSurface::Draw(mScreen,StandardPic[1],printX,printY);
    //Draw Move
    printX = GRID_X + 5*GRID_HALFSPACE_X + 2*GRID_PIC_WIDTH;
    printY = GRID_Y + 3*GRID_HALFSPACE_Y + GRID_PIC_HEIGHT;
    cSurface::Draw(mScreen,StandardPic[2],printX,printY);
    printX = GRID_X + 7*GRID_HALFSPACE_X + 3*GRID_PIC_WIDTH;
    printY = GRID_Y + 3*GRID_HALFSPACE_Y + GRID_PIC_HEIGHT;
    cSurface::Draw(mScreen,StandardPic[3],printX,printY);

    //Animate active ability
    GridAnimTime -= t;
    if(GridAnimTime <= 0) {GridAnimStepNow +=1;GridAnimTime+=GRID_ANIMATION_TICKLENGTH;}
    if(GridAnimStepNow >= GridAnimSteps) GridAnimStepNow =0;

    printX = GRID_X + GRID_HALFSPACE_X;
    printY = GRID_Y + GRID_HALFSPACE_Y;
    abilities = selectedUnit->getActiveAbility();
    printX = printX + 2*abilities*GRID_HALFSPACE_X + abilities*GRID_PIC_WIDTH;
    takeX = GridAnimStepNow*GRID_PIC_WIDTH;
    takeY = 0;

    cSurface::Draw(mScreen,GridAnim,printX,printY,takeX,takeY,GRID_PIC_WIDTH,GRID_PIC_HEIGHT);
}

void cOutput::drawEntities()
{
    cEntity* Entity;
    int enttype;
    SDL_Surface* Object;
    int offsetX,offsetY;
    int drawToX,drawToY,takeX,takeY;
    for(int i=0;i<pGameModel->getNumberOfEntities();i++)
    {
        //read the Unit
        Entity = pGameModel->getEntity(i);
        //read the obj and actiontype
        enttype = Entity->getType();

        //get the corresponding graphic from the list
        Object = EntityGraphic[enttype];

        //draw it to the screen according to its animationstep
        offsetX = Entity->getOffsetX();
        offsetY = Entity->getOffsetY();
        drawToX = (Entity->getX()-pScrollInputs->screenX)*TILE_WIDTH+offsetX;
        drawToY = (Entity->getY()-pScrollInputs->screenY)*TILE_HEIGHT+offsetY;
        takeX = 0; // TODO direction (Entity->getDirection()-1)*TILE_WIDTH;
        takeY = (Entity->getAnimStepNow())*TILE_HEIGHT;
        cSurface::Draw(mScreen,Object,drawToX,drawToY,takeX,takeY,TILE_WIDTH,TILE_HEIGHT);
    }
}

void cOutput::drawFrameBattleGameScreen(Uint32 ticks)
{
   //Draw HUD
    cSurface::Draw(mScreen,BattleGameBackground,0,0);

    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        cSurface::Draw(mScreen,UnitMiniPic[i][thisOwner][0],BATTLE_GAME_AVAILABLE_X + i*MINIPIC_WIDTH,BATTLE_GAME_AVAILABLE_Y);
    }
    for(int i=0;i<UNIT_TYPE_NUMBER;i++)
    {
        if(pBattleGameModel->readyUnits[i] > 0)
        {
            if(i==pBattleGameInputs->placeUnit)
            {
                cSurface::Draw(mScreen,UnitMiniPic[i][thisOwner][1],BATTLE_GAME_UNUSED_X + i*MINIPIC_WIDTH,BATTLE_GAME_UNUSED_Y);
            }
            else
            {
                cSurface::Draw(mScreen,UnitMiniPic[i][thisOwner][0],BATTLE_GAME_UNUSED_X + i*MINIPIC_WIDTH,BATTLE_GAME_UNUSED_Y);
            }
            //TODO Number
        }
        else
        {
            cSurface::Draw(mScreen,UnitMiniPic[i][thisOwner][0],BATTLE_GAME_UNUSED_X + i*MINIPIC_WIDTH,BATTLE_GAME_UNUSED_Y);
            cSurface::Draw(mScreen,forbiddenMask,BATTLE_GAME_UNUSED_X + i*MINIPIC_WIDTH,BATTLE_GAME_UNUSED_Y);
        }

    }
    if(pBattleGameModel->ready)
    {
        cSurface::Draw(mScreen,readyButton[1],BATTLE_GAME_GOBUTTON_X,BATTLE_GAME_GOBUTTON_Y);
    }
    else
    {
        cSurface::Draw(mScreen,readyButton[0],BATTLE_GAME_GOBUTTON_X,BATTLE_GAME_GOBUTTON_Y);
    }

    for(int i=0;i<START_SECTOR_X;i++)
    {
        for(int j=0;j<START_SECTOR_Y;j++)
        {
            if(pBattleGameModel->startSector[i][j] == -2)
            {
                cSurface::Draw(mScreen,blockedSector,BATTLE_GAME_SECTOR_X+i*TILE_WIDTH,BATTLE_GAME_SECTOR_Y+j*TILE_HEIGHT);
            }
            if(pBattleGameModel->startSector[i][j] == -1)
            {
                cSurface::Draw(mScreen,EmptySector,BATTLE_GAME_SECTOR_X+i*TILE_WIDTH,BATTLE_GAME_SECTOR_Y+j*TILE_HEIGHT);
            }
            if(pBattleGameModel->startSector[i][j] >= 0)
            {
                cSurface::Draw(mScreen,UnitGraphics[pBattleGameModel->startSector[i][j]][ACTION_REST][thisOwner]
                               ,BATTLE_GAME_SECTOR_X+i*TILE_WIDTH,BATTLE_GAME_SECTOR_Y+j*TILE_HEIGHT
                               ,4*TILE_WIDTH,0,TILE_WIDTH,TILE_HEIGHT);

            }
        }
    }

    if(pBattleGameInputs->placeUnit != -1)
    {
        cSurface::Draw(mScreen,UnitPicture[pBattleGameInputs->placeUnit][thisOwner],BATTLE_GAME_DESCRIPTION_X,BATTLE_GAME_DESCRIPTION_Y);
    }

    cSurface::Draw(mScreen,mMinimap,BATTLE_GAME_MINIMAP_X,BATTLE_GAME_MINIMAP_Y);

    // Draw the cursor
    cSurface::Draw(mScreen,cursorGraphicBattleGame,pMousePosition->X+CURSOR_OFFSET_X,pMousePosition->Y+CURSOR_OFFSET_Y);

    //Show the new screen
    SDL_Flip(mScreen);
}

void cOutput::setDrawAoe(int aoe)
{
    drawAoe = aoe;
}

void cOutput::setSelection(cSelection* selectionListH, int selectedNum, int selectedBui, int typeNum, int priorizedNum)
{
    selectionListHead = selectionListH;
    selectedNumber = selectedNum;
    selectedBuildings = selectedBui;
    typeNumbers = typeNum;
    priorizedNumber = priorizedNum;
}

void cOutput::setActiveScreen(int scr)
{
    activeScreen = scr;
}

bool cOutput::CleanUpVideo()
{
    //Free all surfaces
    for(int i=0;i<TILE_TYPE_MAX;i++)
    {
        SDL_FreeSurface(TileGraphics[i]);
    }
    for(int i=0;i<OBSTACLE_TYPE_MAX;i++)
    {
        SDL_FreeSurface(ObstacleGraphics[i]);
    }
    for(int j=0;j<NMBR_ACTIONS;j++)
    {
        for(int i=0;i<UNIT_TYPE_MAX;i++)
        {
            for(int k=0;k<=MAX_NUMBER_PLAYERS;k++)
            {
                SDL_FreeSurface(UnitGraphics[i][j][k]);
            }
        }
    }
	SDL_FreeSurface(mScreen);
	SDL_Quit();
	return 1;
}
