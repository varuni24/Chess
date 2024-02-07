#ifndef _KNIGHTPIECE_H_
#define _KNIGHTPIECE_H_

#include "piece.h"

class KnightPiece : public Piece {
    public:
    KnightPiece(PieceType pt, Color c);

    // Check for knight normal move's validity
    bool makeMove(int r, int c, Board* board) override;

    // False automatically, not pawn
    bool isEnpassed(int r, int c, Board* board) override;

    // False automatically, not pawn
    bool isPromotion(int r, int c, Board *board, char promotion) override;

    // False automatically, not King
    bool isCastle(int r, int c, Board* board) override;

    // Since knight doesn't have a special move like pawn or king,
    // it just calls makeMove()
    bool moveable(int r, int c, Board* board, char promotion) override;

    // Ensure doesn't produce check after moving
    bool moveProduceCheck(int r, int c, Board* board, char promotion) override;
};

#endif

