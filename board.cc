#include <iostream>
#include "board.h"
#include "piece.h"
#include "pawnpiece.h"
#include "rookpiece.h"
#include "bishoppiece.h"
#include "knightpiece.h"
#include "kingpiece.h"
#include "queenpiece.h"

using namespace std;

Board::Board(): td{nullptr} , gd{nullptr} {}

Board::~Board() {
    // Delete the dynamically allocated TextDisplay object
    delete td;
    delete gd;

    // Delete all the pieces on the board
    for (auto& row : theGrid) {
        for (auto& cell : row) {
            if (cell.getOccupant() != nullptr) {
                delete cell.getOccupant();
                cell.removePiece();  // so no dangling pointer
            }
        }
    }
    // Clear the vectors of pieces
    whitePieces.clear();
    blackPieces.clear();
}

Board::Board(const Board &other){
    theGrid = vector<vector<Cell>>(8, vector<Cell>(8));
    // Cell default ctor is called.

    td = nullptr;
    gd = nullptr;

    // ---------------------------- SET UP CELLS ------------------------------
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j){
            theGrid[i][j].setCoords(i,j);
            theGrid[i][j].setPiece(nullptr);

            Piece *otherPiece = other.theGrid[i][j].getOccupant();
            
            Piece *newPiece = nullptr;

            if (otherPiece){
                if (otherPiece->getType() == PieceType::Pawn) {
                    newPiece = new PawnPiece{*dynamic_cast<PawnPiece *>(otherPiece)};
                } else if (otherPiece->getType() == PieceType::Rook) {
                    newPiece = new RookPiece{*dynamic_cast<RookPiece *>(otherPiece)};
                } else if (otherPiece->getType() == PieceType::Bishop) {
                    newPiece = new BishopPiece{*dynamic_cast<BishopPiece *>(otherPiece)};
                } else if (otherPiece->getType() == PieceType::Knight) {
                    newPiece = new KnightPiece{*dynamic_cast<KnightPiece *>(otherPiece)};
                } else if (otherPiece->getType() == PieceType::Queen) {
                    newPiece = new QueenPiece{*dynamic_cast<QueenPiece *>(otherPiece)};
                } else if (otherPiece->getType() == PieceType::King) {
                    newPiece = new KingPiece{*dynamic_cast<KingPiece *>(otherPiece)};
                }
            }

            theGrid[i][j].setPiece(newPiece);

            if (newPiece) {
                if (newPiece->getColor() == Color::White) {
                    whitePieces.emplace_back(newPiece);
                } else {
                    blackPieces.emplace_back(newPiece);
                }
            }
        }
    }
}

void Board::init(){
    // Initialize the chessboard with pieces in their starting positions

    // Clear the grid 
    theGrid.clear();
    whitePieces.clear();
    blackPieces.clear();

    theGrid = vector<vector<Cell>>(8, vector<Cell>(8));

    td = new TextDisplay();
    gd = new GraphicsDisplay(*(new Xwindow()));

    // SET UP CELLS ------------------------------
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j){
            theGrid[i][j].setCoords(i,j);
            theGrid[i][j].attach(td);
            theGrid[i][j].attach(gd);
            theGrid[i][j].setPiece(nullptr);
        }
    }

    // ------------------------- ADD PIECES ------------------------

    // --------------------------- PAWNS ---------------------------
    for (int i = 0; i < 8; ++i) {
        theGrid[1][i].setPiece(new PawnPiece{PieceType::Pawn, Color::Black});
        blackPieces.emplace_back(theGrid[1][i].getOccupant());
        theGrid[1][i].getOccupant()->setCoords(1,i);
    }

    for (int i = 0; i < 8; ++i) {
        theGrid[6][i].setPiece(new PawnPiece({PieceType::Pawn, Color::White}));
        whitePieces.emplace_back(theGrid[6][i].getOccupant());
        theGrid[6][i].getOccupant()->setCoords(6,i);
    }
    // ------------------------- END OF PAWNS ------------------------------


    // -------------------------- OTHER BLACK -------------------------------
    theGrid[0][0].setPiece(new RookPiece{PieceType::Rook, Color::Black});
    theGrid[0][1].setPiece(new KnightPiece{PieceType::Knight, Color::Black});
    theGrid[0][2].setPiece(new BishopPiece{PieceType::Bishop, Color::Black});
    theGrid[0][3].setPiece(new QueenPiece{PieceType::Queen, Color::Black});
    theGrid[0][4].setPiece(new KingPiece{PieceType::King, Color::Black});
    theGrid[0][5].setPiece(new BishopPiece{PieceType::Bishop, Color::Black});
    theGrid[0][6].setPiece(new KnightPiece{PieceType::Knight, Color::Black});
    theGrid[0][7].setPiece(new RookPiece{PieceType::Rook, Color::Black});

    for (int i = 0; i < 8; ++i) {
        blackPieces.emplace_back(theGrid[0][i].getOccupant());
        theGrid[0][i].getOccupant()->setCoords(0,i);
    }

    // --------------------- OTHER WHITE -----------------------
    theGrid[7][0].setPiece(new RookPiece{PieceType::Rook, Color::White});
    theGrid[7][1].setPiece(new KnightPiece{PieceType::Knight, Color::White});
    theGrid[7][2].setPiece(new BishopPiece{PieceType::Bishop, Color::White});
    theGrid[7][3].setPiece(new QueenPiece{PieceType::Queen, Color::White});
    theGrid[7][4].setPiece(new KingPiece{PieceType::King, Color::White});
    theGrid[7][5].setPiece(new BishopPiece{PieceType::Bishop, Color::White});
    theGrid[7][6].setPiece(new KnightPiece{PieceType::Knight, Color::White});
    theGrid[7][7].setPiece(new RookPiece{PieceType::Rook, Color::White});

    for (int i = 0; i < 8; ++i) {
        whitePieces.emplace_back(theGrid[7][i].getOccupant());
        theGrid[7][i].getOccupant()->setCoords(7,i);
    }

    // --------------------- ONCE PIECES ARE ADDED -----------------------
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j){
            td->notify(theGrid[i][j]);
            gd->notify(theGrid[i][j]);
        }
    }
}

