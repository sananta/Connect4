//
//  MainGame.cpp
//  connect4
//
//  Created by Varshini Ananta on 3/10/17.
//  Copyright © 2017 Varshini Ananta. All rights reserved.
//
//

#include "MainGame.h"
//Main game loop
void MainGame::run(){
    bool cheat = init();
    while (gameState != State::EXIT){
        connect4.print();
        // Say which player is playing
        if (currentPlayer == X_VAL) {
            printf("Human's turn!\n\n");
            if (cheat)
                ai.bestMove(&connect4, X_VAL, O_VAL, LOOK_AHEAD);
            playerMove(X_VAL);
        } else {
            printf("Computer's turn!\n\n");
            aiMove(O_VAL);
        }
        int end = connect4.checkVictory();
        if (end!=NO_VAL){
            endGame(end == TIE);
        } else {
            changePlayer();
            // clear screen
            for (int i = 0; i < 5; i++) {
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            }
        }
    }
}
//Start the game
bool MainGame::init(){
    gameState = State::PLAYING;
    connect4.init();
    currentPlayer = X_VAL;
    std::cout << "Welcome to Connect 4 AI" << std::endl;
    std::cout << "Your only chance of winning is to play first," << std::endl;
    std::cout << "therefore you will make the first move." << std::endl;
    std::cout << "Do you want to cheat? y/n" << std::endl;
    char cheat;
    std::cin >> cheat;
    if (!std::cin){
        while(!std::cin){
            std::cin.clear();
            std::cin.ignore(255, '\n');
            std::cout << "Invalid input, try again: ";
            std::cin >> cheat;
        }
    }
    if(cheat == 'y')
        return true;
    else
        return false;

}
//Human move
void MainGame::playerMove(int player){
    int slot;
    std::cout << "Enter slot number: ";
    std::cin >> slot;
    if (!std::cin){
        while(!std::cin){
            std::cin.clear();
            std::cin.ignore(255, '\n');
            std::cout << "Invalid input, try again: ";
            std::cin >> slot;
        }
    } else if (connect4.slotFull(slot-1)){
        while(connect4.slotFull(slot-1)){
            std::cout << "Slot full or invalid! Try again: ";
            std::cin.clear();
            std::cin.ignore(255, '\n');
            std::cin >> slot;
        }
    }
    connect4.dropInSlot(slot-1, player);
}
//CPU move
void MainGame::aiMove(int player){
    int move = ai.computerMove(LOOK_AHEAD, connect4);
    connect4.dropInSlot(move, player);
    std::cout << "Computer chose slot " << move+1 << std::endl;
    std::cout << "Press return" << std::endl;
    //std::cin.ignore();
    //std::cin.ignore();
}
//Switch players
void MainGame::changePlayer(){
    if (currentPlayer == X_VAL) {
        currentPlayer = O_VAL;
    } else {
        currentPlayer = X_VAL;
    }
}
//Game over
void MainGame::endGame(bool wasTie){
    
    connect4.print();
    
    if (wasTie) {
        std::cout << "Tie game! Enter any key to play again, or E to exit: " << std::endl;;
    } else {
        if (currentPlayer == X_VAL) {
            std::cout << "Human wins! Enter any key to play again, or E to exit: " << std::endl;
        } else {
            std::cout << "Computer wins! Enter any key to play again, or E to exit: " << std::endl;
        }
    }
    
    char input;
    std::cin >> input;
    if (input == 'E' || input == 'e') {
        gameState = State::EXIT;
    } else {
        // Re init the game
        init();
    }
}


