#ifndef _KINGPIECE_H_
#define _KINGPIECE_H_

#include "piece.h"

class KingPiece : public Piece {
    public:
    KingPiece(PieceType pt, Color c);

    // Determine if King is making a valid normal move
    bool makeMove(int r, int c, Board* board) override;

    // Automatically false since not pawn
    bool isEnpassed(int r, int c, Board* board) override;

    // Automatically false since not pawn
    bool isPromotion(int r, int c, Board *board, char promotion) override;

    // Check if it is King's special move of castling
    bool isCastle(int r, int c, Board* board) override;

    // Check if it was a King's special move or a normal move
    // so it calls both makeMove() and isCastle()
    bool moveable(int r, int c, Board* board, char promotion) override;

    // Ensure move doesn't produce check on the player
    bool moveProduceCheck(int r, int c, Board* board, char promotion) override;
};

#endif

