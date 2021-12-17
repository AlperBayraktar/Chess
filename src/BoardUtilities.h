#pragma once

#include <vector>

#include "Square.h"
#include "Colors.h"
#include <SDL.h>

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

// Resets board
void ResetBoard(Square squares[8][8], ChessPiece defaultBoard[8][8], SDL_Renderer* renderer)
{
    for (int row=0; row<8; row++)
    {
        for (int col=0; col<8; col++)
        {
            Square* square = &squares[row][col];
            // Set grid position
            square->SetGridPosition(row, col);

            // Set bg color
            ResetSquareBg(square);

            // Render
            square->piece = &defaultBoard[row][col];
            square->Render(renderer);
        }
    }
}

// If given square is empty, adds it to moveableSquares vector
// 0: Square wasn't moveable
// 1: Square was moveable
int MakeMoveableIfEmpty(Square* square, std::vector<Square*>* moveableSquares)
{
    if (square->piece->type == "EMPTY")
    {
        moveableSquares->push_back(square);
        return 1;
    }
    return 0;
}

// If given square is opponent's, adds it to takeableSquares vector
// 0: Square wasn't takeable
// 1: Square was takeable
int MakeTakeableIfOpponent(Square* square, std::vector<Square*>* takeableSquares, const char* opponent)
{
    if (square->piece->color == opponent)
    {
        takeableSquares->push_back(square);
        return 1;
    }
    return 0;
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


void FindMoveableSquaresForRookOnAnAxis(Square Squares[8][8], Square* clickedSquare, const char* turn, const char* opponent, std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, const char* axis, const char* incOrDec, bool isKing)
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
        
        if (MakeTakeableIfOpponent(temporarySquare, takeableSquares, opponent)
        ||
        !MakeMoveableIfEmpty(temporarySquare, moveableSquares)
        ||
        isKing
        )
        {
            break;
        }
    }
}


void FindMoveableSquaresForBishopOnAnAxis(Square Squares[8][8], Square* clickedSquare, const char* turn, const char* opponent, std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, const char* incOrDecRow, const char* incOrDecCol, bool isKing)
{
    int row = clickedSquare->row;
    int col = clickedSquare->col;

    while (true)
    {
        incOrDecRow == "inc" ? row++ : row--;
        incOrDecCol == "inc" ? col++ : col--;
        
        if (row < 0 || row > 7 || col > 7 || col < 0) { break; }

        Square* square = &Squares[row][col];

        if ( 
            MakeTakeableIfOpponent(square, takeableSquares, opponent) ||
            !MakeMoveableIfEmpty(square, moveableSquares)
            ||
            isKing
            ) { break; }
    }
}

void SelectBishop(Square Squares[8][8], Square* clickedSquare, const char* turn, const char* opponent, std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, bool isKing)
{
    FindMoveableSquaresForBishopOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "dec", "inc", isKing);
    FindMoveableSquaresForBishopOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "dec", "dec", isKing);

    FindMoveableSquaresForBishopOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "inc", "inc", isKing);
    FindMoveableSquaresForBishopOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "inc", "dec", isKing);
}

void SelectRook(Square Squares[8][8], Square* clickedSquare, const char* turn, const char* opponent, std::vector<Square*>* takeableSquares, std::vector<Square*>* moveableSquares, bool isKing)
{
    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "row", "inc", isKing);
    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "row", "dec", isKing);

    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "col", "inc", isKing);
    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, takeableSquares, moveableSquares, "col", "dec", isKing);
}