#include "src/Boats.hpp"

#include <stdexcept>
#include <random>
#include <iterator>
#include <algorithm>

#include <stdlib.h>
#include <time.h>

int Boat::size()
{
    int size = abs(begin_coord.first - end_coord.first) + abs(begin_coord.second - end_coord.second);
    size++;
    if ( size < 1 || size > 4 ) throw std::invalid_argument("Invalid size");
    return size;
}

direction_t Boat::direction()
{
    return ( begin_coord.first == end_coord.first ? direction_t::HORIZONTAL : direction_t::VERTICAL);
}

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

    user_board.setVal(user_coord_t(inputRow, inputColumn),fieldState_t::BOAT);
}

bool isCoordinateConflict(index_coord_t coordA, index_coord_t coordB)
{
    return (abs(coordA.first - coordB.first) <= 1 ) || (abs(coordA.second - coordB.second) <= 1 );
}

bool isBoatConflict(Boat boatA, Boat boatB)
{
    return (isCoordinateConflict(boatA.begin_coord,boatB.begin_coord) ||
            isCoordinateConflict(boatA.begin_coord,boatB.end_coord) ||
            isCoordinateConflict(boatA.end_coord,boatB.begin_coord) ||
            isCoordinateConflict(boatA.end_coord,boatB.end_coord));
}

void Gameplay::populateComputerBoard()
{
    BoatGenerator allBoatsGen;
    std::vector<Boat> allBoats_4 = allBoatsGen.generate(4);
    std::vector<Boat> allBoats_3 = allBoatsGen.generate(3);
    std::vector<Boat> allBoats_2 = allBoatsGen.generate(2);
    std::vector<Boat> allBoats_1 = allBoatsGen.generate(1);

    std::vector<Boat> randomBoatLayout;

    while(true)
    {
        randomBoatLayout.clear();

        std::sample(allBoats_4.begin(), allBoats_4.end(), std::back_inserter(randomBoatLayout), 1, std::mt19937 {std::random_device{}()});
        std::sample(allBoats_3.begin(), allBoats_3.end(), std::back_inserter(randomBoatLayout), 2, std::mt19937 {std::random_device{}()});
        std::sample(allBoats_2.begin(), allBoats_2.end(), std::back_inserter(randomBoatLayout), 3, std::mt19937 {std::random_device{}()});
        std::sample(allBoats_1.begin(), allBoats_1.end(), std::back_inserter(randomBoatLayout), 4, std::mt19937 {std::random_device{}()});

        for(int boatIndex = 0; boatIndex < randomBoatLayout.size()-1; boatIndex++)
        {
            if(isBoatConflict(randomBoatLayout.at(boatIndex), randomBoatLayout.at(boatIndex+1))) continue;
        }
    }

    for (auto& boat : randomBoatLayout)
    {
        computer_board.setVal(index_coord_t(boat.begin_coord.first,boat.begin_coord.second),fieldState_t::BOAT);
        if(boat.size() > 1)
        {
            computer_board.setVal(index_coord_t(boat.end_coord.first,boat.end_coord.second),fieldState_t::BOAT);

            if(boat.size() > 2)
            {
                if (boat.direction() == direction_t::HORIZONTAL)
                {
                    computer_board.setVal(index_coord_t(boat.begin_coord.first,std::min(boat.begin_coord.second, boat.end_coord.second)+1),fieldState_t::BOAT);
                    if(boat.size() == 4)
                    {
                        computer_board.setVal(index_coord_t(boat.begin_coord.first,std::min(boat.begin_coord.second, boat.end_coord.second)+2),fieldState_t::BOAT);
                    }
                }
                else if (boat.direction() == direction_t::VERTICAL)
                {
                    computer_board.setVal(index_coord_t(std::min(boat.begin_coord.first, boat.end_coord.first)+1,boat.begin_coord.second),fieldState_t::BOAT);
                    if(boat.size() == 4)
                    {
                        computer_board.setVal(index_coord_t(std::min(boat.begin_coord.first, boat.end_coord.first)+2,boat.begin_coord.second),fieldState_t::BOAT);
                    }
                }
            }
        }
    }
}

