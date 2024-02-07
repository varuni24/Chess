#include <iostream>
#include "computer.h"
using namespace std;

Computer::Computer(Board* b, Color c, char level) : Player(b, c, "computer"), level(level - '0') {
}


int Computer::getLevel(){
    return level;
}

bool Computer::makeMove(string start, string end, char promotion) { return false; }

bool Computer::makeMove(int level) {
    if (level == 1) return generateLevel1Move();
    if (level == 2) return generateLevel2Move();
    if (level == 3) return generateLevel3Move();
    if (level == 4) return generateLevel4Move();
    return false;
}

bool Computer::doMove(Board* theBoard, string start, string end, char promotion) {
    int startRow = '8' - start[1];
    int startCol = start[0] - 'a';
    int endRow = '8' - end[1];
    int endCol = end[0] - 'a';
    Piece* startingPiece = theBoard->getPiece(startRow, startCol);
    
    // If Piece is Pawn, there are 2 Special Moves that Cells will
    // need to be updated/notified differently that a normal move.

    if (startingPiece && startingPiece->getType() == PieceType::Pawn) {
        // If Pawn move is Enpassant, 3 Cells are affected
        if (startingPiece->isEnpassed(endRow, endCol, theBoard) && 
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {

            // Notify and update Start and Ending Cell of our Pawn
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

            // Update the Cell that was captured, which is not the start or end Cell
            if (startingPiece->getColor() == Color::Black) { 
                
                // If black is doing Enpassant
                theBoard->removePiece(endRow - 1, endCol);
                theBoard->update(startRow, startCol, endRow - 1, endCol, nullptr);
            } else { 

                // If White is doing Enpassant
                theBoard->removePiece(endRow + 1, endCol);
                theBoard->update(startRow, startCol, endRow + 1, endCol, nullptr);
            }

            // Set coordinates and count for our piece that just moved
            startingPiece->setCoords(endRow, endCol);
            startingPiece->setCount(startingPiece->getCount() + 1);

            // NEW
            theBoard->setLastMove(startingPiece);
            return true;
        }
        // If Pawn promotion, must make Pawn into new PieceType and update location
        if (startingPiece->isPromotion(endRow, endCol, theBoard, promotion) &&
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {
            
            // Make new piece for promotion (setupPiece handles deleting old piece)
            theBoard->setupPiece(startRow, startCol, promotion);
            Piece* promote = theBoard->getPiece(startRow, startCol);
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, promote);
            promote->setCoords(endRow, endCol);
            promote->setCount(promote->getCount() + 1);

            // NEW
            theBoard->setLastMove(promote);
            return true;
        }

    } else if (startingPiece && startingPiece->getType() == PieceType::King) {

        // If piece is King, Castling move requires different updating
        // and notifying than a normal move

        if (startingPiece->isCastle(endRow, endCol, theBoard) && 
            !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {   

            // King Side Castling         
            if (startingPiece->getCol() + 2 == endCol) {
                Piece* destinationPiece = theBoard->getPiece(endRow, endCol + 1);

                // Update/Notify Rook's Movement
                theBoard->update(endRow, endCol + 1, endRow, endCol + 1, nullptr);
                theBoard->update(endRow, endCol - 1, endRow, endCol - 1, destinationPiece);

                // Update/Notify King's Movement
                theBoard->update(startRow, startCol, startRow, startCol, nullptr);
                theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

                // Update King and Rook coordinates and moveCount
                startingPiece->setCoords(endRow, endCol);
                destinationPiece->setCoords(endRow, endCol - 1);
                startingPiece->setCount(startingPiece->getCount() + 1);
                destinationPiece->setCount(destinationPiece->getCount() + 1);

                //NEW
                theBoard->setLastMove(startingPiece);
                return true;
            }

            // Queen Side Castling
            if (startingPiece->getCol() - 2 == endCol) {
                Piece* destinationPiece = theBoard->getPiece(endRow, endCol - 2);

                // Update/Notify King's Movement
                theBoard->update(startRow, startCol, startRow, startCol, nullptr);
                theBoard->update(startRow, startCol, endRow, endCol, startingPiece);

                // Update/Notify Rook's Movement
                theBoard->update(endRow, endCol - 2, endRow, endCol - 2, nullptr);
                theBoard->update(endRow, endCol + 1, endRow, endCol + 1, destinationPiece);

                // Update Rook and King's coordinates and moveCount
                startingPiece->setCoords(endRow, endCol);
                startingPiece->setCount(startingPiece->getCount() + 1);
                destinationPiece->setCoords(endRow, endCol + 1);
                destinationPiece->setCount(destinationPiece->getCount() + 1);

                // NEW
                theBoard->setLastMove(startingPiece);
                return true;

            }
        }
    }
    // Update Piece Coords and Notify Display for a normal move that affects only 2 Cells
    if (startingPiece && startingPiece->makeMove(endRow, endCol, theBoard) &&
        !startingPiece->moveProduceCheck(endRow, endCol, theBoard, promotion)) {
            theBoard->removePiece(endRow, endCol);
            startingPiece->setCoords(endRow, endCol);
            startingPiece->setCount(startingPiece->getCount() + 1);
            theBoard->update(startRow, startCol, startRow, startCol, nullptr);
            theBoard->update(startRow, startCol, endRow, endCol, startingPiece);  
            
            // NEW
            theBoard->setLastMove(startingPiece);
            return true;
    }
    return false;
}


bool Computer::isValidMove(pair<int, int> end, Piece *p) {

    // Check if the piece at the ending position belongs to the opponent player
    if (!p) return false;
    if (theBoard->getPiece(end.first, end.second) && 
        theBoard->getPiece(end.first, end.second)->getColor() == p->getColor()) return false;
    return (p->moveable(end.first, end.second, theBoard, ' ') &&
            !p->moveProduceCheck(end.first, end.second, theBoard, ' '));
}

bool Computer::generateLevel1Move() {
    vector<pair<pair<int, int>, pair<int, int>>> possible_moves; // [ [[start], [end]],....]
    for (int i = 0; i < 8; ++i) {  // i, j for loop to get starting position(i.e any piece of curr player)
        for (int j = 0; j < 8; ++j) {
            if (theBoard->getPiece(i,j) && theBoard->getPiece(i,j)->getColor() == this->getColor()){    // is my piece
                for (int x = 0; x < 8; ++x) {       // // x,y for loop to get ending position
                    for (int y = 0; y < 8; ++y) {

                        pair<int, int> start = {i, j};
                        pair<int, int> end = {x, y};
                        pair<pair<int, int>, pair<int, int>> move = {start, end};
                        if (isValidMove(end, (theBoard->getPiece(i,j)))) {
                            possible_moves.emplace_back(move);
                        }
                    }
                }
            }
        }
    }

    if (possible_moves.empty()) return false;  // no legal moves
    int randInd = rand() % possible_moves.size();
    pair<int, int> starting= possible_moves[randInd].first;
    pair<int, int> ending = possible_moves[randInd].second;

    string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
    string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
    char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
    
    // Call doMove to actually update and notifies Cells
    // Pieces' position that were changed
    return doMove(theBoard, start, end, promotion);
}


bool Computer::generateLevel2Move() {
    vector<pair<pair<int, int>, pair<int, int>>> allowed_moves;
    vector<pair<pair<int, int>, pair<int, int>>> other_moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (theBoard->getPiece(i, j) && theBoard->getPiece(i, j)->getColor() == this->getColor()) {
                
                int oppositeColor = (getColor() == Color::White) ? 2 : 1;
                
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        pair<int, int> start = {i, j};
                        pair<int, int> end = {x, y};
                        pair<pair<int, int>, pair<int, int>> move = {start, end};

                        if (isValidMove(end, (theBoard->getPiece(i, j)))) {
                            Board tempBoard = *theBoard;
                            bool captureMove = tempBoard.isCapturingMove(start.first, start.second, end.first, end.second);
                            string start = string(1, j + 'a') + string(1, '8' - i);
                            string end = string(1, y + 'a') + string(1, '8' - x);

                            // Randomize promotion piece in case it is a promotion case
                            char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
                            
                            // Does the actual move temporarily to check if 
                            // the move puts our opponent in check
                            doMove(&tempBoard, start, end, promotion);
                            
                            if (captureMove || tempBoard.isCheck(oppositeColor)) {
                                allowed_moves.emplace_back(move); 
                            }
                            else other_moves.emplace_back(move);
                        }
                    }
                }
            }
        }
    }
    if (allowed_moves.empty() && other_moves.empty()) return false;
    if (!allowed_moves.empty()) {
        int rand_index = rand() % allowed_moves.size();
        pair<int, int> starting = allowed_moves[rand_index].first;
        pair<int, int> ending = allowed_moves[rand_index].second;
     
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        cout << "Made a level 2 Computer Move" << endl;

        return doMove(theBoard, start, end, promotion);   
    } else if (!other_moves.empty()) {
        int rand_index = rand() % other_moves.size();                
        pair<int, int> starting = other_moves[rand_index].first;
        pair<int, int> ending = other_moves[rand_index].second;

        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        cout << "Made a level 2 Computer Move" << endl;
        return doMove(theBoard, start, end, promotion); 
    }

    return false;
}



