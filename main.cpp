#include "src/Boats.hpp"

int main(){
    Gameplay game;
    game.populateUserBoard();
    game.populateComputerBoard();

    result_t moveResult = result_t::MISHIT;

    while(true)
    {
        do
        {
            moveResult = game.userMove();
            if (moveResult == result_t::ENDGAME)
            {
                std::cout << "Congratulations to user" << std::endl;
                return 0;
            }
        }
        while(moveResult == result_t::HIT_CONTINUE);

        do
        {
            moveResult = game.computerMove();
            if (moveResult == result_t::ENDGAME)
            {
                std::cout << "Congratulations to computer" << std::endl;
                return 0;
            }
        }
        while(moveResult == result_t::HIT_CONTINUE);
    }
}