result_t Gameplay::userMove()
{
    char inputRow = 0;
    int inputColumn = 0;
    static int totalUserHits = 0;

    std::cout << "Podaj pole do trafienia..." << std::endl;
    std::cin >> inputRow >> inputColumn;

    // pudlo
    if (computer_board.getVal(user_coord_t(inputRow, inputColumn)) == fieldState_t::EMPTY)
    {
        user_hit_board.setVal(user_coord_t(inputRow, inputColumn),fieldState_t::EMPTY);
        return result_t::MISHIT;
    }

    // trafiony
    user_hit_board.setVal(user_coord_t(inputRow, inputColumn),fieldState_t::BOAT);
    if(++totalUserHits == ALL_HITS) return result_t::ENDGAME;

    // plynie dalej - sprawdz sasiednie
    if (
        // nad
        (FROM_USER_CHAR_TO_INDEX(inputRow) > 0 &&
         computer_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow)-1, FROM_USER_NUM_TO_INDEX(inputColumn))) == fieldState_t::BOAT &&
         user_hit_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow)-1, FROM_USER_NUM_TO_INDEX(inputColumn))) == fieldState_t::UNKNOWN
        ) ||
        // pod
        (FROM_USER_CHAR_TO_INDEX(inputRow) < BOARD_SIZE-1 &&
         computer_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow)+1, FROM_USER_NUM_TO_INDEX(inputColumn))) == fieldState_t::BOAT &&
         user_hit_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow)+1, FROM_USER_NUM_TO_INDEX(inputColumn))) == fieldState_t::UNKNOWN
        ) ||
        // z lewej
        (FROM_USER_NUM_TO_INDEX(inputColumn) > 0 &&
         computer_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow), FROM_USER_NUM_TO_INDEX(inputColumn)-1)) == fieldState_t::BOAT &&
         user_hit_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow), FROM_USER_NUM_TO_INDEX(inputColumn)-1)) == fieldState_t::UNKNOWN
        ) ||
        // z prawej
        (FROM_USER_NUM_TO_INDEX(inputColumn) < BOARD_SIZE-1 &&
         computer_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow), FROM_USER_NUM_TO_INDEX(inputColumn)+1)) == fieldState_t::BOAT &&
         user_hit_board.getVal(index_coord_t(FROM_USER_CHAR_TO_INDEX(inputRow), FROM_USER_NUM_TO_INDEX(inputColumn)+1)) == fieldState_t::UNKNOWN
        )
        )
    {
        return result_t::HIT_CONTINUE;
    }

    return result_t::HIT_DONE;
}

result_t Gameplay::computerMove()
{
    srand (time(NULL));

    int indexRow = 0;
    int indexColumn = 0;

    static int totalComputerHits = 0;

    do
    {
        indexRow = rand() % 10;
        indexColumn = rand() % 10;
    }
    while (computer_hit_board.getVal(index_coord_t(indexRow, indexColumn)) == fieldState_t::UNKNOWN);


    // pudlo
    if (user_board.getVal(index_coord_t(indexRow, indexColumn)) == fieldState_t::EMPTY)
    {
        computer_hit_board.setVal(index_coord_t(indexRow, indexColumn),fieldState_t::EMPTY);
        return result_t::MISHIT;
    }

    // trafiony
    computer_hit_board.setVal(index_coord_t(indexRow, indexColumn),fieldState_t::BOAT);
    if(++totalComputerHits == ALL_HITS) return result_t::ENDGAME;

    // plynie dalej - sprawdz sasiednie
    if (
        // nad
        (indexRow > 0 &&
         user_board.getVal(index_coord_t(indexRow-1, indexColumn)) == fieldState_t::BOAT &&
         computer_hit_board.getVal(index_coord_t(indexRow-1, indexColumn)) == fieldState_t::UNKNOWN
        ) ||
        // pod
        (indexRow < BOARD_SIZE-1 &&
         user_board.getVal(index_coord_t(indexRow+1, indexColumn)) == fieldState_t::BOAT &&
         computer_hit_board.getVal(index_coord_t(indexRow+1, indexColumn)) == fieldState_t::UNKNOWN
        ) ||
        // z lewej
        (indexColumn > 0 &&
         user_board.getVal(index_coord_t(indexRow, indexColumn-1)) == fieldState_t::BOAT &&
         computer_hit_board.getVal(index_coord_t(indexRow, indexColumn-1)) == fieldState_t::UNKNOWN
        ) ||
        // z prawej
        (indexColumn < BOARD_SIZE-1 &&
         user_board.getVal(index_coord_t(indexRow, indexColumn+1)) == fieldState_t::BOAT &&
         computer_hit_board.getVal(index_coord_t(indexRow, indexColumn+1)) == fieldState_t::UNKNOWN
        )
        )
    {
        return result_t::HIT_CONTINUE;
    }

    return result_t::HIT_DONE;
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
