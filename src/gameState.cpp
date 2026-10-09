#include "quorridor-RL/gameState.hpp"

GameState::GameState() : A_remaining_walls_{NUM_WALLS}, B_remaining_walls_{NUM_WALLS}, current_turn_{Player::A} {
    board_state_.horizontal_walls.fill(false);
    board_state_.vertical_walls.fill(false);
}

Cell index_to_cell(int index) {
    Cell cell;

    cell.row = (int)(index / BOARD_SIZE);
    cell.col = (int)(index % BOARD_SIZE);

    return cell;
}

int cell_to_index(Cell cell) {
    return cell.row * BOARD_SIZE + cell.col;
}

bool GameState::is_cellition_valid(Cell cell) {
    return !(cell.row > BOARD_SIZE || cell.col > BOARD_SIZE);
}

