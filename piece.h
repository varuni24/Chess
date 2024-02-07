#ifndef _PIECE_H_
#define _PIECE_H_

#include "desc.h"

class Board;

class Piece {
    PieceType pt;
    int row = 0;
    int col = 0;
    // NOT IN UML YET
    Color color;
    //bool isCaptured;
    int moveCount = 1;
    public:
    // add
        // Makemove Function
        Piece(PieceType t, Color c);
        virtual bool makeMove(int r, int c, Board *board) = 0;
        virtual bool isEnpassed(int r, int c, Board *board) = 0;
        virtual bool isPromotion(int r, int c, Board *board, char promotion) = 0;
        virtual bool isCastle(int r, int c, Board* board) = 0;
        virtual bool moveable(int r, int c, Board* board, char promotion) = 0;   
        virtual bool moveProduceCheck(int r, int c, Board* board, char promotion) = 0;
        // move thing needs to be added
        int getRow() const;
        int getCol() const;

        Color getColor() const; // NEW !!
        PieceType getType() const;
        void setType(PieceType t);
        void setCoords(int r, int c);
        //void setCaptured();
        int getCount() const;
        void setCount(int n);
        
        virtual ~Piece() = 0;
};

#endif

