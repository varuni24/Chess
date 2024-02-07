#ifndef _BISHOPPIECE_H_
#define _BISHOPPIECE_H_

#include "piece.h"

class BishopPiece : public Piece {
    public:
        // Ctor
        BishopPiece(PieceType pt, Color c);

        // Check validity of a "typical" bishop move
        bool makeMove(int r, int c, Board* board) override;

        // Automatically returns false since this special
        // move is only for Pawns
        bool isEnpassed(int r, int c, Board* board) override;

        // Automatically returns false since this special
        // move is only for Pawns
        bool isPromotion(int r, int c, Board *board, char promotion) override;

        // Automatically returns false since this special
        // move is only for Kings
        bool isCastle(int r, int c, Board* board) override;

        // Used to check validity of move (which covers all the special cases
        // that a piece could have). Since bishop don't have any special moves,
        // it just return the value of the makeMove() method.
        bool moveable(int r, int c, Board* board, char promotion) override;

        // Utilizes a temporary board to check if moving to the end coordinate
        // (r, c) would put our player in check or not.
        bool moveProduceCheck(int r, int c, Board* board, char promotion) override;
};

#endif




