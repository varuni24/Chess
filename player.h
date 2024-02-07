#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "board.h"

using namespace std;

class Player {
    protected:
        Board* theBoard;
    private:
        Color c;
        string type;

    public:
        Player(Board* b, Color c, string t);

        // for human move
        virtual bool makeMove(string start, string end, char promotion = ' ') = 0;  
        
        // for computers random move
        virtual bool makeMove(int level) = 0;                                       
        string getType();
        Color getColor();
        virtual int getLevel() = 0;
        virtual ~Player() = default;
    };

#endif