bool Computer::generateLevel3Move() {

    vector<pair<pair<int, int>, pair<int, int>>> avoid_capture_moves;
    vector<pair<pair<int, int>, pair<int, int>>> allowed_moves;
    vector<pair<pair<int, int>, pair<int, int>>> other_moves;

    int oppositeColor = (getColor() == Color::White) ? 2 : 1;

    // generate vector of moves
    // for each cell
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // get my piece
            if (theBoard->getPiece(i, j) && theBoard->getPiece(i, j)->getColor() == this->getColor()) {

                // for each cell
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        pair<int, int> start = {i, j};
                        pair<int, int> end = {x, y};

                        
                        pair<pair<int, int>, pair<int, int>> move = {start, end};

                        if (isValidMove(end, (theBoard->getPiece(i, j)))) {

                            bool wasUnderThreat = false;
                        
                            // is the piece at i j under threat?
                            if (theBoard->isUnderThreat(i,j, this->getColor())){
                                wasUnderThreat = true;
                            }
                            Board tempBoard = *theBoard;

                            // this doesn't make the move.
                            bool captureMove = tempBoard.isCapturingMove(start.first, start.second, end.first, end.second);
                            
                            // simulate making that move by using temo board
                            string start = string(1, j + 'a') + string(1, '8' - i);
                            string end = string(1, y + 'a') + string(1, '8' - x);
                            //

                            char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
                            doMove(&tempBoard, start, end, promotion);

                            // Check if after move we are still in danger
                            bool stillInThreat = tempBoard.isUnderThreat(x,y,this->getColor());

                            // is the piece still under threat even after moving?
                            if (wasUnderThreat == true && stillInThreat == false) { // aka an avaoiding being captured move
                                avoid_capture_moves.emplace_back(move); 
                            }
                            if (captureMove || tempBoard.isCheck(oppositeColor)) {
                                allowed_moves.emplace_back(move); 
                            }
                            else other_moves.emplace_back(move);
                        }
                    }
                }
            }
        }
    }

    // actual moves/updating
    if (allowed_moves.empty() && other_moves.empty() && avoid_capture_moves.empty()) return false;

    if (!avoid_capture_moves.empty()){
        int rand_index = rand() % avoid_capture_moves.size();
        pair<int, int> starting = avoid_capture_moves[rand_index].first;
        pair<int, int> ending = avoid_capture_moves[rand_index].second;

        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard,start,end, promotion);
    }
    else if (!allowed_moves.empty()) {
        int rand_index = rand() % allowed_moves.size();
        pair<int, int> starting = allowed_moves[rand_index].first;
        pair<int, int> ending = allowed_moves[rand_index].second;

        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first); 
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);     
        return doMove(theBoard,start,end, promotion);

    } else if (!other_moves.empty()) {
        int rand_index = rand() % other_moves.size();                                       // repeated code
        pair<int, int> starting = other_moves[rand_index].first;
        pair<int, int> ending = other_moves[rand_index].second;
        
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard,start,end, promotion);
    }
    return true;
}



