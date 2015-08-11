#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

//Network sizes
#define NET_CORESIZE 32
#define NET_PACKSIZE 40
//resendingspeed TODO; calculate average ping
#define RESENDINGSPEED 300

#define MIN_FRAME_LENGTH 100

#define MAX_NUMBER_PLAYERS 2

//Size of the Tiles on the map and the units and so on
#define TILE_WIDTH 40
#define TILE_HEIGHT 40

#define CURSOR_OFFSET_X -11
#define CURSOR_OFFSET_Y -11
//cursortypes
#define CURSOR_STATES 4

#define CURSOR_NORMAL 0
#define CURSOR_MOVE 1
#define CURSOR_ATTACK 2
#define CURSOR_CAST 3

// the maximally allowed units for this compilation
#define MAX_UNITS 100

//the Number of maximal actions of units
#define NMBR_MIN_ACTIONS -2
#define NMBR_ACTIONS 8
//Action ID's
#define ACTION_DYING         0
#define ACTION_HOLDPOSITION  1
#define ACTION_REST          2
#define ACTION_WAIT          3
#define ACTION_MOVE          4
#define ACTION_ATTACKMOVE    5
#define ACTION_INTERCEPT     6
#define ACTION_FIGHT         7
//pseudoactions > 16
#define ACTION_CAST          16
//a special type for taunting
#define ACTION_INTERCEPT_ZERO 106
//length of the action queue
#define QUEUE_ACTION_LENGTH 10

//the amount of waiting ticks when the path is blocked by a different unit
#define GENERAL_WAITING_TIME 10

//maximal Number of tiles Units, Obstacles and Entities
#define TILE_TYPE_MAX 7
#define UNIT_TYPE_MAX 3
#define OBSTACLE_TYPE_MAX 6
#define ENTITY_TYPE_MAX 6
#define BUILDING_TYPE_MAX 1

//Object type identifiers
#define OBJ_TYPE_UNIT 1
#define OBJ_TYPE_OBSTACLE 2
#define OBJ_TYPE_BUILDING_ACCESS 3
#define OBJ_TYPE_BUILDING_WALL 4
#define OBJ_TYPE_BUILDING_WITH_UNIT 5

//Map size; TODO read everything from file; dynamical
#define MAX_WORLD_WIDTH  240
#define MAX_WORLD_HEIGHT 240

//HUD Specifications
#define UNIT_PICTURE_X 247
#define UNIT_PICTURE_Y 524

#define UNIT_PIC_HEALTHBAR_X 9
#define UNIT_PIC_HEALTHBAR_Y 25

#define UNIT_PIC_HEALTHBAR_HEIGHT 100
#define UNIT_PIC_HEALTHBAR_WIDTH 10

#define UNIT_PIC_MANABAR_X 131
#define UNIT_PIC_MANABAR_Y 25

#define UNIT_PIC_MANABAR_HEIGHT 100
#define UNIT_PIC_MANABAR_WIDTH 10

#define MINIPIC_WIDTH 60
#define MINIPIC_HEIGHT 60

#define MINIPIC_ROWS 4
#define MINIPIC_COLUMNS 6

#define MINIPIC_X 410
#define MINIPIC_Y 524

#define BUILDING_MAX_WIDTH 8
#define BUILDING_MAX_HEIGHT 8

#define BUILDING_LAYOUT_WALL 0
#define BUILDING_LAYOUT_ACCESS 1

//Servermode; which number in the mode variable means which type of state
#define MODE_SINGPLAY 0
#define MODE_SERVER   1
#define MODE_CLIENT   2
#define MODE_REPLAY   3

//This is used to address the changetype
#define CHANGE_NEW              1
#define CHANGE_REMOVE           2
#define CHANGE_POSITION         3
#define CHANGE_GOAL             4
#define CHANGE_UNITGOAL         5
#define CHANGE_MOVETYPE         6
#define CHANGE_HEALTH           7
#define CHANGE_CREATE_ENT       8
#define CHANGE_MANA             9
#define CHANGE_ACTIVE_ABILITY   10
#define CHANGE_CAST_ONCE        11

//This is used to address the commandtype
#define COMMAND_CLIENT_READY    102
#define COMMAND_INIT_SPAWN      101
#define COMMAND_TERMINATE_GAME  100
#define COMMAND_STOP             1
#define COMMAND_HOLDPOSI         2
#define COMMAND_MOVE             3
#define COMMAND_ATTACK           4
#define COMMAND_ACTIVATE_ABILITY 5
#define COMMAND_CAST_0           6
#define COMMAND_CAST_1           7
#define COMMAND_CAST_2           8
#define COMMAND_CAST_3           9

//Ability Number realted things
#define ABILITY_MAX   4

