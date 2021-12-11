#pragma once

#include <vector>

#include "Square.h"
#include "Colors.h"


// Resets given square's bg
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

// If given square is empty, adds it to moveableSquares vector
void MakeMoveableIfEmpty(Square* square, std::vector<Square*>* moveableSquares)
{
    if (square->piece->type == "EMPTY")
    {
        moveableSquares->push_back(square);
    }
}

// If given square is opponent's, adds it to takeableSquares vector
void MakeTakeableIfOpponent(Square* square, std::vector<Square*>* takeableSquares, const char* opponent)
{
    if (square->piece->color == opponent)
    {
        takeableSquares->push_back(square);
    }
}

// Gets the square and updates moveable and takeables
void HandleSquareSelectMoveableAndTakeables(Square* square,  std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, const char* opponent)
{
    MakeTakeableIfOpponent(square, takeableSquares, opponent);
    MakeMoveableIfEmpty(square, moveableSquares);
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


void FindMoveableSquaresForRookOnAnAxis(Square Squares[8][8], Square* clickedSquare, const char* turn, const char* opponent, std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, const char* axis, const char* incOrDec)
{
    int axisInt = axis == "row" ? clickedSquare->row : clickedSquare->col;
    while (true)
    {
        incOrDec == "inc" ? axisInt++ : axisInt--;

        if ( 
            (incOrDec == "dec" && axisInt < 0 )
            ||
            (incOrDec == "inc" && axisInt > 7 ) )
        {
            break;
        }

        Square* temporarySquare = axis == "row" ? &Squares[axisInt][clickedSquare->col] : &Squares[clickedSquare->row][axisInt];

        HandleSquareSelectMoveableAndTakeables(temporarySquare, takeableSquares, moveableSquares, opponent);
        if (temporarySquare->piece->type != "EMPTY")
        {
            break;
        }
    }
}

