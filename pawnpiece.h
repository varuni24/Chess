#ifndef _PAWNPIECE_H_
#define _PAWNPIECE_H_

#include "piece.h"

class PawnPiece : public Piece {
    public:

    PawnPiece(PieceType pt, Color c);

    // Check for valid normal move of a pawn
    bool makeMove(int r, int c, Board* board) override;

    // Is Pawn doing EnPassant
    bool isEnpassed(int r, int c, Board* board) override;

    // Is Pawn promotion
    bool isPromotion(int r, int c, Board *board, char promotion) override;

    // returns false automatically since not King piece
    bool isCastle(int r, int c, Board* board) override;

    // Calls both Pawn's special moves (isEnpassed and isPromotion)
    // and normal move makeMove() to see if the move was valid
    bool moveable(int r, int c, Board* board, char promotion) override;

    // Ensure moving doesn't produce check on the player
    bool moveProduceCheck(int r, int c, Board* board, char promotion) override;
};

#endif