Piece* Board::getLastMove() { return lastMove; }

void Board::setLastMove(Piece* last) { lastMove = last; }


void Board::setupInit() {
    theGrid.clear();
    whitePieces.clear();
    blackPieces.clear();

    theGrid = vector<vector<Cell>>(8, vector<Cell>(8));

    if (td) {delete td;}
    else {td = new TextDisplay();}

    if (gd) {delete gd;}
    else {gd = new GraphicsDisplay(*(new Xwindow()));}

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j){
            theGrid[i][j].setCoords(i,j);
            theGrid[i][j].attach(td);
            theGrid[i][j].attach(gd);
            theGrid[i][1].setPiece(nullptr);
        }
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j){
            td->notify(theGrid[i][j]);
            gd->notify(theGrid[i][j]);
        }
    }
}

// Function returns Piece of a certain Cell
Piece* Board::getPiece(int r, int c) {
    return theGrid[r][c].getOccupant();
}

// Returns a Cell of the Board
Cell& Board::getCell(int r, int c) {
    return theGrid[r][c];
}


void Board::update(int startR, int startC, int r, int c, Piece* p) {
    // If we are updating the initial position cell, we don't want
    // to remove the piece there, we just want to move it to 
    // a different cell.
    if (theGrid[r][c].getOccupant() &&
        theGrid[r][c].getOccupant() != theGrid[startR][startC].getOccupant()) {
        removePiece(r, c);
        theGrid[r][c].setPiece(p);
        if (td){
            td->notify(theGrid[r][c]);
        }
        if (gd){
            gd->notify(theGrid[r][c]);
        }
    } else {
        theGrid[r][c].setPiece(p);
        if (td){
            td->notify(theGrid[r][c]);
        }
        if (gd){
            gd->notify(theGrid[r][c]);
        }
    }
}

bool Board::removePiece(int r, int c) {
    if (theGrid[r][c].getOccupant()) {
        // Cell is not empty
        Piece *removedPiece = theGrid[r][c].getOccupant();
        
        auto &piecesVector = removedPiece->getColor() == Color::White ? whitePieces : blackPieces;
            
        for (size_t i = 0; i < piecesVector.size(); ++i) {
            if (piecesVector[i] == removedPiece) {
                piecesVector.erase(piecesVector.begin() + i);
                break; 
            }
        }
        // Delete the removed piece
        delete removedPiece;
        // Set the cell to nullptr
        theGrid[r][c].removePiece();
        // samething as setPiece(nullptr)
        if (td){
            td->notify(theGrid[r][c]);
        }
        if (gd){
            gd->notify(theGrid[r][c]);
        }
        return true;
    } 
    return false;
}

// For Setup Mode
void Board::setupPiece(int r, int c, char p){
    removePiece(r, c);

    Piece *newPiece;

    if (p == 'p') {
        newPiece = new PawnPiece{Pawn, Black};
    } else if (p == 'r') {
        newPiece = new RookPiece{Rook, Black};
    } else if (p == 'b') {
        newPiece = new BishopPiece{Bishop, Black};
    } else if (p == 'n') {
        newPiece = new KnightPiece{Knight, Black};
    } else if (p == 'q') {
        newPiece = new QueenPiece{Queen, Black};
    } else if (p == 'k') {
        newPiece = new KingPiece{King, Black};
    } else if (p == 'P') {
        newPiece = new PawnPiece{Pawn, White};
    } else if (p == 'R') {
        newPiece = new RookPiece{Rook, White};
    } else if (p == 'B') {
        newPiece = new BishopPiece{Bishop, White};
    } else if (p == 'N') {
        newPiece = new KnightPiece{Knight, White};
    } else if (p == 'Q') {
        newPiece = new QueenPiece{Queen, White};
    } else if (p == 'K') {
        newPiece = new KingPiece{King, White};
    }  else {
        newPiece = nullptr;
    } 
    
    if (newPiece) {
        newPiece->setCoords(r,c);
        theGrid[r][c].setPiece(newPiece);
    }

    if (theGrid[r][c].getOccupant()->getColor() == White){
        whitePieces.emplace_back(newPiece);
    } else {
        blackPieces.emplace_back(newPiece);
    }
    
    // notify my cell af 
    if (td) td->notify(theGrid[r][c]);
    if (gd) gd->notify(theGrid[r][c]);
}


