#include "graphicsdisplay.h"
#include "cell.h"
#include <string>

using namespace std;

GraphicsDisplay::GraphicsDisplay(Xwindow &window): window{window} {
    int factor = 90;

    for (int row = 0; row != 8; ++row){
        for (int col = 0; col!= 8; ++col){
            int x = row * factor;
            int y = col * factor;
            window.fillRectangle(x + 80, y, factor, factor,   ((row + col) % 2 == 0) ? Xwindow::White : Xwindow::Gray);
        }
    }
    // set row 
    window.fillRectangle(80, 0, 5, 800,   Xwindow::Black);
    window.fillRectangle(0, 720, 800, 5,   Xwindow::Black);

    for (int i = 0; i != 8; ++i){
        int y = (i * factor) + 45;
        int x = 45 ;
        window.drawString( x  , y, to_string(8 - i));
    }

    string positions[8] = {"a", "b", "c", "d", "e", "f","g", "h"};
    
    for (int i = 0; i != 8; ++i){
        int x = (i * factor) + 125;
        int y = 750 ;
        window.drawString( x  , y, positions[i]);
    }
}

void GraphicsDisplay::notify(Cell &c) {
  // Get the row and column of the cell
  int row = c.getRow();
  int col = c.getCol();

  // Calculate the factor to determine the size of each cell on the display
  int factor = 90;
  
  // Calculate the coordinates of the top-left corner of the cell on the display
  int y = row * factor + 45;
  int x = col * factor + 45;

  string pieceSymbol = "";

  if (c.getOccupant()) {  // Default for unknown pieces
    switch (c.getOccupant()->getType()) {
        case PieceType::Pawn:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "P" : "p";
            break;
        case PieceType::Rook:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "R" : "r";
            break;
        case PieceType::Bishop:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "B" : "b";
            break;
        case PieceType::Knight:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "N" : "n";
            break;
        case PieceType::Queen:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "Q" : "q";
            break;
        case PieceType::King:
            pieceSymbol = (c.getOccupant()->getColor() == Color::White) ? "K" : "k";
            break;
        default:
            pieceSymbol = " ";
            break;  // Handle other piece types if needed
    }
    window.drawString(x + 80 , y, pieceSymbol); 
  } else {
        window.fillRectangle(x + 35, y - 45, factor, factor,   ((row + col) % 2 == 0) ? Xwindow::White : Xwindow::Gray);
    }
    window.fillRectangle(80, 0, 5, 800,   Xwindow::Black);
    window.fillRectangle(0, 720, 800, 5,   Xwindow::Black);
}

GraphicsDisplay::~GraphicsDisplay() {}


