#include "piece.h"
#include "board.h"

Piece::Piece(PieceType t, Color c) : pt{t}, color{c} {
    // Initialize the piece with the given color and type
}

bool Piece::makeMove(int r, int c, Board *board){
    return 0;
}

bool Piece::isEnpassed(int r, int c, Board* board){
    return false;
}

bool Piece::isPromotion(int r, int c, Board *board, char promotion){
    return false;
}

bool Piece::isCastle(int r, int c, Board* board){
    return false;
}

int Piece::getRow() const {
    return row;
}

int Piece::getCol() const {
    return col;
}

Color Piece::getColor() const { // NEW???
    return color;
}

PieceType Piece::getType() const {
    return pt;
}

void Piece::setType(PieceType t){
    pt = t;
}

void Piece::setCoords(int r, int c){
    row = r;
    col = c;
}

int Piece::getCount() const { return moveCount; }
void Piece::setCount(int n) { moveCount = n; }

Piece::~Piece() {}


