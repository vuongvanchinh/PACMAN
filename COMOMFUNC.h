#ifndef COMONFUNC_H
#define COMONFUNC_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include<iostream>
#include<vector>
#include<ctime>
#include<stdlib.h>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 25 * 32;
const int SCREEN_HEIGHT = 25 * 32;

//The dimensions of the level
const int LEVEL_WIDTH = 25 * 32;
const int LEVEL_HEIGHT = 25 * 32;

//Map constants
const int Map_WIDTH = 32;
const int Map_HEIGHT = 32;
const int TOTAL_MapS = 25 * 25;
const int TOTAL_Map_SPRITES = 2;

//The different Map sprites
const int Map_BLACK= 0;
const int Map_BLUE = 1;
const int Map_CENTER =8;
const int Map_TOP = 4;
const int Map_LEFT = 1;
const int Map_TOPLEFT = 11;
const int Map_TOPRIGHT = 5;
const int Map_RIGHT = 6;
const int Map_BOTTOMRIGHT = 7;
const int Map_BOTTOM = 8;
const int Map_BOTTOMLEFT = 9;
const int TOTAL_Pacman_SPRITES =5;
const int TOTAL_GHOST = 4;
using namespace std;

/*
const int Map_BLUE = 2;


*/

bool checkCollision(SDL_Rect a, SDL_Rect b);


//The window we'll be rendering to
static SDL_Window* gWindow = NULL;

//The window renderer

static SDL_Renderer* gRenderer = NULL;


#endif // !COMONFUNC_H
