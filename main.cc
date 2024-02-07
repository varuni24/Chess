#include <iostream>
#include <sstream>
#include <string>
#include "chesscontroller.h"
using namespace std;


bool valid(string s) {
    if (s == "HUMAN"){
        return true;
    }
    else if (s.substr(0, 8) == "COMPUTER"){
        if (s.length() != 9){
            cout << "You need to specify the computer level in the format computer[1-4]" << endl;
            return false;
        } else if (s[8] - '0' < 1 || s[8] - '0' > 4){
            cout << "Level can only be 1, 2, 3 or 4!" << endl;
            return false;
        } else return true;
    } 
    else {
        cout << "You need to specify player of type human or computer[1-4]" << endl;
        return false;
    }
}



int main() {
    ChessController cc;

    string command;

    // cout << "Enter commands or 'exit' to quit:\n";

    while (getline(cin, command)) {

        istringstream iss{command};
        string cmd;
        iss >> cmd;

        if (cmd == "game") {
            if (cc.gameStarted() == false){
                string whitePlayer, blackPlayer;
                iss >> whitePlayer >> blackPlayer;
                for (auto & c: whitePlayer) c = (char)toupper(c);
                for (auto & c: blackPlayer) c = (char)toupper(c);   
                // cc.attemptCopy();
                // computer is 8 char
                // computer1 is 9 char
                if (valid(whitePlayer) && valid(blackPlayer)){
                    cc.start(whitePlayer, blackPlayer);
                    cout << "Game started, White Player Turn" << endl;
                } 
            } else{
                // cc.reset();
                cout << "Game has already begun LOL, resign or play the game please :)" << endl;
            }
        } else if (cmd == "setup") {
            if (cc.gameStarted() == false){
                cc.setup();
                cin.ignore();
            } else {
                cout << "You may not enter setup mode once game has started." << endl;
            }
        } else if (cmd == "move"){

            if (cc.gameStarted() == true){
                string start, end;
                char promotion;
                cout << cc.getPlayerTurnType(1) << " " << cc.getPlayerTurnType(2) << endl;

                if ((cc.getPlayerTurn() == 1 && cc.getPlayerTurnType(1) == "human") ||
                    (cc.getPlayerTurn() == 2 && cc.getPlayerTurnType(2) == "human")) {
                    if (iss >> start >> end) {
                        if (cc.turnPiece(start)) {
                            if (iss >> promotion) {
                                cc.makeHumanMove(start,end,promotion);
                            } else {
                                cc.makeHumanMove(start,end);
                            } 
                        } else {
                            cout << "You cannot move your opponents pieces!" << endl;
                        }
                    } else {
                        cout << "please give start, end position" << endl;
                    }
                } else {
                    cc.makeComputerMove();
                }

                cout << (cc.getPlayerTurn() == 1? "White " : "Black ") << "player turn!" << endl;

                // check if game is over after most recent move is made;
                // do it based on last player so invert the checks on the opponent
                // like in check would prob be for the opponent once i make a move 
                if (cc.gameOver()){
                    // can modify to take the opponents integer flag
                    cout << "Score So Far:" << endl;
                    cc.getFinalScore();
                    cc.reset(); // instead of break, do a reset
                } // FIXXXXXX
                // POSSIBLY ADD PROMOTION CHECK IN MOVE
            } else {
                cout << "You may not move if game has not started" << endl;
            }
            
        } else if (cmd == "resign") {
            cc.resign();
            cout << "Score So Far:" << endl;
            cc.getScore();
            cc.reset();
        } else {
            // cout << "i shouldn't be here" << endl;
            cout << "Invalid command" << endl;
        }
    }
    cc.getFinalScore();
}

