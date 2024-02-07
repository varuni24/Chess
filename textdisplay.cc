#include <iostream>
#include "textdisplay.h"
#include "cell.h"
using namespace std;

TextDisplay::TextDisplay() {
  theDisplay = vector<vector<char>>(8, vector<char>(8, '_'));
}

void TextDisplay::notify(Cell &c) {
    int row = c.getRow();
    int col = c.getCol();
    // || c.getOccupant()->getType() == PieceType::None
    
    if (!c.getOccupant()) {
        theDisplay[row][col] = ((row + col) % 2 == 0) ? ' ' : '_';
    } else {
        char pieceSymbol = ' ';  // Default for unknown pieces
        switch (c.getOccupant()->getType()) {
            case PieceType::Pawn:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'P' : 'p';
                break;
            case PieceType::Rook:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'R' : 'r';
                break;
            case PieceType::Bishop:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'B' : 'b';
                break;
            case PieceType::Knight:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'N' : 'n';
                break;
            case PieceType::Queen:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'Q' : 'q';
                break;
            case PieceType::King:
                pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? 'K' : 'k';
                break;
            default:
                break;  // Handle other piece types if needed
        }

        theDisplay[row][col] = pieceSymbol;
    }
}

TextDisplay::~TextDisplay() {}

ostream &operator<<(ostream &out, const TextDisplay &td) {
  // Iterate through each row and column of theDisplay, printing the cell content
  int n = 8;
  for( int i = 0; i != 8; ++i){
    out << n << " " ;
    for( int j = 0; j != 8; ++j){
      out << td.theDisplay[i][j];
    }
    --n;
    out << '\n';
  }
  out << "  abcdefgh\n";
  return out;
}


