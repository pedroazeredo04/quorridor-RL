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
    bool is_player_A_there = (board_state_.A_position == cell);
         
    bool is_player_B_there = (board_state_.B_position == cell);

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

        if (request == valid_pawn_move) {
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

std::vector<PawnMove> GameState::get_valid_pawn_moves() {
    Cell player_cell;

    if (current_turn_ == Player::A) {
        player_cell = board_state_.A_position;
    } else {
        player_cell = board_state_.B_position;
    }

    std::vector<PawnMove> valid_pawn_moves;

    constexpr std::array<std::pair<int,int>, 4> directions{{ {-1,0}, {1,0}, {0,-1}, {0,1} }};
    for (auto [delta_row, delta_col] : directions) { 
        Cell target_cell;
        target_cell.row = player_cell.row + delta_row;
        target_cell.col = player_cell.col + delta_col;

        if (is_cell_valid(target_cell) && (!is_player_at(target_cell))) {
            valid_pawn_moves.push_back(PawnMove{ target_cell });
        }

        Cell jump_over_cell = target_cell;
        if (is_player_at(target_cell)) {
            jump_over_cell.col += delta_col;
            jump_over_cell.row += delta_row;
            if (is_cell_valid(jump_over_cell)) {
                valid_pawn_moves.push_back(PawnMove { jump_over_cell });
            }
        }
     }

    return valid_pawn_moves;
}

std::vector<WallMove> GameState::get_valid_wall_moves() {
    std::vector<WallMove> valid_wall_moves;

    if ((current_turn_ == Player::A && A_remaining_walls_ == 0) ||
        (current_turn_ == Player::B && B_remaining_walls_ == 0)) {
        return valid_wall_moves;
    }

    // All moves are legal, except the ones that aren't.
    // Start with horizontal walls:
    for (int i=0; i < board_state_.walls.size(); i++) {
        int current_intersection_row = (int) (i / (BOARD_SIZE-1));
        int current_intersection_col = (int) (i % (BOARD_SIZE-1));
        WallSlot current_wall = board_state_.walls[i];

        if (current_wall == WallSlot::Empty) {
            Intersection valid_intersection = {current_intersection_row, current_intersection_col};
            WallMove valid_wall_move_h = {valid_intersection, Orientation::Horizontal};
            WallMove valid_wall_move_v = {valid_intersection, Orientation::Vertical};
            valid_wall_moves.push_back(valid_wall_move_h);
            valid_wall_moves.push_back(valid_wall_move_v);
        }
    }

    return valid_wall_moves;
}

std::vector<Move> GameState::get_valid_moves() {
    std::vector<PawnMove> pawn_moves = get_valid_pawn_moves();
    std::vector<WallMove> wall_moves = get_valid_wall_moves();

    std::vector<Move> valid_moves;
    valid_moves.reserve(pawn_moves.size() + wall_moves.size());
    valid_moves.insert(valid_moves.end(), pawn_moves.begin(), pawn_moves.end());
    valid_moves.insert(valid_moves.end(), wall_moves.begin(), wall_moves.end());
    return valid_moves;
}
