#include "kingpiece.h"
#include "piece.h"
#include "board.h"
using namespace std;

KingPiece::KingPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool KingPiece::isEnpassed(int r, int c, Board* board) { return false; }

bool KingPiece::isPromotion(int r, int c, Board* board, char promotion) { return false; }

bool KingPiece::makeMove(int r, int c, Board* board) { 
    // Check if the end coordinates and moving is valid:
    // 1. If end coordinates are out of bounds of the board
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    // 2. If end coordinate is my current coordinate
    if (r == getRow() && c == getCol()) return false;
    // 3. If move to a Cell that has piece of my own colour
    if (board->getPiece(r,c) && board->getPiece(r, c)->getColor() == this->getColor()) return false;
    
    // If diagonal move in any direction exceeds 1 Cell.
    if (abs(r - getRow()) > 1 || abs(c - getCol()) > 1) return false;
    return true;
}

bool KingPiece::moveable(int r, int c, Board* board, char promotion) {
    if (isCastle(r, c, board)) return true;
    if (makeMove(r, c, board)) return true;
    return false;
}

bool KingPiece::isCastle(int r, int c, Board* board) {
    // If King Side Castle
    if (r < 0 || r > 7 || c < 0 || c > 6) return false;
    int rightbishopC = getCol() + 1;
    int rightknightC = getCol() + 2;
    if (((getColor() == Color::Black && getRow() == 0 && r == 0) || 
         (getColor() == Color::White && getRow() == 7 && r == 7)) && 
          getCol() == 4 && c == 6) {
        if (getCount() == 1 && board->getPiece(r, c + 1) && 
            board->getPiece(r, c + 1)->getColor() == getColor() &&
            board->getPiece(r, c + 1)->getType() == PieceType::Rook &&
            board->getPiece(r, c + 1)->getCount() == 1) {

            // If no piece in between King and Rook
            if (board->getPiece(r, rightbishopC) == nullptr && 
                board->getPiece(r, rightknightC) == nullptr) {
                return true;
            }
        }
    }
    // Queen Side Castle Move
    int queen = getCol() - 1;
    int leftbishopC = getCol() - 2;
    int leftknightC = getCol() - 3;
    if (((getColor() == Color::Black && getRow() == 0 && r == 0) ||
         (getColor() == Color::White && getRow() == 7 && r == 7)) &&
          getCol() == 4 && c == 2) {
            
        if (getCount() == 1 && board->getPiece(r, c - 2) &&
            board->getPiece(r, c - 2)->getColor() == getColor() &&
            board->getPiece(r, c - 2)->getType() == PieceType::Rook &&
            board->getPiece(r, c - 2)->getCount() == 1) {

            // If no piece between King and Rook
            if (!board->getPiece(r, queen) && !board->getPiece(r, leftbishopC) && 
                !board->getPiece(r, leftknightC)) {
                    return true;
            }
        }
    }
    return false;
}

bool KingPiece::moveProduceCheck(int r, int c, Board* board, char promotion) { 
    if (isCastle(r, c, board)) {
        // Tempboard must update differently if the move
        // was a Castling move

        Board testBoard = *board;
        Piece* start = testBoard.getPiece(getRow(), getCol());
        int pColor = (start->getColor() == Color::White ? 1 : 2);

        if (start->getCol() + 2 == c) {
            
            Piece* destinationPiece = testBoard.getPiece(r, c + 1);
            testBoard.update(r, c + 1, r, c + 1, nullptr);
            testBoard.update(r, c - 1, r, c - 1, destinationPiece);
            testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
            testBoard.update(getRow(), getCol(), r, c, start);
            
            start->setCoords(r, c);
            destinationPiece->setCoords(r, c - 1);
            start->setCount(start->getCount() + 1);
            destinationPiece->setCount(destinationPiece->getCount() + 1);
            if (testBoard.isCheck(pColor)) return true;
        }

        if (start->getCol() - 2 == c) {
            
            Piece* destinationPiece = testBoard.getPiece(r, c - 2);
            testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
            testBoard.update(getRow(), getCol(), r, c, start);
            testBoard.update(r, c - 2, r, c - 2, nullptr);
            testBoard.update(r, c + 1, r, c + 1, destinationPiece);
            
            start->setCoords(r, c);
            start->setCount(start->getCount() + 1);
            destinationPiece->setCoords(r, c + 1);
            destinationPiece->setCount(destinationPiece->getCount() + 1);
            if (testBoard.isCheck(pColor)) return true;
        }
    }

    // If it is a normal king move, tempboard only udpates two cells
    if (makeMove(r, c, board)) {
        
        Board testBoard = *board;
        Piece *start = testBoard.getPiece(getRow(), getCol());
        int pColor = (start->getColor() == Color::White ? 1 : 2);
        
        testBoard.removePiece(r, c);
        start->setCoords(r, c);
        start->setCount(start->getCount() + 1);
        testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
        testBoard.update(getRow(), getCol(), r, c, start);
        if (testBoard.isCheck(pColor)) return true;
    }
    return false;
}

