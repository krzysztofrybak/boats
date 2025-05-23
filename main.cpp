#include "src/Boats.hpp"

int main(){

    std::ofstream logFile("log.txt");

    Gameplay game(logFile);

    logFile << "Populate user board" << std::endl;
    game.populateUserBoard();
    game.populateComputerBoard();

    result_t moveResult = result_t::MISHIT;

    while(true)
    {
        logFile << "Next round." << std::endl;
        logFile << "User board:\n" << game.user_board << std::endl;
        logFile << "User hit board:\n" << game.user_hit_board << std::endl;
        logFile << "Computer board:\n" << game.computer_board << std::endl;
        logFile << "Computer hit board:\n" << game.computer_hit_board << std::endl;

        do
        {
            logFile << "User move." << std::endl;
            moveResult = game.userMove();
            if (moveResult == result_t::ENDGAME)
            {
                std::cout << game.user_hit_board << std::endl;
                std::cout << "Congratulations to user" << std::endl;
                logFile.close();
                return 0;
            }
            else if(moveResult == result_t::HIT_CONTINUE)
            {
                std::cout << "Trafiony plynie dalej..." << std::endl;
            }
            else if(moveResult == result_t::HIT_DONE)
            {
                std::cout << "Trafiony zatopiony." << std::endl;
            }
            else
            {
                std::cout << "Pudlo." << std::endl;
            }
        }
        while(moveResult == result_t::HIT_CONTINUE);

        do
        {
            logFile << "Computer move." << std::endl;
            moveResult = game.computerMove();
            if (moveResult == result_t::ENDGAME)
            {
                std::cout << "Congratulations to computer" << std::endl;
                logFile.close();
                return 0;
            }
            else if(moveResult == result_t::HIT_CONTINUE)
            {
                logFile << "Trafiony plynie dalej..." << std::endl;
            }
            else if(moveResult == result_t::HIT_DONE)
            {
                logFile << "Trafiony zatopiony." << std::endl;
            }
            else
            {
                logFile << "Pudlo." << std::endl;
            }
        }
        while(moveResult == result_t::HIT_CONTINUE);
    }
}