//maximal Entity properties
#define ENT_MAX_MODIF 2
// entity properties; powers of 2 that will be switched on or off; wordlength will be 64
#define ENT_PROP_SELF       4096
#define ENT_PROP_INSTANT    2048
#define ENT_PROP_AOE        1024
#define ENT_PROP_GROUND      512
#define ENT_PROP_FRIENDLY    256
//
#define ENT_MODIF_DAMAGE             1
#define ENT_MODIF_MAGICDAMAGE        2
#define ENT_MODIF_BUFF_SPEED         3
#define ENT_MODIF_HEAL               256
#define ENT_MODIF_GROUND_BURST       512
#define ENT_MODIF_GROUND_BURST_MAGIC 513
#define ENT_MODIF_SELF_SPEED         4099

#define MAX_ENT 500

#define REG_TICK_LENGTH     1000

//show healthbar settings
#define HEALTHBARS_ALWAYS 1
#define HEALTHBARS_SELECTED 2

#define HEALTHBAR_HEIGHT 2
#define HEALTHBAR_WIDTH 38
#define HEALTHBAR_X 1
#define HEALTHBAR_Y 1

#define GRID_ANIMATION_TICKLENGTH 100

#define GRID_X 777
#define GRID_Y 524

#define GRID_PIC_WIDTH  58
#define GRID_PIC_HEIGHT 58

#define GRID_HALFSPACE_X 1
#define GRID_HALFSPACE_Y 1

//Battle Game screen settings
#define BATTLE_GAME_TIME 60000

#define START_SECTOR_X_MAX 15
#define START_SECTOR_Y_MAX 15

#define BATTLE_GAME_AVAILABLE_X 100
#define BATTLE_GAME_AVAILABLE_Y 150

#define BATTLE_GAME_HERO_X 100
#define BATTLE_GAME_HERO_Y 30

#define BATTLE_GAME_PIC_WIDTH  60
#define BATTLE_GAME_PIC_HEIGHT 60

#define BATTLE_GAME_UNUSED_X 100
#define BATTLE_GAME_UNUSED_Y 250

#define BATTLE_GAME_SECTOR_X 100
#define BATTLE_GAME_SECTOR_Y 340

#define BATTLE_GAME_CLOCK_X 838
#define BATTLE_GAME_CLOCK_Y 25
#define BATTLE_GAME_CLOCK_WIDTH 160
#define BATTLE_GAME_CLOCK_HEIGHT 70

#define BATTLE_GAME_GOBUTTON_X 838
#define BATTLE_GAME_GOBUTTON_Y 95
#define BATTLE_GAME_GOBUTTON_WIDTH 160
#define BATTLE_GAME_GOBUTTON_HEIGHT 70

#define BATTLE_GAME_DESCRIPTION_X 560
#define BATTLE_GAME_DESCRIPTION_Y 340

extern int MINIMAP_WIDTH;
extern int MINIMAP_HEIGHT;

extern int BATTLE_GAME_MINIMAP_X;
extern int BATTLE_GAME_MINIMAP_Y;

extern int START_SECTOR_X;
extern int START_SECTOR_Y;

extern int PLAYER_1_START_X;
extern int PLAYER_1_START_Y;
extern int PLAYER_2_START_X;
extern int PLAYER_2_START_Y;

extern int MINIMAP_TILE_WIDTH;
extern int MINIMAP_TILE_HEIGHT;

extern int WORLD_WIDTH;
extern int WORLD_HEIGHT;

extern int MINIMAP_X;
extern int MINIMAP_Y;

extern std::string GRAPHIC_SETTINGS_FILE;
extern std::string MAP_SETTINGS_FILE;
extern std::string UNIT_SETTINGS_FILE;
extern std::string INPUT_SETTINGS_FILE;
extern std::string UNIT_SPEC_FILE[UNIT_TYPE_MAX];
extern std::string ENTITY_SPEC_FILE[ENTITY_TYPE_MAX];
extern std::string BUILDING_SPEC_FILE[BUILDING_TYPE_MAX];
extern std::string NET_SETTINGS_FILE;
extern std::string REPLAY_FILE;
extern std::string BATTLE_GAME_SETTINGS_FILE;
extern std::string PORT;
extern std::string IP_ADDY;

extern int CONTROL_SETTINGS;

extern int TILE_TYPE_NUMBER;
extern int UNIT_TYPE_NUMBER;
extern int OBSTACLE_TYPE_NUMBER;
extern int ENTITY_TYPE_NUMBER;
extern int BUILDING_TYPE_NUMBER;

//width of the screen in tilenumbers
extern int SCR_TILE_WIDTH;
extern int SCR_TILE_HEIGHT;
//Graphic details
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int COLOR_DEPTH;

extern bool WRITE_REPLAY;

#endif // GLOBAL_H_INCLUDED