bool Computer::generateLevel4Move() {
    vector<pair<pair<int, int>, pair<int, int>>> capturing_moves_and_avoid_capture_moves;
    vector<pair<pair<int, int>, pair<int, int>>> avoid_capture_moves;
    vector<pair<pair<int, int>, pair<int, int>>> capturing_moves;
    vector<pair<pair<int, int>, pair<int, int>>> checking_moves;
    vector<pair<pair<int, int>, pair<int, int>>> other_moves;


    pair<int,int> highest_capture;

    int oppositeColor = (getColor() == Color::White) ? 2 : 1;

    // Generate vector of moves
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (theBoard->getPiece(i, j) && theBoard->getPiece(i, j)->getColor() == this->getColor()) {
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        pair<int, int> start = {i, j};
                        pair<int, int> end = {x, y};

                        pair<pair<int, int>, pair<int, int>> move = {start, end};

                        
                        
                        if (isValidMove(end, (theBoard->getPiece(i, j)))) {

                            // is the piece at i j under threat?
                            bool wasUnderThreat = false;
                            if (theBoard->isUnderThreat(i,j, this->getColor())){
                                wasUnderThreat = true;
                            }

                            Board tempBoard = *theBoard;
                            bool captureMove = tempBoard.isCapturingMove(start.first, start.second, end.first, end.second);

                            // simulate making that move by using temp board
                            string start = string(1, j + 'a') + string(1, '8' - i);
                            string end = string(1, y + 'a') + string(1, '8' - x);
                            char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
                            doMove(&tempBoard, start, end, promotion);
                            //

                            bool stillInThreat = tempBoard.isUnderThreat(x,y,this->getColor());

                            // is the piece still under threat even after moving?
                            if ((captureMove && stillInThreat == false)) { 
                                capturing_moves_and_avoid_capture_moves.emplace_back(move); 
                            } else if (wasUnderThreat && stillInThreat == false) {
                                avoid_capture_moves.emplace_back(move); 
                            } else if (captureMove && stillInThreat == false) {
                                capturing_moves.emplace_back(move); 
                            } else if (tempBoard.isCheck(oppositeColor) && stillInThreat == false) {
                                checking_moves.emplace_back(move); 
                            } else {
                                other_moves.emplace_back(move);
                            }
                        }
                    }
                }
            }
        }
    }

    // Priority 1: Avoid being captured by capturing the opponent's
    // Piece. However, we prioritize capturing opponent's Queen,
    // Rook, Knight, Bishop and Pawn in descending order
    if (!capturing_moves_and_avoid_capture_moves.empty()) {
        char bestCapture;
        char curPiece;
        pair<pair<int, int>, pair<int, int>> bestMove = {{-1, -1,}, {-1, -1}};
        int count = 0;
        int pColor = (getColor() == Color::White) ? 2 : 1;

        for (auto move : capturing_moves_and_avoid_capture_moves) {
            if (count == 0) {
                if (pColor == 1) {
                    bestCapture = 'P';
                    curPiece = 'P';
                }
                else {
                    bestCapture = 'p';
                    curPiece = 'p';
                }
            }

            auto p = theBoard->getPiece(move.second.first, move.second.second);
            if (!p) continue;
            
            if  (p->getType() == PieceType::Queen) {
                curPiece = (p->getColor() == Color::White) ? 'Q' : 'q';
            }
            if  (p->getType() == PieceType::Rook) {
                curPiece = (p->getColor() == Color::White) ? 'R' : 'r';
            }
            if  (p->getType() == PieceType::Bishop) {
                curPiece = (p->getColor() == Color::White) ? 'B' : 'b';
            }
            if  (p->getType() == PieceType::Knight) {
                curPiece = (p->getColor() == Color::White) ? 'N' : 'n';
            }
            if  (p->getType() == PieceType::Pawn) {
                curPiece = (p->getColor() == Color::White) ? 'P' : 'p';
            }

            // Compare if curpiece better than current bestPiece, 
            // make best piece to that, else dont do anything

            if (curPiece == 'Q' || curPiece == 'q') {
                bestCapture = (p->getColor() == Color::White) ? 'Q' : 'q';
                bestMove = move;
                break;
            } else if ((curPiece == 'R' || curPiece == 'r') && (bestCapture == 'p' || bestCapture == 'P' ||
                       bestCapture == 'B' || bestCapture == 'b' || bestCapture == 'N' || bestCapture == 'n')) {

                bestCapture = (p->getColor() == Color::White) ? 'R' : 'r';      
                bestMove = move;    
            } else if ((curPiece == 'B' || curPiece == 'b') && (bestCapture == 'p' || bestCapture == 'P' ||
                        bestCapture == 'N' || bestCapture == 'n')) {

                bestCapture = (p->getColor() == Color::White) ? 'B' : 'b';
                bestMove = move;
            } else if ((curPiece == 'N' || curPiece == 'n') && (bestCapture == 'p' || bestCapture == 'P')) {
                bestCapture = (p->getColor() == Color::White) ? 'N' : 'n';
                bestMove = move;
            } else if ((curPiece == 'p' || curPiece == 'P')) {
                bestCapture = (p->getColor() == Color::White) ? 'P' : 'p';
                bestMove = move;
            }
            count += 1;
        }

        // If we found a bestMove
        if (!(bestMove.first.first == -1 && bestMove.first.second == -1 &&
            bestMove.second.first == -1 && bestMove.second.first == -1)) {
           
            string start = string(1, bestMove.first.second + 'a') + string(1, '8' - bestMove.first.first);
            string end = string(1, bestMove.second.second + 'a') + string(1, '8' - bestMove.second.first);
            char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
            return doMove(theBoard, start, end, promotion);      
        }\

        // If we did not, randomize move selection
        int rand_index = rand() % capturing_moves_and_avoid_capture_moves.size();
        pair<int, int> starting = capturing_moves_and_avoid_capture_moves[rand_index].first;
        pair<int, int> ending = capturing_moves_and_avoid_capture_moves[rand_index].second;
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard, start, end, promotion);
    }

    // Priority 2: Avoiding capture moves
    if (!avoid_capture_moves.empty()) {
        int rand_index = rand() % avoid_capture_moves.size();
        pair<int, int> starting = avoid_capture_moves[rand_index].first;
        pair<int, int> ending = avoid_capture_moves[rand_index].second;
        
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard, start, end, promotion);
    }

    // Priority 3: Capturing moves
    if (!capturing_moves.empty()) {
        int rand_index = rand() % capturing_moves.size();
        pair<int, int> starting = capturing_moves[rand_index].first;
        pair<int, int> ending = capturing_moves[rand_index].second;
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard, start, end, promotion);
    }
    
    // Priority 4: Checking without putting in danger option
    if (!checking_moves.empty()) {
        int rand_index = rand() % checking_moves.size();
        pair<int, int> starting = checking_moves[rand_index].first;
        pair<int, int> ending = checking_moves[rand_index].second;
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard, start, end, promotion);
    }

    // Priority 5: Other valid moves
     if (!other_moves.empty()) {
        int rand_index = rand() % other_moves.size();
        pair<int, int> starting = other_moves[rand_index].first;
        pair<int, int> ending = other_moves[rand_index].second;
        
        string start = string(1, starting.second + 'a') + string(1, '8' - starting.first);
        string end = string(1, ending.second + 'a') + string(1, '8' - ending.first);
        char promotion = (this->getColor() == Color::White ? promotionWhite[rand() % 4] : promotionBlack[rand() % 4]);
        return doMove(theBoard, start, end, promotion);
    }

    return false;

}


