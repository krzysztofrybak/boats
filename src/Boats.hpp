#pragma once

#include <array>
#include <iostream>

/*
 *  _|_1_|_2_|_3_|...
 *  A|___|___|___|...
 *  B|___|___|___|...
 *  C|___|___|___|...
 * ...
 */
enum class fieldState_t : char{
    UNKNOWN = ' ',
    EMPTY = 'o',
    BOARD = 'x'
};

constexpr int SIZE = 10;

#define CHAR_TO_INDEX(ch) ch-65

#define FROM_USER_NUM_TO_INDEX(val) val-1

using coord_t = std::pair<char,int>;

class Board{
public:
    fieldState_t getVal(coord_t coord)
    {
        return playgroud[CHAR_TO_INDEX(coord.first)][FROM_USER_NUM_TO_INDEX(coord.second)];
    }

    void setVal(coord_t coord, fieldState_t value)
    {
        playgroud[CHAR_TO_INDEX(coord.first)][FROM_USER_NUM_TO_INDEX(coord.second)] = value;
    }

    friend std::ostream & operator<<(std::ostream &os, const Board& board);
private:
    std::array<std::array<fieldState_t,SIZE>, SIZE> playgroud;
};

class Gameplay
{
public:
    void populateUserBoard();
    void populateComputerBoard();
private:
    Board user_board;
    Board computer_board;
};
