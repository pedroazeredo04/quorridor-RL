#ifndef _DISPLAY_HPP_
#define _DISPLAY_HPP_

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"
#include "gameState.hpp"

constexpr int screenWidth = 900;
constexpr int screenHeight = 900;

class Display {
    public:
        Display();
        Cell pixel_to_cell(int x_mouse, int y_mouse);
        Vector2 center_of(Cell c);

        void update(int screen_width, int screen_height);

        int get_margin();
        int get_cell_size();

    private:
        int board_size_px_;
        float margin_px_;
        int cell_size_px_;

        float margin_percentage_{0.05f};  // 5% border
};

#endif  // _DISPLAY_HPP_