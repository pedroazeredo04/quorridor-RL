#include "quorridor-RL/display.hpp"

Display::Display() { }

Cell Display::pixel_to_cell(int x_mouse, int y_mouse) {
    int mouse_col = (int) ((x_mouse - margin_px_) / cell_size_px_);
    int mouse_row = (int) ((board_size_px_ - y_mouse - margin_px_) / cell_size_px_);

    if (mouse_col < 0) mouse_col = 0;
    if (mouse_row < 0) mouse_row = 0;
    if (mouse_col > (BOARD_SIZE - 1)) mouse_col = (BOARD_SIZE - 1);
    if (mouse_row > (BOARD_SIZE - 1)) mouse_row = (BOARD_SIZE - 1);

    Cell board;
    board.col = mouse_col;
    board.row = mouse_row;

    return board;
}

void Display::update(int screen_width, int screen_height) {
    this->board_size_px_ = std::min(GetScreenWidth(), GetScreenHeight());
    this->margin_px_ = (int) board_size_px_ * margin_percentage_;              
    this->cell_size_px_ = (int) (board_size_px_ - 2 * margin_px_) / BOARD_SIZE;
}

int Display::get_margin() {
    return this->margin_px_;
}

int Display::get_cell_size() {
    return this->cell_size_px_;
}