#include "bishoppiece.h"
#include "piece.h"
#include "board.h"
using namespace std;

BishopPiece::BishopPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool BishopPiece::makeMove(int r, int c, Board *board) {
    // Check if the end coordinates and moving is valid:
    // 1. If end coordinates are out of bounds of the board
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    // 2. If end coordinate is my current coordinate
    if (r == getRow() && c == getCol()) return false;
    // 3. If move to a Cell that has piece of my own colour
    if (board->getPiece(r, c) && board->getPiece(r, c)->getColor() == this->getColor()) return false;

    // if move is not diagonal
    if (abs(r - getRow()) != abs(c - getCol())) return false;

    // if pieces in path
    int rowIncrement = (r > getRow()) ? 1 : -1;  // diagonal up or down
    int colIncrement = (c > getCol()) ? 1 : -1;  // diagonal left or right
    int currentRow = getRow() + rowIncrement;
    int currentCol = getCol() + colIncrement;

    // Loop to check if there are any pieces in our way
    while (currentRow != r && currentCol != c) {
        // there is a piece in the diagonal path
        if (board->getPiece(currentRow, currentCol)) return false;  
        currentRow += rowIncrement;
        currentCol += colIncrement;
    }
    return true;
}


bool BishopPiece::isEnpassed(int r, int c, Board* board) { return false; }

bool BishopPiece::isPromotion(int r, int c, Board* board, char promotion) { return false; }

bool BishopPiece::isCastle(int r, int c, Board* board) { return false; }

bool BishopPiece::moveable(int r, int c, Board* board, char promotion) { 
    return makeMove(r,c,board); 
}

bool BishopPiece::moveProduceCheck(int r, int c, Board* board, char promotion) {
    // Set up Temporary Board to do the actual move and 
    // see if that puts us in check or not
    Board testBoard = *board;
    Piece* start = testBoard.getPiece(getRow(), getCol());
    int pColor = (start->getColor() == Color::White ? 1 : 2);
    if (makeMove(r, c, &testBoard)) {
        // Remove Piece if there are any at end coordinates
        testBoard.removePiece(r, c);

        // Set our piece to end coordinate
        start->setCoords(r, c);
        start->setCount(start->getCount() + 1);

        // Update the temporary board
        testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
        testBoard.update(getRow(), getCol(), r, c, start);

        // Check if move produce check
        if (testBoard.isCheck(pColor)) return true;
    }
    return false;
}


