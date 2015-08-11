#include "cFramework.h"

std::string GRAPHIC_SETTINGS_FILE;
std::string MAP_SETTINGS_FILE;
std::string UNIT_SETTINGS_FILE;
std::string INPUT_SETTINGS_FILE;
std::string UNIT_SPEC_FILE[UNIT_TYPE_MAX];
std::string BUILDING_SPEC_FILE[BUILDING_TYPE_MAX];
std::string ENTITY_SPEC_FILE[ENTITY_TYPE_MAX];
std::string NET_SETTINGS_FILE;
std::string REPLAY_FILE;
std::string BATTLE_GAME_SETTINGS_FILE;
std::string PORT;
std::string IP_ADDY;

int START_SECTOR_X;
int START_SECTOR_Y;

int PLAYER_1_START_X;
int PLAYER_1_START_Y;
int PLAYER_2_START_X;
int PLAYER_2_START_Y;

int MINIMAP_TILE_WIDTH;
int MINIMAP_TILE_HEIGHT;

int WORLD_WIDTH;
int WORLD_HEIGHT;

int MINIMAP_X;
int MINIMAP_Y;

int MINIMAP_WIDTH;
int MINIMAP_HEIGHT;

int BATTLE_GAME_MINIMAP_X;
int BATTLE_GAME_MINIMAP_Y;

int TILE_TYPE_NUMBER;
int UNIT_TYPE_NUMBER;
int BUILDING_TYPE_NUMBER;
int OBSTACLE_TYPE_NUMBER;
int ENTITY_TYPE_NUMBER;
//width of the screen in tilenumbers
int SCR_TILE_WIDTH;
int SCR_TILE_HEIGHT;
//Graphic details
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int COLOR_DEPTH;

int CONTROL_SETTINGS;

bool WRITE_REPLAY;

int main( int argc, char* args[] )
{
    //here only the simple calls to the main Framework class
    cFramework workObj;
    workObj.InitFramework();
    workObj.Run();
    workObj.CleanUpFramework();
}
