#include "knightpiece.h"
#include "piece.h"
#include "board.h"
using namespace std;

KnightPiece::KnightPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool KnightPiece::isEnpassed(int r, int c, Board* board) { return false; }

bool KnightPiece::isPromotion(int r, int c, Board* board, char promotion) { return false; }

bool KnightPiece::makeMove(int r, int c, Board* board) {
    // get diff in movemenet
    int rowDiff = abs(getRow() - r);
    int colDiff = abs(getCol() - c);

    if ((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1)) {
        return true;
    }
    return false;
}

bool KnightPiece::isCastle(int r, int c, Board* board) { return false; }

bool KnightPiece::moveable(int r, int c, Board* board, char promotion) {
    return makeMove(r, c, board);
}

bool KnightPiece::moveProduceCheck(int r, int c, Board* board, char promotion) {
    Board testBoard = *board;
    Piece *start = testBoard.getPiece(getRow(), getCol());
    int pColor = (start->getColor() == Color::White ? 1 : 2);
    
    if (makeMove(r, c, board)) {
        
        testBoard.removePiece(r, c);
        start->setCoords(r, c);
        start->setCount(start->getCount() + 1);
        testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);  
        testBoard.update(getRow(), getCol(), r, c, start);
        if (testBoard.isCheck(pColor)) return true;
    }
    return false;
}

