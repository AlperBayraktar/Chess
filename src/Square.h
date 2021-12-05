#pragma once
#include <SDL.h>
#include "ChessPiece.h"

struct Square{
    static const int WIDTH = 60;
    static const int HEIGHT = 60;
    int row, col;
    
    SDL_Rect rect;
    SDL_Color backgroundColor;
    ChessPiece* piece = nullptr;
    
    Square();
    void SetGridPosition(int row_, int col_);
    void Render(SDL_Renderer* renderer);
};