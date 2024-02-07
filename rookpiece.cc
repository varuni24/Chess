#include "rookpiece.h"
#include "piece.h"
#include "board.h"
using namespace std;

RookPiece::RookPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool RookPiece::isEnpassed(int r, int c, Board* board) { return false; }

bool RookPiece::isPromotion(int r, int c, Board* board, char promotion) { return false; }

bool RookPiece::makeMove(int r, int c, Board* board) {
    if (r >= 0 && r < 8 && c >= 0 && c < 8) {
        // If box we move to have our own piece color
        if (board->getPiece(r,c) && board->getPiece(r, c)->getColor() == this->getColor()) return false;

        int rowIncrement = (r > getRow()) ? 1 : -1;
        int colIncrement = (c > getCol()) ? 1 : -1;
        int currRow = getRow();
        int currCol = getCol();
        // If moving horizontally:
        if (getRow() == r && getCol() != c) {
            currCol += colIncrement;
            while (currCol != c) {
                if (board->getPiece(currRow, currCol)) return false;
                currCol += colIncrement;
            }
            return true;
        }

        // If moving vertically:
        if (getRow() != r && getCol() == c) {
            currRow += rowIncrement;
            while (currRow != r) {
                if (board->getPiece(currRow, currCol)) return false;
                currRow += rowIncrement;
            }
            return true;
        }
    }
    return false;
}


bool RookPiece::isCastle(int r, int c, Board* board) { return false; }

bool RookPiece::moveable(int r, int c, Board* board, char promotion) { return makeMove(r,c,board); }


bool RookPiece::moveProduceCheck(int r, int c, Board* board, char promotion) {
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


