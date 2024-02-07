#include "player.h"
#include "piece.h"
#include <iostream>
using namespace std;


Player::Player(Board *b, Color c, string t) : theBoard{b}, c{c} , type{t} {}

Color Player::getColor(){ // added 
    return c;
}

string Player::getType() {
    return type;
}

