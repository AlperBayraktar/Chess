#pragma once
#include <SDL.h>

// These colors has CHESS prefix because otherwise it occurs an redefiniton error when SDL_GUI is included
static SDL_Color CHESS_BLACK = { 168, 108, 59, 255 };
static SDL_Color CHESS_WHITE = { 255, 255, 255, 1 };
static SDL_Color CHESS_BLUE = { 25, 106, 198, 8  };
static SDL_Color CHESS_GREEN = { 45, 231, 30, 8 };
static SDL_Color CHESS_RED  = { 241, 22, 22, 8 };