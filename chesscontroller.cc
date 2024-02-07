#include <iostream>
#include <string>
#include <sstream>

#include "chesscontroller.h"

using namespace std;


ChessController::ChessController(): playerTurn{1} , WPscore{0}, BPscore{0}, WPlayer{nullptr}, BPlayer{nullptr} , setupMode{false} , gameStart{false} {}


void ChessController::start(std::string whitePlayer, std::string blackPlayer){
    gameStart = true;

    if (!setupMode){
        theBoard = Board{};
        theBoard.init();
        playerTurn = 1; 
    }

    if (whitePlayer == "HUMAN" && blackPlayer == "HUMAN"){
        WPlayer = new Human{&theBoard, Color::White};
        BPlayer = new Human{&theBoard, Color::Black}; 
    } else if (whitePlayer == "HUMAN"){
        WPlayer = new Human{&theBoard, Color::White};
        BPlayer = new Computer{&theBoard, Color::Black, blackPlayer[8]};
        // cout << blackPlayer[8] << endl;
    } else if (blackPlayer == "HUMAN"){
        WPlayer =  new Computer{&theBoard, Color::White, whitePlayer[8]};
        BPlayer =  new Human{&theBoard, Color::Black};
    } else {
        WPlayer =  new Computer{&theBoard, Color::White, whitePlayer[8]};
        BPlayer =  new Computer{&theBoard, Color::Black, blackPlayer[8]};
    }

    cout << theBoard;
}

// void ChessController::attemptCopy(){
//     theBoard = Board{};
//     theBoard.init();
    
//     cout << "i tried" << endl;
//     Board tempBoard = Board{theBoard};
//     cout << "i made it here" << endl;
//     cout << theBoard << endl;
//     cout << tempBoard << endl;
// }


void ChessController::setup() {
    cout << "Entering setup mode. Type 'done' to exit setup mode." << endl;
    setupMode = true;

    string cmd;

    theBoard.setupInit();

    WPscore = 0;
    BPscore = 0;

    while (cin >> cmd) {
        if (cmd == "+") {
            // Place a piece on the board
            char piece;
            string position;

            cin >> piece >> position;
            if (position.length() < 2 || position.length() > 2){
                cout << "Invalid board position" << endl;
                continue;
            }
           
            int row = '8' - position[1] ;
            int col = position[0] - 'a';

            if (0 > row || row > 7 || 0 > col || col > 7 ){
                cout << "Position you are trying to add to is out of bounds" << endl;
            }

            else if (piece != 'p' && piece != 'P' && 
                    piece != 'r' && piece != 'R' &&
                    piece != 'n' && piece != 'N' &&
                    piece != 'b' && piece != 'B' &&
                    piece != 'q' && piece != 'Q' &&
                    piece != 'k' && piece != 'K') {
                cout << "Invalid piece" << endl;

            } else if (piece == 'k') {
                if (theBoard.atleastOneKing(2)) {
                    cout << "Only one King is allowed per color." << endl;
                
                } else {
                    theBoard.setupPiece(row,col, 'k');
                    cout << theBoard;
                }
            } else if (piece == 'K') {
                if (theBoard.atleastOneKing(1)) {
                    cout << "Only one King is allowed per color." << endl;
                } else {
                    theBoard.setupPiece(row,col, 'K');
                    cout << theBoard;
                }
            } else if (piece == 'p' || piece == 'P'){
                if (position[1] == '1' || position[1] == '8' ){
                    cout << "Pawn cannot be placed in first or last row." << endl;
                } else {
                    theBoard.setupPiece(row,col, piece);
                    cout << theBoard;
                }
            } else  {
                theBoard.setupPiece(row,col, piece);
                cout << theBoard;
            }
        } else if (cmd == "-"){
            string position;

            cin >> position;
           
            if (position.length() < 2 || position.length() > 2){
                cout << "Invalid board position" << endl;
                continue;
            }

            int row = '8' - position[1] ;
            int col = position[0] - 'a';

            if (row < 0 || row >= 8 || col < 0 || col >= 8) { 
                cout << "Position you are trying to remove from is out of bounds" << endl;
                continue;
            }

            theBoard.removePiece(row,col);
            cout << theBoard;

        } else if (cmd == "=") {
            string color;
            cin >> color;
            for (auto & c: color) c = (char)toupper(c);

            if (color == "WHITE") {
                playerTurn = 1;
            } else if (color == "BLACK"){
                playerTurn = 2;
            } else {
                cout << "Sorry no player of color " << color << " exists.";
            }
        } else if (cmd == "done") {
            if (theBoard.atleastOneKing(1) && theBoard.atleastOneKing(2) && (!theBoard.isCheck(1) && !theBoard.isCheck(2))){
                cout << "Leaving setup mode..." << endl;
                break;
            } else if (theBoard.isCheck(1) || theBoard.isCheck(2)) {
                cout << "Sorry you cannot leave setup mode: One or more of your king's is in check." << endl;
            } else {
                cout << "Sorry you cannot leave setup mode: you do not have enough kings" << endl;
            }
        } else {
            cout << "Invalid Command" << endl;
        }
    }
}


