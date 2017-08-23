//
//  Board.hpp
//  connect4
//
//  Created by Varshini Ananta on 3/10/17.
//  Copyright © 2017 Varshini Ananta. All rights reserved.
//
//

#ifndef Board_h
#define Board_h
#include <stdio.h>
#include <iostream>
#include <vector>
#include "AI.h"

#define OFF_BOARD -2
const int TIE = -1;
// board values
const int NO_VAL = 0;
const int X_VAL = 1;
const int O_VAL = 2;
struct GameState;
// Game board class
class Board {
public:
    int last_move;
    int weight;
    int refs;
    // Initializes the board
    void init();
    // Clears the board
    void clear();
    // Prints the board to standard output
    void print() const;
    
    // If no player won, returns NO_VAL. Otherwise returns X_VAL, O_VAL or TIE_VAL
    int checkVictory() const;
    
    // Drop a coin in the slot
    void dropInSlot(int slot, int player);
    
    // Gets value at x,y spot
    int getPlayerVal(int x, int y) const;
    
    bool slotFull(int slot) const;
    
    int* board;
private:
    
};

#endif /* Board_h */
