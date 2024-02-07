#ifndef _ROOKPIECE_H_
#define _ROOKPIECE_H_

#include "piece.h"

class RookPiece : public Piece {
    public:
    RookPiece(PieceType pt, Color c);

    // Check validity of a normal move for Rook
    bool makeMove(int r, int c, Board* board) override;

    // Return false automatically since not Pawn
    bool isEnpassed(int r, int c, Board* board) override;

    // Return false automatically since not Pawn
    bool isPromotion(int r, int c, Board *board, char promotion) override;

    // Return false automatically since not King
    bool isCastle(int r, int c, Board* board) override;

    // Returns makeMove() since Rook doesn't have a special move
    bool moveable(int r, int c, Board* board, char promotion) override;

    // Ensure moving doesn't put player in check by opponent
    bool moveProduceCheck(int r, int c, Board* board, char promotion) override;
};

#endif

