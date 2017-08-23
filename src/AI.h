//
//  AI.hpp
//  connect4
//
//  Created by Varshini Ananta on 3/10/17.
//  Copyright © 2017 Varshini Ananta. All rights reserved.
//

#ifndef AI_h
#define AI_h

#include <stdio.h>
#include <cmath>
#include <algorithm>
#include "Board.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//set depth
//
#define LOOK_AHEAD 5
#define TABLE_SIZE 32000
#define TABLE_BIN_SIZE 10
//stating this class exists
class Board;
//TranspositionTable
typedef struct {
    Board*** bins;
} TranspositionTable;

//GameTreeNode
typedef struct {
    Board* board;
    int player;
    int other_player;
    int turn;
    
    int alpha;
    int beta;
    
    int best_move;
    
    TranspositionTable* ht;
} GameTreeNode;

class AI {
public:
    //constructor
    AI();
    //Evaluation function passes coordinates, board, player, returns int (score)
    int static countAt(Board* board, int x, int y, int player);
    //finds the amount of connected pieces for a given player in an array, used in countAt
    int static getIncrementForArray(int* arr, int player);
    //performs the evaluation function given the board and the players for the current move
    int static getHeuristic(Board board, int player, int other_player);
    //copies the board and returns said board after a given move
    Board* stateForMove(Board *board, int column, int player);
    //creates a hash value for the board
    unsigned long long hashBoard(Board board);
    //checks to see if two boards are equal
    int isBoardEqual(Board *board1, Board *board2);
    //create new hash table
    TranspositionTable* newTable();
    //find a board in the table
    Board* lookupInTable(TranspositionTable* t, Board* k);
    //add a board to the table
    void addToTable(TranspositionTable* t, Board* k);
    //deallocate hash table
    void freeTranspositionTable(TranspositionTable* t);
    //create tree node
    GameTreeNode* newGameTreeNode(Board* gs, int player, int other, int turn, int alpha, int beta, TranspositionTable* ht);
    //return the score for the current player
    int static heuristicForBoard(Board* gs, int player, int other);
    //ascending comparison
    int static ascComp(const void* a, const void* b);
    //descending comparison
    int static desComp(const void* a, const void* b);
    //main alpha-beta algorithm
    int getWeight(GameTreeNode* node, int movesLeft);
    //runs get-weight
    int getBestMove(GameTreeNode* node, int movesLeft);
    //runs newTable newGameTreeNode and getBestMove, frees the node and runs freeTranspositionTable, returns the best move
    int bestMove(Board* gs, int player, int other_player, int look_ahead);
    //runs bestMove
    int computerMove(int look_ahead, Board board);
    //keep track of the depth
    void retainGameState(Board* board);
    //go back one level or if the root node, free the board
    void freeGameState(Board* board);
private:
    //main game tree function (alpha-beta minimax)
    int CPU;
    int HUMAN;
};
#endif /* AI_h */
