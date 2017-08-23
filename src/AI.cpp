//
//  AI.cpp
//  connect4
//
//  Created by Varshini Ananta on 3/10/17.
//  Copyright © 2017 Varshini Ananta. All rights reserved.
//
//

#include "AI.h"
using namespace std;

AI::AI(){
    HUMAN = X_VAL;
    CPU = O_VAL;
}

int AI::getIncrementForArray(int* arr, int player) {
    int toR = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (arr[i] == player) {
            toR = 1;
            continue;
        }
        
        if (arr[i] != player && arr[i] != NO_VAL) {
            return 0;
        }
    }
    
    return toR;
}

int AI::countAt(Board* board, int x, int y, int player) {
    
    // check across
    int found = 0;
    int buf[4];
    int i;
    
    for (i = 0; i < 4; i++) {
        buf[i] = board->getPlayerVal(x+i, y);
    }
    
    found += getIncrementForArray(buf, player);
    
    // check down
    for (i = 0; i < 4; i++) {
        buf[i] = board->getPlayerVal(x, y+i);
    }
    found += getIncrementForArray(buf, player);
    
    // check diag +/+
    for (i = 0; i < 4; i++) {
        buf[i] = board->getPlayerVal(x+i, y+i);
    }
    found += getIncrementForArray(buf, player);
    
    // check diag -/+
    for (i = 0; i < 4; i++) {
        buf[i] = board->getPlayerVal(x-i, y+i);
    }
    found += getIncrementForArray(buf, player);
    
    return found;
}

int AI::getHeuristic(Board board, int player, int other_player) {
    int toR = 0;
    int x, y;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 6; y++) {
            toR += countAt(&board, x, y, player);
            toR -= countAt(&board, x, y, other_player);
        }
    }
    
    return toR;
}

Board* AI::stateForMove(Board* board, int column, int player) {
    if (board==NULL || board->board == NULL)
        return NULL;
    Board* toR = new Board();
    toR->init();
    if(toR == NULL)
        return NULL;
    memcpy(toR->board, board->board, sizeof(int*)*7*6);
    toR->dropInSlot(column, player);
    return toR;
}

unsigned long long AI::hashBoard(Board board) {
    unsigned long long hash = 14695981039346656037Lu;
    for (int j = 0; j< 6; j++){
        for (int i = 0; i < 7; i++) {
            hash ^= board.getPlayerVal(i, j);
            hash *= 1099511628211Lu;
        }
    }
    return hash;
}

int AI::isBoardEqual(Board* board1, Board* board2) {
    for (int j=0; j<6; j++){
        for (int i = 0; i < 7; i++) {
            if (board1->getPlayerVal(i, j) == board2->getPlayerVal(i, j))
            continue;
            return 0;
        }
    }
    
    return 1;
}



TranspositionTable* AI::newTable() {
    int i, j;
    TranspositionTable* toR = (TranspositionTable*) malloc(sizeof(TranspositionTable));
    toR->bins = (Board***) malloc(sizeof(Board**) * TABLE_SIZE);
    for (i = 0; i < TABLE_SIZE; i++) {
        toR->bins[i] = (Board**) malloc(sizeof(Board*) * TABLE_BIN_SIZE);
        for (j = 0; j < TABLE_BIN_SIZE; j++) {
            toR->bins[i][j] = NULL;
        }
    }
    
    return toR;
}

Board* AI::lookupInTable(TranspositionTable* t, Board *k) {
    int hv = hashBoard(*k) % TABLE_SIZE;
    int i;
    Board** bin = t->bins[hv];
    for (i = 0; i < TABLE_BIN_SIZE; i++) {
        if (bin[i] == NULL) {
            return NULL;
        }
        
        if (isBoardEqual(k, bin[i])) {
            return bin[i];
        }
    }
    return NULL;
}

void AI::addToTable(TranspositionTable* t, Board* k) {
    int hv = hashBoard(*k) % TABLE_SIZE;
    Board** bin = t->bins[hv];
    for (int i = 0; i < TABLE_BIN_SIZE; i++) {
        if (bin[i] == NULL) {
            bin[i] = k;
            retainGameState(k);
            return;
        }
    }
    
    fprintf(stderr, "Overflow in hash bin %d, won't store Board\n", hv);
}

void AI::freeTranspositionTable(TranspositionTable* t) {
    int i, j;
    for (i = 0; i < TABLE_SIZE; i++) {
        for (j = 0; j < TABLE_BIN_SIZE; j++) {
            if (t->bins[i][j] != NULL)
                freeGameState(t->bins[i][j]);
        }
        free(t->bins[i]);
    }
    free(t->bins);
    
    free(t);
}

GameTreeNode* AI::newGameTreeNode(Board* gs, int player, int other, int turn, int alpha, int beta, TranspositionTable* ht) {
    GameTreeNode* toR = (GameTreeNode*) malloc(sizeof(GameTreeNode));
    toR->board = gs;
    toR->player = player;
    toR->other_player = other;
    toR->turn = turn;
    toR->alpha = alpha;
    toR->beta = beta;
    toR->best_move = -1;
    toR->ht = ht;
    return toR;
}