bool Board::atleastOneKing(int color){

    auto &pieces = (color == 1) ? whitePieces : blackPieces;
    for (const auto &piece : pieces) {
        if (piece->getType() == PieceType::King) {
            return true;
        }
    } 
    return false;
}


bool Board::isCheckMate(int color) {
    // If not in check can't be checkmate anyways
    if (!isCheck(color)) {
        return false;
    }
    // If piece can move to cover King and get out of Check
    const auto& pieces = (color == 1) ? whitePieces : blackPieces;

    for (const auto& piece : pieces) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                
                // If the piece exist and is legal to move to the end coordinate
                // and the move doesn't produce check on its own color
                if (piece && piece->moveable(row, col, this, ' ') && 
                    !piece->moveProduceCheck(row,col,this,' ')) return false;
            }
        }
    }

    // If king can get itself out of check
    Piece* king;
    for (const auto& piece : pieces) {
        if (piece && piece->getType() == PieceType::King) {
            king = piece;
            int kingR = king->getRow();
            int kingC = king->getCol();
        // Check for possible escape moves
            for (int row = kingR - 1; row <= kingR + 1; row++) {
                for (int col = kingC - 1; col <= kingC + 1; col++) {
                    if (row >= 0 && row < 8 && col >= 0 && col < 8 && (row != kingR || col != kingC)) {  // bound checking
                        if (king->moveable(row, col, this, ' ') && !king->moveProduceCheck(row, col, this, ' ')) return false;  // legal move -> not checkmate
                    }
                }
            }
        }
    }
    return true;
}


bool Board::isCheck(int color) {
    int kingR = -1;
    int kingC = -1;

    // Find the king's position
    for (const auto &piece : (color == 1) ? whitePieces : blackPieces) {
        if (piece && piece->getType() == PieceType::King) {
            kingR = piece->getRow();
            kingC = piece->getCol();
            break;
        }
    }
    // If King is not on the board which shouldn't happen but for safety
    if (kingR == -1 && kingC == -1) return false;
    if (color == 1) { // WHITE IS ONE
        for (const auto &piece : blackPieces) {
            if (piece && piece->moveable(kingR, kingC, this, ' ')) {
                return true;
            }
        }
    } else { // BLACK IS TWO 
        for (const auto &piece : whitePieces) {  
            if (piece && piece->moveable(kingR, kingC, this, ' ')) {
                return true;
            }
        }
    }
    return false;
}



bool Board::isStalemate(int color) {
    // Check if the current player is in check
    // If the player is in check, not in stalemate
    if (isCheck(color)) {
        return false;
    }

    // If there are only one King left on each side
    if ((whitePieces.size() == 1 && whitePieces[0]->getType() == PieceType::King) &&
        (blackPieces.size() == 1 && blackPieces[0]->getType() == PieceType::King)) {
        return true;
    }

    // Check if the current player has any legal moves
    for (const auto &piece : (color == 1) ? whitePieces : blackPieces) {

        // Check if there is any legal move for the piece
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (piece && piece->moveable(row, col, this, ' ') &&
                    !piece->moveProduceCheck(row, col, this, ' ')) {
                    return false;
                }
            }
        }
    }

    // If no legal moves are found, it's stalemate
    cout << "Stalemate!" << endl;
    return true;
}


bool Board::isCapturingMove(int startR, int startC, int endR, int endC) {
    // If end coordinate has a piece and it is valid for your piece to move there
    if (theGrid[startR][startC].getOccupant() && theGrid[endR][endC].getOccupant()) {
        if (theGrid[startR][startC].getOccupant()->moveable(endR, endC, this, ' ')) return true;
    }
    return false;
}

bool Board::isUnderThreat(int row, int col, Color threatColor) {
    auto& opponentPieces = (threatColor == Color::White) ? blackPieces : whitePieces;

    // Iterate through the opponent's pieces
    for (const auto& opponentPiece : opponentPieces) {
        // if (opponentPiece->getType() == PieceType::Rook) cout << "ROOK" << endl;
        if (opponentPiece->moveable(row, col, this, ' ')) {
            return true; // The square is under threat
        }
    }
    
    return false; // The square is not under threat
}



ostream &operator<<(ostream &out, const Board &g) {
  out << *(g.td); 
  return out;
}



