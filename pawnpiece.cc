#include "pawnpiece.h"
#include "piece.h"
#include "board.h"

using namespace std;

PawnPiece::PawnPiece(PieceType pt, Color c) : Piece{pt, c} {}

bool PawnPiece::makeMove(int r, int c, Board* board) {

    if (!board->getPiece(getRow(), getCol())) return false;
    // Check if the end coordinates and moving is valid:
    // 1. If end coordinates are out of bounds of the board
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    // 2. If end coordinate is my current coordinate
    if (r == getRow() && c == getCol()) return false;
    // 3. If move to a Cell that has piece of my own colour
    if (board->getPiece(r,c) && board->getPiece(r, c)->getColor() == this->getColor()) return false;


    // Figure out how to move, depending on the Piece color since their
    // rows starting positions are flipped.
    int dirFactor = (getColor() == Color::White) ? -1 : 1;
    // Double first move. Also make sure the Cell we move to is Empty
    // Also ensure we aren't jumping over anyone when moving double
    // cout<< getRow() + dirFactor + dirFactor << " " << "Double Move" << endl;
    if (getCount() == 1 && getColor() == Color::White && (getRow() - 1 - 1 == r) && (getCol() == c) && !board->getPiece(r,c)
        && !board->getPiece(getRow() - 1, c) && !board->getPiece(getRow() - 1 - 1, c)) {
        return true;
    }

    if (getCount() == 1 && getColor() == Color::Black && (getRow() + 1 + 1 == r) && (getCol() == c) && !board->getPiece(r,c)
        && !board->getPiece(getRow() + 1, c) && !board->getPiece(getRow() + 1 + 1, c)) {
        return true;
    }


    // Case when pawn moves one forward
    if ((getRow() + dirFactor == r) && (getCol() == c) && !board->getPiece(r,c)) {
        return true;
    }

    // Case when white pawn moves diagonally to capture
    if ((getRow() + dirFactor == r) && (getColor() == Color::White) && board->getPiece(r,c) &&
        (getCol() + 1 == c || getCol() - 1 == c)) {
            return true;
    }
    if ((getRow() + dirFactor == r) && (getColor() == Color::Black) && board->getPiece(r,c) &&
        (getCol() - 1 == c || getCol() + 1 == c)) {
            return true;
    }
    // Maybe add case to check if moving put you in check.
    return false;
}


bool PawnPiece::isEnpassed(int r, int c, Board* board) {

    // ADD A ISENPASSED VARIABLE TO THE SUPERCLASS
    if (r < 2) return false;

    // Check if the end coordinates and moving is valid:
    // 1. If end coordinates are out of bounds of the board
    if (r < 0 || r >= 7 || c < 0 || c >= 7) return false;
    // 2. If end coordinate is my current coordinate
    if (r == getRow() && c == getCol()) return false;
    // 3. If move to a Cell that has piece
    if (board->getPiece(r,c)) return false;
    
    // EnPasse White
    if (this->getColor() == Color::White && this->getRow() == 3 && board->getPiece(r, c) == nullptr) {
        if (board->getPiece(r + 1, c) && (board->getLastMove() == board->getPiece(r + 1, c)) &&
            board->getPiece(r + 1, c)->getType() == PieceType::Pawn && 
            board->getPiece(r + 1, c)->getColor() == Color::Black &&
            board->getPiece(r + 1, c)->getCount() == 2) {
                // Figure out how to Notify the Cell that the piece just got captured in
                return true;
        }
    }
    // EnPasse Black
    if (getColor() == Color::Black && getRow() == 4 && board->getPiece(r, c) == nullptr) {
        if (board->getPiece(r - 1, c) && ((board->getLastMove() == board->getPiece(r - 1, c))) &&
            board->getPiece(r - 1, c)->getType() == PieceType::Pawn &&
            board->getPiece(r - 1, c)->getColor() == Color::White &&
            board->getPiece(r - 1, c)->getCount() == 2) {
                return true;
        }
    }  
    return false;
}


bool PawnPiece::moveable(int r, int c, Board* board, char promotion) {
    if (isEnpassed(r, c, board)) return true;
    if (isPromotion(r, c, board, promotion)) return true;
    if (makeMove(r, c, board)) return true;

    return false;
}

bool PawnPiece::isPromotion(int r, int c, Board* board, char promotion) {
    // If move straight to the last cell of opposite side and that destination cell is empty
    if (promotion == ' ') { return false; }
    if (getColor() == Color::White && getRow() == 1 && getCol() >= 0 && getCol() < 8 && r == 0 && 
        ((getCol() == c && !board->getPiece(r, c)) || (abs(c - getCol()) == 1 && board->getPiece(r, c)))) {
        return true;
    } 
    if (getColor() == Color::Black && getRow() == 6 && getCol() >= 0 && getCol() < 8 && r == 7 && 
        ((getCol() == c && !board->getPiece(r, c)) || (abs(c - getCol()) == 1 && board->getPiece(r, c)))) {
        return true;
    }
    return false; 
}


bool PawnPiece::isCastle(int r, int c, Board* board) { return false; }


bool PawnPiece::moveProduceCheck(int r, int c, Board* board, char promotion) {

    if (isEnpassed(r, c, board)) {
        Board testBoard = *board;
        Piece *start = testBoard.getPiece(getRow(), getCol());
        int pColor = (start->getColor() == Color::White ? 1 : 2);
        
        // Notify and update Cells
        testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
        testBoard.update(getRow(), getCol(), r, c, start);
        start->setCount(start->getCount() + 1);
        // If black piece doing enpasse, remove the captured piece
        if (start->getColor() == Color::Black) {
            testBoard.removePiece(r - 1, c);
            testBoard.update(getRow(), getCol(), r - 1, c, nullptr);
        } else {
            testBoard.removePiece(r + 1, c);
            testBoard.update(getRow(), getCol(), r + 1, c, nullptr);
        }
        start->setCoords(r, c);
        start->setCount(start->getCount() + 1);
        
        if (testBoard.isCheck(pColor)) return true;
    }

    if (isPromotion(r, c, board, promotion)) {
        Board testBoard = *board;
        Piece *start = testBoard.getPiece(getRow(), getCol());
        int pColor = (start->getColor() == Color::White ? 1 : 2);
        testBoard.setupPiece(getRow(), getCol(), promotion);
        Piece* promote = testBoard.getPiece(getRow(), getCol());
        testBoard.update(getRow(), getCol(), getRow(), getCol(), nullptr);
        testBoard.update(getRow(), getCol(), r, c, promote);
        promote->setCoords(r, c);
        promote->setCount(promote->getCount() + 1);
        if (testBoard.isCheck(pColor)) return true;
    }

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


