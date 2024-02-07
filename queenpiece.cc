#include "queenpiece.h"
#include "piece.h"
#include "board.h"
using namespace std;

QueenPiece::QueenPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool QueenPiece::isEnpassed(int r, int c, Board* board) { return false; }

bool QueenPiece::isPromotion(int r, int c, Board* board, char promotion) { return false; }

bool QueenPiece::makeMove(int r, int c, Board* board) {
    // Check if the end coordinates and moving are valid:
    // 1. If end coordinates are out of bounds of the board
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    // 2. If end coordinate is my current coordinate
    if (r == getRow() && c == getCol()) return false;
    // 3. If move to a Cell that has a piece of my own color
    if (board->getPiece(r, c) && board->getPiece(r, c)->getColor() == this->getColor()) return false;

    // Check if the move is either purely horizontal, purely vertical, or purely diagonal
    if (getRow() != r && getCol() != c && abs(getRow() - r) != abs(getCol() - c)) {
        return false;  // Invalid move (not horizontal, vertical, or diagonal)
    }

    // Check if there are no pieces in the path
    int rowIncrement = (r > getRow()) ? 1 : ((r < getRow()) ? -1 : 0);
    int colIncrement = (c > getCol()) ? 1 : ((c < getCol()) ? -1 : 0);
    int currentRow = getRow() + rowIncrement;
    int currentCol = getCol() + colIncrement;

    while (currentRow != r || currentCol != c) {
        if (board->getPiece(currentRow, currentCol)) {
            return false;  // there is a piece in the path
        }
        currentRow += rowIncrement;
        currentCol += colIncrement;
    }
    return true;
}


bool QueenPiece::isCastle(int r, int c, Board* board) { return false; }

bool QueenPiece::moveable(int r, int c, Board* board, char promotion) { 
    return makeMove(r, c, board); 
}

bool QueenPiece::moveProduceCheck(int r, int c, Board* board, char promotion) {
    Board testBoard = *board;
    Piece* start = testBoard.getPiece(getRow(), getCol());
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



