#include "string"
#include "human.h"
using namespace std;


Human::Human(Board* b, Color c) : Player{b,c,"human"} {}

int Human::getLevel() { return 0; }

bool Human::makeMove(std::string start, std::string end, char promotion) {
    int startRow = '8' - start[1];
    int startCol = start[0] - 'a';
    int endRow = '8' - end[1];
    int endCol = end[0] - 'a';
    Piece* startingPiece = theBoard->getPiece(startRow, startCol);
    
    // If Piece is Pawn, there are 2 special moves that Cells might 
    // need to be notified/updated differently than a normal move.

    if (startingPiece && startingPiece->getType() == PieceType::Pawn) {

        // If Pawn move is Enpassant, 3 Cells are affected
        if (startingPiece->isEnpassed(endRow, endCol, theBoard) && 
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {

            // Notify and update Start and Ending Cell of our Pawn
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

            // Update the Cell that was captured, which is not the start or end Cell
            if (startingPiece->getColor() == Color::Black) {
                theBoard->removePiece(endRow - 1, endCol);
                theBoard->update(startRow, startCol, endRow - 1, endCol, nullptr);
            } else {
                theBoard->removePiece(endRow + 1, endCol);
                theBoard->update(startRow, startCol, endRow + 1, endCol, nullptr);
            }
            startingPiece->setCoords(endRow, endCol);
            startingPiece->setCount(startingPiece->getCount() + 1);
            
            // NEW
            theBoard->setLastMove(startingPiece);
            return true;
        }

        // If Pawn promotion, must make Pawn into new PieceType and update location
        if (startingPiece->isPromotion(endRow, endCol, theBoard, promotion) &&
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {
            
            theBoard->setupPiece(startRow, startCol, promotion);
            Piece* promote = theBoard->getPiece(startRow, startCol);
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, promote);
            promote->setCoords(endRow, endCol);
            promote->setCount(promote->getCount() + 1);
            
            // NEW
            theBoard->setLastMove(promote);
            return true;
        }
    } else if (startingPiece && startingPiece->getType() == PieceType::King) {
        if (startingPiece->isCastle(endRow, endCol, theBoard) && 
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {            
            
            // If King side Castling
            if (startingPiece->getCol() + 2 == endCol) {
                // Find Rook
                Piece* destinationPiece = theBoard->getPiece(endRow, endCol + 1);

                // Update Rook position on the Board
                theBoard->update(endRow, endCol + 1, endRow, endCol + 1, nullptr);
                theBoard->update(endRow, endCol - 1, endRow, endCol - 1, destinationPiece);

                // Update King's position on the Board
                theBoard->update(startRow, startCol, startRow, startCol, nullptr);
                theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

                // Set King's and Rook's coordinates to where it just moved
                startingPiece->setCoords(endRow, endCol);
                destinationPiece->setCoords(endRow, endCol - 1);
                startingPiece->setCount(startingPiece->getCount() + 1);
                destinationPiece->setCount(destinationPiece->getCount() + 1);

                theBoard->setLastMove(startingPiece);
                return true;
            }

            // If Queen side Castling
            if (startingPiece->getCol() - 2 == endCol) {

                // Find Rook
                Piece* destinationPiece = theBoard->getPiece(endRow, endCol - 2);

                // Update King
                theBoard->update(startRow, startCol, startRow, startCol, nullptr);
                theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

                // Update Rook
                theBoard->update(endRow, endCol - 2, endRow, endCol - 2, nullptr);
                theBoard->update(endRow, endCol + 1, endRow, endCol + 1, destinationPiece);

                // Set Rook and King to its new position
                startingPiece->setCoords(endRow, endCol);
                startingPiece->setCount(startingPiece->getCount() + 1);
                destinationPiece->setCoords(endRow, endCol + 1);
                destinationPiece->setCount(destinationPiece->getCount() + 1);
                return true;
                theBoard->setLastMove(startingPiece);
                return true;
            } 
        }
    }
    // Update Piece Coords and Notify Display for a normal move that affects only 2 Cells
    if (startingPiece && startingPiece->makeMove(endRow, endCol, theBoard) &&
        !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {
            
            theBoard->removePiece(endRow, endCol);
            startingPiece->setCoords(endRow, endCol);
            startingPiece->setCount(startingPiece->getCount() + 1);
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, startingPiece);  
            
            // NEW
            theBoard->setLastMove(startingPiece);
            return true;
    }
    return false;
}

bool Human::makeMove(int level){
    return false;
}


