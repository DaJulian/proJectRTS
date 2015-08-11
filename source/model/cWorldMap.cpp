#include "cWorldMap.h"

cWorldMap::cWorldMap()
{

}

void cWorldMap::InitMap(std::string filename,cChangeList* pChList,cWorldMap* pMapObj)
{
    int tile,containtype,onMapType,owner;
    cTile* Tile;
//    cUnit* Unit;
    cObstacle* Obst;
    std::fstream f;
	f.open(filename.c_str(), std::ios::in);
    //safely set everything to 0
    for(int i=0;i< MAX_WORLD_HEIGHT;i++)
    {
        for(int j=0;j< MAX_WORLD_WIDTH;j++)
        {
            Map[j][i] = NULL;
        }
    }

    f >> WORLD_HEIGHT >> WORLD_WIDTH;

	for(int i=0;i< WORLD_HEIGHT;i++)
	{
	    for(int j=0;j< WORLD_WIDTH;j++)
	    {
	        Tile = new cTile;
            f >> tile >> containtype >> onMapType >> owner;
            Map[j][i] = Tile;
            Map[j][i]->Init(tile);
            Map[j][i]->SetPosition(j,i);
            switch(containtype)
            {
                //Empty field
                case 0:
                {
                    break;
                }
                //Spawn Obstacle
                case OBJ_TYPE_OBSTACLE:
                {
                    Obst = new cObstacle(onMapType,Map[j][i]);
                    Map[j][i]->setContain(NULL,Obst);
                    break;
                }
                //Spawn Unit
                case OBJ_TYPE_UNIT:
                {

                }
            }
	    }
	}

	f.close();
}

cTile* cWorldMap::getMap(int x,int y)
{
    return Map[x][y];
}

void cWorldMap::SetMap(int x,int y, cTile* Til)
{
    Map[x][y]=Til;
}

void cWorldMap::ClearMap()
{
    for(int i=0;i< WORLD_WIDTH;i++)
	{
	    for(int j=0;j< WORLD_HEIGHT;j++)
	    {
	        Map[i][j]->setContain(NULL,NULL);
            delete Map[i][j];
	    }
	}
}
