#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <map>
#include "player.h"
using namespace std;


class Computer : public Player {
    int level;

    // Vector of possible promotion options for white player
    vector<char> promotionWhite = {'R', 'N', 'B', 'Q'};

    // Vector of possible promotion options for black player
    vector<char> promotionBlack = {'r', 'n', 'b', 'q'};

    // If it is valid to move Piece(p) to the "end" coordinate
    bool isValidMove(std::pair<int, int> end, Piece *p);

    // Execute all the update and set new coordinates for 
    // any piece that just moved for the computer player.
    bool doMove(Board* theBoard, string start, string end, char promotion);    
    public:

        Computer(Board* b, Color c, char level);
        bool makeMove(string start, string end, char promotion = ' ') override;
        bool makeMove(int level) override;
        bool generateLevel1Move();
        bool generateLevel2Move();
        bool generateLevel3Move();
        bool generateLevel4Move();
        int getLevel() override;

};

#endif


