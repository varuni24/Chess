#ifndef _QUEENPIECE_H_
#define _QUEENPIECE_H_

#include "piece.h"

class QueenPiece : public Piece {
    public:
    QueenPiece(PieceType pt, Color c);

    // Normal move validity of a queen piece
    bool makeMove(int r, int c, Board* board) override;

    // Automatically returns false since not Pawn
    bool isEnpassed(int r, int c, Board* board) override;

    // Automatically returns false since not Pawn
    bool isPromotion(int r, int c, Board *board, char promotion) override;

    // Automatically returns false since not King
    bool isCastle(int r, int c, Board* board) override;

    // Return makeMove() since Queen doesn't have a special move like pawn and king
    bool moveable(int r, int c, Board* board, char promotion) override;

    // Make sure moving doesn't put us in check
    bool moveProduceCheck(int r, int c, Board* board, char promotion) override;

};

#endif

