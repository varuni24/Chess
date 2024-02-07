#include "cell.h"


Cell::Cell(): occupant{nullptr} {}


Piece* Cell::getOccupant() const {
    return occupant ? occupant : nullptr;
}

void Cell::setPiece(Piece *p){
    occupant = p;
}

void Cell::removePiece() {
    occupant = nullptr;
}

bool Cell::empty(){
    return occupant;
}

void Cell::attach(Observer *o){
    observers.emplace_back(o);
}

void Cell::setCoords(int r, int c){
    this->r = r;
    this->c = c;
}

int Cell::getRow() const{
    return r;
}

int Cell::getCol() const {
    return c;
}


