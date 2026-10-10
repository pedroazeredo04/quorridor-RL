#ifndef _GAME_STATE_HPP_
#define _GAME_STATE_HPP_

#include <array>
#include <iostream>
#include <variant>
#include <vector>

constexpr int BOARD_SIZE = 9;
constexpr int NUM_WALLS = 10;

enum class Player {
    A,
    B,
};

enum class WallSlot {
    Empty,
    Vertical,
    Horizontal,
};

enum class Orientation {
    Vertical,
    Horizontal,
};

struct Cell {
    int row;
    int col;
};

struct Intersection { 
    int row;
    int col;
};

struct BoardState
{
    // Store walls on INTERSECTIONS
    std::array<WallSlot, (BOARD_SIZE-1)*(BOARD_SIZE-1)> walls;
    Cell A_position;
    Cell B_position;
};

struct PawnMove { Cell target; };
struct WallMove { Intersection at; Orientation orientation; };
using Move = std::variant<PawnMove, WallMove>;  // Claude suggestion, very elegant

class GameState 
{
    public:
        GameState();

        Cell index_to_cell(int index);
        int cell_to_index(const Cell& cell);
        bool is_cell_valid(const Cell& cell);
        bool is_player_at(const Cell& cell);
        bool is_player_at(const Cell& cell, const Player& player);
        
        void move_pawn(const PawnMove& request);
        std::vector<Move> get_valid_moves();

        // Getters
        Cell get_A_cell() { return board_state_.A_position; };
        Cell get_B_cell() { return board_state_.B_position ; }

    private:
        BoardState board_state_;
        int A_remaining_walls_;
        int B_remaining_walls_;
        Player current_turn_;
        Player winner_;
};

#endif  // _GAME_STATE_HPP_
