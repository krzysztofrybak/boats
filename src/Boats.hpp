#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <fstream>

/*
 *  _|_1_|_2_|_3_|...
 *  A|___|___|___|...
 *  B|___|___|___|...
 *  C|___|___|___|...
 * ...
 */
enum class fieldState_t : char{
    UNKNOWN = '_',
    EMPTY = '.',
    BOAT = 'X'
};

enum class direction_t : char{
    VERTICAL,
    HORIZONTAL
};

enum class result_t : char{
    HIT_CONTINUE,
    HIT_DONE,
    MISHIT,
    ENDGAME
};

constexpr int BOARD_SIZE = 10;

constexpr int ALL_HITS = 20;

#define FROM_USER_CHAR_TO_INDEX(ch) ch-65

#define FROM_USER_NUM_TO_INDEX(val) val-1

#define IS_VALID_FIELD(row,column) (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)

// *_coord_t.first() -> row A, B, C ... / 0, 1, 2...
// *_coord_t.second() -> column 1, 2, 3... / 0, 1, 2...
using user_coord_t = std::pair<char,int>;
using index_coord_t = std::pair<int,int>;

class Board
{
public:
    Board(bool isHitBoard)
    {
        for (auto& row: playgroud) row.fill(isHitBoard ? fieldState_t::UNKNOWN : fieldState_t::EMPTY);
    }
    fieldState_t getVal(index_coord_t coord)
    {
        return playgroud.at(coord.first).at(coord.second);
    }
    fieldState_t getVal(user_coord_t coord)
    {
        return playgroud.at(FROM_USER_CHAR_TO_INDEX(coord.first)).at(FROM_USER_NUM_TO_INDEX(coord.second));
    }

    void setVal(index_coord_t coord, fieldState_t value)
    {
        playgroud.at(coord.first).at(coord.second) = value;
    }

    void setVal(user_coord_t coord, fieldState_t value)
    {
        playgroud.at(FROM_USER_CHAR_TO_INDEX(coord.first)).at(FROM_USER_NUM_TO_INDEX(coord.second)) = value;
    }

    friend std::ostream & operator<<(std::ostream &os, const Board& board);
private:
    std::array<std::array<fieldState_t,BOARD_SIZE>, BOARD_SIZE> playgroud;
};

class Gameplay
{
public:
    Gameplay(std::ofstream &log): logFile(log) {}
    void populateUserBoard();
    void populateComputerBoard();
    result_t userMove();
    result_t computerMove();
public:
    Board user_board{false};
    Board computer_board{false};
    Board user_hit_board{true};
    Board computer_hit_board{true};

    int userHits = 0;
    int computerHits = 0;
    std::ofstream &logFile;
};

class Boat
{
public:
    Boat(index_coord_t argBeginCoord,
         index_coord_t argEndCoord):
        begin_coord(argBeginCoord), end_coord(argEndCoord)
    {
        if(!IS_VALID_FIELD(argBeginCoord.first, argBeginCoord.second) || !IS_VALID_FIELD(argEndCoord.first, argEndCoord.second))
        {
            throw std::invalid_argument("Invalid field provided");
        }
    }
    Boat() = delete;

    int size();
    direction_t direction();

    index_coord_t begin_coord;
    index_coord_t end_coord;

    friend std::ostream & operator<<(std::ostream &os, const Boat& boat);
};

class BoatGenerator
{
public:
    std::vector<Boat> generate(int boatSize);
};
