#include "src/Boats.hpp"

int main(){
    Gameplay game;
    game.populateUserBoard();
    game.populateComputerBoard();

    bool userWin = false;
    bool computerWin = false;

    while(!(userWin || computerWin))
    {
        userWin = game.userMove();
        computerWin = game.computerMove();
    }
    std:::cout << "Congratulation " << (userWin? "user" : "computer") << std::endl;
}
