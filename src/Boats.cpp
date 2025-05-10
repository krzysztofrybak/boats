#include "src/Boats.hpp"

#include <stdexcept>
#include <random>
#include <iterator>
#include <algorithm>

void Gameplay::populateUserBoard()
{
    char inputRow = 0;
    int inputColumn = 0;

    system("clear");
    std::cout << user_board << std::endl;
    std::cout << "Podaj pole...    (Nacisnij 'k' aby zakonczyc)" << std::endl;
    std::cin >> inputRow >> inputColumn;

    if (inputRow == 'k') return;
    if (user_board.getVal(user_coord_t(inputRow, inputColumn)) != fieldState_t::EMPTY) throw std::invalid_argument("Invalid field provided");
    user_board.setVal(user_coord_t(inputRow, inputColumn),fieldState_t::BOARD);
}

void Gameplay::populateComputerBoard()
{
    BoatGenerator allBoatsGen;
    std::vector<Boat> allBoats_4 = allBoatsGen.generate(4);
    std::vector<Boat> allBoats_3 = allBoatsGen.generate(3);
    std::vector<Boat> allBoats_2 = allBoatsGen.generate(2);
    std::vector<Boat> allBoats_1 = allBoatsGen.generate(1);

    std::vector<Boat> sampleBoats;
    std::sample(allBoats_4.begin(), allBoats_4.end(), std::back_inserter(sampleBoats), 1, std::mt19937 {std::random_device{}()});
    std::sample(allBoats_3.begin(), allBoats_3.end(), std::back_inserter(sampleBoats), 2, std::mt19937 {std::random_device{}()});
    std::sample(allBoats_2.begin(), allBoats_2.end(), std::back_inserter(sampleBoats), 3, std::mt19937 {std::random_device{}()});
    std::sample(allBoats_1.begin(), allBoats_1.end(), std::back_inserter(sampleBoats), 4, std::mt19937 {std::random_device{}()});

tu pisac dalej...
}

std::ostream & operator<<(std::ostream &os, const Board& board)
{
    char rowName = 'A';
    os << "_|_1_|_2_|_3_|_4_|_5_|_6_|_7_|_8_|_9_|_10|" << std::endl;
    for (std::array<fieldState_t,BOARD_SIZE> row : board.playgroud)
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

std::vector<Boat> BoatGenerator::generate(int boatSize)
{
    std::vector<Boat> possibleBoats;

    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int column = 0; column < BOARD_SIZE - boatSize + 1; column++)
        {
            // horizontal
            possibleBoats.push_back(Boat((row,column),(row,column+boatSize)));
            if (boatSize != 1)
            {
                // vertical
                possibleBoats.push_back(Boat((column,row),(column+boatSize,row)));
            }
        }
    }
    return possibleBoats;
}
