#ifndef _GAME_STATE_HPP_
#define _GAME_STATE_HPP_

#include <array>
#include <iostream>

constexpr int BOARD_SIZE = 9;
constexpr int NUM_WALLS = 10;

enum class Player {
    A,
    B,
};

struct Cell {
    int row;
    int col;
};


struct BoardState
{
    // Store walls on INTERSECTIONS
    std::array<bool, (BOARD_SIZE-1)*(BOARD_SIZE-1)> horizontal_walls;
    std::array<bool, (BOARD_SIZE-1)*(BOARD_SIZE-1)> vertical_walls;
    Cell A_position;
    Cell B_position;
};

class GameState 
{
    public:
        GameState();

        Cell index_to_cell(int index);
        int cell_to_index(Cell cell);
        bool is_cellition_valid(Cell cell);

    private:
        BoardState board_state_;
        int A_remaining_walls_;
        int B_remaining_walls_;
        Player current_turn_;
};

#endif  // _GAME_STATE_HPP_