int AI::heuristicForBoard(Board* board, int player, int other) {
    if (board->checkVictory()==TIE)
        return 0;
    
    int term_stat = board
    ->checkVictory();
    if (term_stat == player)
        return 1000;
    
    else if (term_stat == other)
        return -1000;
    
    
    return getHeuristic(*board, player, other);
    
}


// using a global instead of qsort_r because of emscripten support

GameTreeNode* g_node = NULL;


int AI::ascComp(const void* a, const void* b) {
    GameTreeNode* node = g_node;
    return heuristicForBoard(*(Board**) a, node->player, node->other_player) -heuristicForBoard(*(Board**) b, node->player, node->other_player);
    
}

int AI::desComp(const void* a, const void* b) {
    GameTreeNode* node = g_node;
    return heuristicForBoard(*(Board**) b, node->player, node->other_player) -
    heuristicForBoard(*(Board**) a, node->player, node->other_player);
    
}

int AI::getWeight(GameTreeNode* node, int movesLeft) {
    int toR=0, move, best_weight;
    bool breakflag = false;
    if (node->board->checkVictory()!=NO_VAL || movesLeft == 0)
        return heuristicForBoard(node->board, node->player, node->other_player);
    
    Board** possibleMoves = (Board**) malloc(sizeof(Board*) * 7);
    int validMoves = 0;
    for (int possibleMove = 0; possibleMove < 7; possibleMove++) {
        if (node->board->slotFull(possibleMove)) {
            continue;
        }
        
        possibleMoves[validMoves] = stateForMove(node->board, possibleMove, (node->turn ? node->player : node->other_player));
        validMoves++;
    }
    
    // order possibleMoves by the heuristic
    g_node = node;
    if (node->turn) {
        std::qsort(possibleMoves, validMoves, sizeof(Board*), ascComp);
    } else {
        std::qsort(possibleMoves, validMoves, sizeof(Board*), desComp);
    }
    
    best_weight = (node->turn ? INT_MIN : INT_MAX);
    
    for (move = 0; move < validMoves; move++) {
        // see if the board is already in the hash table
        Board* inTable = lookupInTable(node->ht, possibleMoves[move]);
        int child_weight;
        int child_last_move;
        if (inTable != NULL) {
            child_weight = inTable->weight;
            child_last_move = possibleMoves[move]->last_move;
            
        } else {
            GameTreeNode* child = newGameTreeNode(possibleMoves[move], node->player, node->other_player, !(node->turn),
                                                  node->alpha, node->beta, node->ht);
            child_weight = getWeight(child, movesLeft - 1);
            child_last_move = child->board->last_move;
            free(child);
        }
        
        
        
        possibleMoves[move]->weight = child_weight;
        addToTable(node->ht, possibleMoves[move]);
        
        if (movesLeft == LOOK_AHEAD){
            printf("Move %d has weight %d\n", child_last_move+1, child_weight);

        }
        // alpha-beta pruning
        if (!node->turn) {
            // min node
            if (child_weight <= node->alpha) {
                // MAX ensures we will never go here
                toR = child_weight;
                breakflag = true;
                break;
            }
            node->beta = (node->beta < child_weight ? node->beta : child_weight);
        } else {
            // max node
            if (child_weight >= node->beta) {
                // MIN ensures we will never go here
                toR = child_weight;
                breakflag = true;
                break;
            }
            node->alpha = (node->alpha > child_weight ? node->alpha : child_weight);
        }
        
        if (!(node->turn)) {
            // min node
            if (best_weight > child_weight) {
                best_weight = child_weight;
                node->best_move = child_last_move;
            }
        } else {
            // max node
            if (best_weight < child_weight) {
                best_weight = child_weight;
                node->best_move = child_last_move;
            }
        }
        
        
    }
    if(!breakflag)
        toR = best_weight;
    
    for (int i = 0; i < validMoves; i++) {
        freeGameState(possibleMoves[i]);
    }
    
    free(possibleMoves);
    return toR;
}

int AI::getBestMove(GameTreeNode* node, int movesLeft) {
    getWeight(node, movesLeft);
    return node->best_move;
}



int AI::bestMove(Board* gs, int player, int other_player, int look_ahead) {
    TranspositionTable* t1 = newTable();
    GameTreeNode* n = newGameTreeNode(gs, player, other_player, 1, INT_MIN, INT_MAX, t1);
    int move = getBestMove(n, look_ahead);
    free(n);
    freeTranspositionTable(t1);
    return move;
}

int AI::computerMove(int look_ahead, Board board) {
    return bestMove(&board, CPU, HUMAN, look_ahead);
}

void AI::freeGameState(Board* board) {
    board->refs--;
    if(board->refs <=0){;
        free(board->board);
        free(board);
    }
}

void AI::retainGameState(Board* board) {
    board->refs++;
}
