#include "quorridor-RL/gameState.hpp"

GameState::GameState() : A_remaining_walls_{NUM_WALLS}, B_remaining_walls_{NUM_WALLS}, current_turn_{Player::A} {
    board_state_.walls.fill(WallSlot::Empty);
    board_state_.A_position = { 0, (int)BOARD_SIZE/2 };
    board_state_.B_position = { BOARD_SIZE -1, (int)BOARD_SIZE/2 };
}

Cell index_to_cell(int index) {
    Cell cell;

    cell.row = (int)(index / BOARD_SIZE);
    cell.col = (int)(index % BOARD_SIZE);

    return cell;
}

int cell_to_index(const Cell& cell) {
    return cell.row * BOARD_SIZE + cell.col;
}

bool GameState::is_cell_valid(const Cell& cell) {
    bool positive = (cell.row >= 0 && cell.col >= 0);
    bool less_than_board_size = (cell.row < BOARD_SIZE && cell.col < BOARD_SIZE);

    return (positive && less_than_board_size);
}

bool GameState::is_player_at(const Cell& cell) {
    bool is_player_A_there = ((board_state_.A_position.row == cell.row) &&
                             (board_state_.A_position.col == cell.col));
         
    bool is_player_B_there = ((board_state_.B_position.row == cell.row) &&
                            (board_state_.B_position.col == cell.col));

    return is_player_A_there || is_player_B_there;
}

bool GameState::is_player_at(const Cell& cell, const Player& player) {
    Cell player_cell;
    if (player == Player::A) {
        player_cell = board_state_.A_position;
    } else {
        player_cell = board_state_.B_position;
    }

    return ((player_cell.row == cell.row) && (player_cell.col == cell.col));
}

void GameState::move_pawn(const PawnMove& request) {
    std::vector<Move> valid_moves = get_valid_moves();

    bool move_is_valid = false;
    for (auto valid_move : valid_moves) {

        if (!std::holds_alternative<PawnMove>(valid_move)) continue;

        PawnMove valid_pawn_move = std::get<PawnMove>(valid_move);

        if ((request.target.col == valid_pawn_move.target.col) && 
            (request.target.row == valid_pawn_move.target.row)) {
                move_is_valid = true;
            }
    }

    if (!move_is_valid) return;
    
    Cell selected = request.target;
    if (current_turn_ == Player::A) {
        board_state_.A_position = selected;
        current_turn_ = Player::B;
    } else {
        board_state_.B_position = selected;
        current_turn_ = Player::A;
    }
}

std::vector<Move> GameState::get_valid_moves() {
    Cell player_cell;

    if (current_turn_ == Player::A) {
        player_cell = board_state_.A_position;
    } else {
        player_cell = board_state_.B_position;
    }

    std::vector<Move> valid_pawn_moves;

    constexpr std::array<std::pair<int,int>, 4> directions{{ {-1,0}, {1,0}, {0,-1}, {0,1} }};
    for (auto [delta_row, delta_col] : directions) { 
        Cell target_cell;
        target_cell.row = player_cell.row + delta_row;
        target_cell.col = player_cell.col + delta_col;

        if (is_cell_valid(target_cell) && (!is_player_at(target_cell))) {
            valid_pawn_moves.push_back(PawnMove{ target_cell });
        }

        Cell jump_over_cell = target_cell;
        if ((is_player_at(target_cell, Player::B) && current_turn_ == Player::A) ||
            (is_player_at(target_cell, Player::A) && current_turn_ == Player::B)) {
            jump_over_cell.col += delta_col;
            jump_over_cell.row += delta_row;
            if (is_cell_valid(jump_over_cell)) {
                valid_pawn_moves.push_back(PawnMove { jump_over_cell });
            }
        }
     }

    return valid_pawn_moves;
}
