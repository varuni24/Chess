#ifndef _CELL_H_
#define _CELL_H_

#include <iostream>
#include <vector>

#include "piece.h"
#include "observer.h"

class Cell{
    Piece * occupant;

    int r, c;

    std::vector <Observer *> observers;

    public:
        Cell();

        Piece* getOccupant() const;

        // Mutator to update the corresponding
        // piece that is on that cell
        void setPiece(Piece *p);

        void removePiece(); // NEWWWW

        bool empty();

        // Attach to cell observer
        void attach(Observer *o);

        // Set cell coordinates
        void setCoords(int c, int r);

        // r field accessor
        int getRow() const ;

        // c field accessor
        int getCol() const;
};

#endif

