#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "player.h"

using namespace std;

class Human : public Player {
    public:
        Human(Board* b, Color c);

        // Override human's makemove
        bool makeMove(string start, string end, char promotion = ' ') override;

        // Automatically return false since this is meant
        // for computer to make a move
        bool makeMove(int level) override; 

        int getLevel() override;

};

#endif


