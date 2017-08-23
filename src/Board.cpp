//
//  Board.cpp
//  connect4
//
//  Created by Varshini Ananta on 3/10/17.
//  Copyright � 2017 Varshini Ananta. All rights reserved.
//
//

#include "Board.h"

//Set size of board
void Board::init() {
    this->board = (int*) malloc(sizeof(int*) * 7 * 6);
    this->weight = 0;
    this->refs = 1;
    this->last_move = 0;
	this->clear();
}
//Clear the board
void Board::clear() {
		for (int i = 0; i<7*6; i++) {
			//notice j and i order (y and x respectively)
			board[i] = NO_VAL;
		}
}
//Print the board
void Board::print() const {
	for (int j = 0; j<6; j++) {

		for (int i = 0; i<7; i++) {
                std::cout << "(";
            if (getPlayerVal(i, j)==NO_VAL)
                std::cout << " " << ")";
            else if(getPlayerVal(i, j)==X_VAL){
                
                std::cout << "X" << ")";
            } else {
                std::cout << "O" << ")";
            }
		}
		std::cout << std::endl;
	}
	std::cout << " 1  2  3  4  5  6  7" << std::endl;
}
//Is the game over?
int Board::checkVictory() const {
	bool isTie = true;
    for (int i = 0; i<7; i++) {
        if (!slotFull(i))
            isTie = false;
    }
    if (isTie == true)
        return TIE;
    else{
        
        //Human victory
        //loop through entire board, skip NO_VAL's, and O_VAL's
        //check diagonals, horizontal, and vertical
        //if four X_VAL's in a row, return X_VAL
        
        //horizontal
        for (int j = 5; j >= 0; j--) {
            int count = 0;
            for (int i = 0; i<7; i++) {
                if (getPlayerVal(i, j) == X_VAL) {
                    count++;
                    if (count == 4)
                        break;
                }
                else
                    count = 0;
            }
            if (count >= 4)
                return X_VAL;
        }
        //vertical
        for (int i = 0; i<7; i++) {
            for (int j = 5; j >= 0; j--) {
                if (getPlayerVal(i, j) == X_VAL && getPlayerVal(i, j+1) == X_VAL&& getPlayerVal(i, j+2) == X_VAL&& getPlayerVal(i, j+3) == X_VAL) {
                    return X_VAL;
                }
            }
        }
        //diagonal
        for (int j = 0; j<6; j++){
            for (int i = 0; i<7; i++) {
                if(getPlayerVal(i, j)==X_VAL&&getPlayerVal(i+1, j-1)==X_VAL&&getPlayerVal(i+2, j-2)==X_VAL&&getPlayerVal(i+3, j-3)==X_VAL){
                    return X_VAL;
                }
            }
        }
        for(int j=0; j<6; j++){
            for(int i=0; i<7; i++){
                if(getPlayerVal(i, j)==X_VAL&&getPlayerVal(i-1, j-1)==X_VAL&&getPlayerVal(i-2, j-2)==X_VAL&&getPlayerVal(i-3, j-3)==X_VAL){
                    return X_VAL;
                }
            }
        }

        //Computer victory
        //horizontal
        for (int j = 5; j >= 0; j--) {
            int count = 0;
            for (int i = 0; i<7; i++) {
                if (getPlayerVal(i, j) == O_VAL) {
                    count++;
                    if (count == 4)
                        break;
                }
                else
                    count = 0;
            }
            if (count >= 4)
                return O_VAL;
        }
        //vertical
        for (int i = 0; i<7; i++) {
            for (int j = 5; j >= 0; j--) {
                if (getPlayerVal(i, j) == O_VAL && getPlayerVal(i, j+1) == O_VAL&& getPlayerVal(i, j+2) == O_VAL&& getPlayerVal(i, j+3) == O_VAL) {
                    return O_VAL;
                }
            }
        }
        //diagonal
        for (int j = 0; j<6; j++){
            for (int i = 0; i<7; i++) {
                if(getPlayerVal(i, j)==O_VAL&&getPlayerVal(i+1, j-1)==O_VAL&&getPlayerVal(i+2, j-2)==O_VAL&&getPlayerVal(i+3, j-3)==O_VAL){
                    return O_VAL;
                }
            }
        }
        for(int j=0; j<6; j++){
            for(int i=0; i<7; i++){
                if(getPlayerVal(i, j)==O_VAL&&getPlayerVal(i-1, j-1)==O_VAL&&getPlayerVal(i-2, j-2)==O_VAL&&getPlayerVal(i-3, j-3)==O_VAL){
                    return O_VAL;
                }
            }
        }
        return NO_VAL;
    }
}
//Stack values on top of each other
void Board::dropInSlot(int slot, int player) {
	if (player == X_VAL) {
		//loop through the rows of the slot
		for (int i = 5; i >= 0; i--) {
			if (getPlayerVal(slot, i) == NO_VAL) {
				board[slot *6 +i] = X_VAL;
				break;
			}
		}
	}
	else {
		for (int i = 5; i >= 0; i--) {
			if (getPlayerVal(slot, i) == NO_VAL) {
				board[slot*6+i] = O_VAL;
				break;
			}
		}
	}
    this->last_move = slot;
}
//Get the value of the board position
int Board::getPlayerVal(int x, int y) const {
	if (x<0 || x>6 || y<0 || y>5) {
		return OFF_BOARD;
	}
	else
		return this->board[x * 6 + y];
}
//Is the slot full?
bool Board::slotFull(int slot) const {
    int y;
    for (y = 0; y < 6; y++) {
        if (getPlayerVal(slot, y) == NO_VAL)
            return 0;
    }
    
    return 1;
}