void ChessController::makeHumanMove(std::string start, std::string end, char promotion){
    if (start.length() > 2 || end.length() > 2 || start.length() < 2 || end.length() < 2){
        cout << "Invalid position bro, coordinates can't exceed the bounds of an 8x8 chess board"<< endl;
        return;
    }
    int startR = '8' - start[1] ;
    int startC = start[0] - 'a';

    int endR = '8' - end[1] ;
    int endC = end[0] - 'a';

    if (startR < 0 || startR >= 8 || startC < 0 || startC >= 8) { 
        cout << "Position you are trying to move from is out of bounds" << endl;
    } else if (endR < 0 || endR >= 8 || endC < 0 || endC >= 8){
        cout << "Position you are trying to move to is out of bounds" << endl;
    } else {
        if (playerTurn == 1){
            if (!WPlayer->makeMove(start,end,promotion)){
                cout << "invalid move!!!" << endl;
            } else {
                if (theBoard.isCheck(2)) cout << "BLACK IS CHECKED" << endl;
                cout << theBoard;
                playerTurn = 2; // added 
            }
        } else {
            if (!BPlayer->makeMove(start,end,promotion)){
                cout << "invalid move!!!" << endl;
            } else {
                if (theBoard.isCheck(1)) cout << "WHITE IS CHECKED" << endl;
                cout << theBoard;
                playerTurn = 1;
            }
        }
    }
}

void ChessController::makeComputerMove(){
    if (playerTurn == 1){
        if (!WPlayer->makeMove(WPlayer->getLevel())) {
            //if (theBoard.isCheckMate(1)) cout << "CHECKMATE WHITE" << endl;
        }
        if (theBoard.isCheck(2)) cout << "BLACK IS CHECKED" << endl;
        playerTurn = 2; // added 
    } else {
        if (!BPlayer->makeMove(BPlayer->getLevel())) {
            //if (theBoard.isCheckMate(2)) cout << "CHECKMATE WHITE" << endl; 
        }
        if (theBoard.isCheck(1)) cout << "WHITE IS CHECKED" << endl;
        playerTurn = 1;
    }
    cout << theBoard;
}

int ChessController::getPlayerTurn(){
    return playerTurn;
}

string ChessController::getPlayerTurnType(int pt){
    if (pt == 1){
        return WPlayer->getType();
    } else {
        return BPlayer->getType();
    }
}

void ChessController::resign() {
    if (playerTurn == 1) {
        ++BPscore;
    } else {
        ++WPscore;
    }
}

bool ChessController::gameOver() {
    if (theBoard.isStalemate(1) || theBoard.isStalemate(2)) {
        // the game is a draw 
        WPscore += 0.5;
        BPscore += 0.5;
        gameStart = false;
        return true;
    }
    if (theBoard.isCheckMate(1)) {
        cout << "Check Mate White!!!" << endl;
        BPscore += 1;
        gameStart = false;
        return true;
    }
    if (theBoard.isCheckMate(2)) {
        cout << "Check Mate Black!!!" << endl;
        WPscore += 1;
        gameStart = false;
        return true;
    }
    return false;
}

void ChessController::getScore(){
    // cout << "Final Score:" << endl;
    cout << "White: " << WPscore << endl;
    cout << "Black: " << BPscore << endl;

}

void ChessController::getFinalScore(){
    cout << "Final Score:" << endl;
    cout << "White: " << WPscore << endl;
    cout << "Black: " << BPscore << endl;

}


void ChessController::reset(){
    theBoard = Board(); // Reconstruct a new Board object

    playerTurn = 1;
    setupMode = false;
    gameStart = false;

    // Clean up and reset the players
    delete WPlayer;
    delete BPlayer;
    WPlayer = nullptr;
    BPlayer = nullptr;

    cout << "Board has been reset, game to begin or setup to configure." << endl;
}

bool ChessController::turnPiece(string pos){
    int r = '8' - pos[1] ;
    int c = pos[0] - 'a';

    if ((theBoard.getPiece(r, c) && theBoard.getPiece(r, c)->getColor() == White && playerTurn == 1) ||
        (theBoard.getPiece(r, c) && theBoard.getPiece(r, c)->getColor() == Black && playerTurn == 2)) {
        return true;
    }
    return false;
} 

bool ChessController::gameStarted(){
    return gameStart;
 }
ChessController::~ChessController() {
    // Delete the dynamically allocated Player objects
    delete WPlayer;
    delete BPlayer;
}


