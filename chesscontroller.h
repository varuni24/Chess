#ifndef _CHESSCONTROLLER_H_
#define _CHESSCONTROLLER_H_

#include <string>
#include "player.h"
#include "board.h"
#include "human.h"
#include "computer.h"

#include "desc.h"

class ChessController{
    int playerTurn; // signifies whiteplayer

    float WPscore, BPscore;

    Board theBoard;
    
    Player * WPlayer;
    Player * BPlayer;

    bool setupMode;
    bool gameStart; 
    
    public:
        ChessController();

        // Start a game, take in white and black player to
        // determine if it is Human or which Computer Level
        void start(std::string whitePlayer, std::string blackPlayer);

        void setup();

        // Method to determine when to call human makeMove() method
        void makeHumanMove(std::string start="", std::string end="", char promotion=' ');
        
        // Method to determine when to call Computer makeMove() method
        void makeComputerMove();

        int getPlayerTurn();
        
        string getPlayerTurnType(int pt);

        void resign();

        bool gameOver(); // new 

        bool gameStarted();

        void getScore(); // changed to void

        void reset();

        void getFinalScore();

        bool turnPiece(std::string pos); 

        //void attemptCopy();

        ~ChessController();

};

#endif


