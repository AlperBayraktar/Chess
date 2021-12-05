#pragma once

#include <vector>

#include "Square.h"
#include "Colors.h"


// Resets given square's bg to
void ResetSquareBg(Square* square) 
{
    if ((square->row % 2 == 0 && square->col % 2 == 0) || (square->row % 2 == 1 && square->col % 2 == 1))
    {
        square->backgroundColor = WHITE;
    }
    else 
    {
        square->backgroundColor = BLACK;
    }
}

// Deselects the square
// Clears takeableSquares and moveableSquares
// And makes selectedSquare nullptr
void DeselectSquare(std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, SDL_Renderer* renderer, Square* selectedSquare)
{
    for (Square* takeableSquare : *takeableSquares)
    {
        ResetSquareBg(takeableSquare);
        takeableSquare->Render(renderer);
    }
    takeableSquares->clear();

    for (Square* moveableSquare : *moveableSquares)
    {
        ResetSquareBg(moveableSquare);
        moveableSquare->Render(renderer);
    }
    moveableSquares->clear();

    ResetSquareBg(selectedSquare);
    selectedSquare->Render(renderer);
}

// If given square is empty, adds it to moveableSquares vector
void MakeMoveableIfEmpty(Square* square, std::vector<Square*>* moveableSquares)
{
    if (square->piece->type == "EMPTY")
    {
        moveableSquares->push_back(square);
    }
}

// If given square is opponent's, adds it to takeableSquares vector
void MakeTakeableIfOpponent(Square* square, std::vector<Square*>* takeableSquares, const char* turn)
{
    if (square->piece->color == (turn == "WHITE" ? "BLACK" : "WHITE") )
    {
        takeableSquares->push_back(square);
    }
}