#include "src/Boats.hpp"
#include <stdexcept>

void Gameplay::populateUserBoard()
{
    char inputRow = 0;
    int inputColumn = 0;

    system("clear");
    std::cout << user_board << std::endl;
    std::cout << "Podaj pole...    (Nacisnij 'k' aby zakonczyc)" << std::endl;
    std::cin >> inputRow >> inputColumn;

    if (inputRow == 'k') return;
    if (user_board.getVal(coord_t(inputRow, inputColumn)) != fieldState_t::EMPTY) throw std::invalid_argument("Invalid field provided");
    user_board.setVal(coord_t(inputRow, inputColumn),fieldState_t::BOARD);
}

void Gameplay::populateComputerBoard()
{

}

std::ostream & operator<<(std::ostream &os, const Board& board)
{
    char rowName = 'A';
    os << "_|_1_|_2_|_3_|_4_|_5_|_6_|_7_|_8_|_9_|_10|" << std::endl;
    for (std::array<fieldState_t,SIZE> row : board.playgroud)
    {
        os << rowName++ << "|";
        for (fieldState_t field : row)
        {
            os << "_" << static_cast<char>(field) << "_|";
        }
        os << std::endl;
    }
    return os;
}
