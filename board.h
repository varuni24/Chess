#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>

#include "cell.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"


class Piece;

class Board{
    std::vector<std::vector<Cell>> theGrid;
    std::vector<Piece *> whitePieces;
    std::vector<Piece *> blackPieces;

    TextDisplay * td;
    GraphicsDisplay *gd;

    Piece* lastMove;  // Used to check for isEnpassed function

    
    public:
    // add
        Board();
        ~Board();
        // copy  ctor;
        Board(const Board &other);
        
        void init();

        void setupInit();

        // need to implement
        // void resetBoard();

        Piece* getPiece(int r, int c);  // NEW!!!!

        bool removePiece(int r, int c);  // NEW!!!!

        Cell& getCell(int r, int c);  // NEW!!!!


        // Mutator to update to the Piece that was just moved.
        void setLastMove(Piece* last);

        // Accessor to retrieve the last Piece that was moved 
        Piece* getLastMove();

        // Update removes the piece that was captured at (r, c) if
        // there were any. It then update the original Cell at 
        // (startR, startC) to not have any piece on it and update 
        // the end coordinate (r, c) to have the piece (p) that just moved. 
        void update(int startR, int startC, int r, int c, Piece* p);

        // Remove Piece if there were any on cell (r, c) 
        // then setup a new piece given the character "p"
        void setupPiece(int r, int c, char p); // NEWW

        // Check if White (1) or Black (2) is Checked
        bool isCheck(int color);

        // Returns true if there is only one king of 
        // each colour on the board
        bool atleastOneKing(int color);

        // If color is 1, check if White is Stalemate 
        // (which means Black is too) and vice versa
        bool isStalemate(int color); // 1 or 2

        // If color is 1, check if White is Checkmate
        // If color is 2, check if Black is Checkmate
        bool isCheckMate(int color); // 1 or 2

        // Returns true if the move result in a Piece 
        // being captured and false otherwise.
        bool isCapturingMove(int startR, int startC, int endR, int endC);

        // Returns true if the piece at (row, color) is underthreat.
        bool isUnderThreat(int row, int col, Color threatColor);

        // add other methods;
        friend std::ostream &operator<<(std::ostream &out, const Board &g);

};

#endif


