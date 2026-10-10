#include "quorridor-RL/display.hpp"
#include "quorridor-RL/gameState.hpp"

// Header

void draw_player(Cell player_cell, Player player_type, Display display);


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Quorridor");

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    Display display;
    GameState game_state;
    bool pawn_selected = false;
    bool wall_selected = true;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        display.update(GetScreenWidth(), GetScreenHeight());

        Cell mouse_cell = display.pixel_to_cell(GetMouseX(), GetMouseY());

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            int margin = display.get_margin();
            int cell = display.get_cell_size();

            // Draw rectangles
            for (int row=0; row<BOARD_SIZE; row++) {
                for (int col=0; col<BOARD_SIZE; col++) {
                    int x_rectangle = margin + col*cell;
                    int y_rectangle = margin + row*cell;
                    DrawRectangleLines(x_rectangle, y_rectangle, (int)cell, (int)cell, BLACK);
                }
            }

            // Draw players
            draw_player(game_state.get_A_cell(), Player::A, display);
            draw_player(game_state.get_B_cell(), Player::B, display);

            Color ballColor = YELLOW;

            // Make move

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Select or unselect pawn
                bool is_clicked_cell_a_valid_pawn_move = false;
                if ((mouse_cell == game_state.get_A_cell() && game_state.get_current_turn() == Player::A) ||
                    (mouse_cell == game_state.get_B_cell() && game_state.get_current_turn() == Player::B)) {
                        pawn_selected = true;
                    } else {
                        for (auto valid_pawn_move : game_state.get_valid_pawn_moves()) {
                            if (mouse_cell == valid_pawn_move.target) {
                                is_clicked_cell_a_valid_pawn_move = true;
                                continue;
                            }
                        }
                        
                        if (!is_clicked_cell_a_valid_pawn_move) {
                            pawn_selected = false;
                        }
                    }
                
                // Actually move the pawn, if it selected
                if (pawn_selected) {
                    PawnMove target = { mouse_cell };
                    game_state.move_pawn(target);

                    // unselect pawn if a _valid_ move is made. It is now the other player's turn!
                    if (is_clicked_cell_a_valid_pawn_move) {
                        pawn_selected = false;
                    }
                }
            }

            if (pawn_selected) {
                for (auto pawn_move : game_state.get_valid_pawn_moves()) {
                    Cell pawn_move_cell = pawn_move.target;

                    DrawCircleV(display.center_of(pawn_move_cell), display.get_cell_size() * 0.075f, LIGHTGRAY);
                }
            }

            ballColor = DARKGRAY;

            // Draw mouse reference
            //Vector2 mousePos = GetWorldToScreen2D(GetMousePosition(), camera)
            DrawCircleV(GetMousePosition(), 4, ballColor);
            DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", mouse_cell.col, mouse_cell.row),
                Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void draw_player(Cell player_cell, Player player_type, Display display) {
    DrawCircleV(display.center_of(player_cell), display.get_cell_size() * 0.35f, player_type == Player::A ? RED : BLUE);
}
