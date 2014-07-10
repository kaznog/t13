//
//  Constant.h
//
//  Created by kaznog on 2014/02/25.
//
//

#ifndef ccs2dx_v3tutorial001_Constant_h
#define ccs2dx_v3tutorial001_Constant_h

#include "cocos2d.h"
#include "chipmunk.h"

// Game state
#define STATE_UNKNOWN 0
#define STATE_PAUSE  1
#define STATE_PLAYING 2
#define STATE_GAME_OVER 3
#define STATE_LEVEL_COMPLETE 4

// parent is scroll batch node
#define Z_WATERMELON 5
#define Z_COIN 8
#define Z_FINISH 9
#define Z_CHASSIS 10
#define Z_WHEEL 11

// parent is scroll node (parallax node)
#define Z_MOUNTAINS 0
#define Z_SPRITES 10
#define Z_TERRAIN 20
#define Z_DEBUG_PHYSICS 50

// parent is game layer
#define Z_SCROLL 10
#define Z_SMOKE 15
#define Z_HUD 20
#define Z_LABEL 30
#define Z_DEBUG_MENU 30

#define COLLISION_TYPE_CAR 1
#define COLLISION_TYPE_COIN 2
#define COLLISION_TYPE_WATERMELON 3
#define COLLISION_TYPE_FLOOR 4
#define COLLISION_TYPE_FINISH 5

// Create some collision rules for fancy layer based filtering.
// There is more information about how this works in the Chipmunk docs.
#define COLLISION_RULE_TERRAIN_BUGGY 1 << 0
#define COLLISION_RULE_BUGGY_ONLY 1 << 1

// Bitwise or the rules together to get the layers for a certain shape type.
#define COLLISION_LAYERS_TERRAIN COLLISION_RULE_TERRAIN_BUGGY
#define COLLISION_LAYERS_BUGGY (COLLISION_RULE_TERRAIN_BUGGY | COLLISION_RULE_BUGGY_ONLY)

// Some constants for controlling the car and world:
#define GRAVITY 1200.0f
#define WHEEL_MASS 0.25f
#define CHASSIS_MASS 0.7f
#define FRONT_SPRING 150.0f
#define FRONT_DAMPING 2.0f
#define COG_ADJUSTMENT cpv(0.0, -10.0)
#define REAR_SPRING 120.0f
#define REAR_DAMPING 2.4f
#define ROLLING_FRICTION 5e2f
#define ENGINE_MAX_TORQUE 5.0e4f
#define ENGINE_MAX_W 60.0f
#define BRAKING_TORQUE 3.0e4f
#define DIFFERENTIAL_TORQUE 0.4f

// Groups
#define GROUP_BUGGY 1
#define GROUP_COIN 2

#define WATERMELON_MASS 0.05

// Node Tags (used by CocosBuilder)
#define SCORE_LABEL_TAG 10
#define TIME_LABEL_TAG 11
#define TITLE_TAG 50

#define MUSIC_STORAGE_KEY "MUSIC"

#define S_COIN_PLIST "coins.plist"
#define S_GAME_MUSIC_MP3 "game-music.mp3"
#define S_LEVELCOMPLETE_MP3 "LevelComplete.mp3"
#define S_PICKUP_COIN_MP3 "pickup_coin.mp3"
#define S_GAME_OVER_MP3 "GameOver.mp3"

#define S_GAS40_FNT "Gas40.fnt"
#define S_ABADI40_FNT "Abadi40.fnt"
#define S_KONQA32_FNT "konqa32.fnt"

#define S_CCB_HUD "HUD.ccbi"
#define S_CCB_MAINMENU "MainMenu.ccbi"
#define S_CCB_ABOUT "About.ccbi"

#define S_TITLE_CLASSNAME "TitleScene"
#define S_ABOUT_CLASSNAME "AboutScene"

#endif
