#include "game.h"
#include "grid.h"
#include "tetriminoes.h"
#include "vga.h"

Piece current_piece;
int score = 0;

void game_init(void) {
    grid_clear();
    score = 0;
    game_spawn_piece();
}

int game_check_collision(int nx, int ny, int id, int rot) {
    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++) {
            if (!piece_cell(id,rot,r,c)) continue;
            int px = nx + c;
            int py = ny + r;
            if (px < 0 || px >= GRID_W || py >= GRID_H) return 1;
            if (py >= 0 && grid_is_occupied(px,py)) return 1;
        }
    return 0;
}

void game_spawn_piece(void) {
    static uint8_t next_id = 0;
    current_piece.id  = next_id;
    next_id = (next_id + 3) % 7; // simple pseudo-random pattern
    current_piece.rot = 0;
    current_piece.x = GRID_W/2 - 2;
    current_piece.y = -2;

    if (game_check_collision(current_piece.x, current_piece.y,
                             current_piece.id, current_piece.rot)) {
        grid_clear();
        score = 0;
    }
}

void game_lock_piece(void) {
    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++) {
            if (piece_cell(current_piece.id, current_piece.rot, r, c)) {
                int px = current_piece.x + c;
                int py = current_piece.y + r;
                grid_set(px, py, current_piece.id+1);
            }
        }
    int lines = grid_clear_full_lines();
    if (lines == 1) score += 40;
    else if (lines == 2) score += 100;
    else if (lines == 3) score += 300;
    else if (lines == 4) score += 1200;
    game_spawn_piece();
}

void game_rotate(void) {
    int new_rot = (current_piece.rot + 1) & 3;
    if (!game_check_collision(current_piece.x, current_piece.y,
                              current_piece.id, new_rot))
        current_piece.rot = new_rot;
}

void game_move_left(void) {
    if (!game_check_collision(current_piece.x - 1, current_piece.y,
                              current_piece.id, current_piece.rot))
        current_piece.x--;
}

void game_move_right(void) {
    if (!game_check_collision(current_piece.x + 1, current_piece.y,
                              current_piece.id, current_piece.rot))
        current_piece.x++;
}

void game_soft_drop(void) {
    if (!game_check_collision(current_piece.x, current_piece.y + 1,
                              current_piece.id, current_piece.rot))
        current_piece.y++;
    else {
        game_lock_piece();
    }
}

void game_step_gravity(void) {
    if (!game_check_collision(current_piece.x, current_piece.y + 1,
                              current_piece.id, current_piece.rot))
        current_piece.y++;
    else
        game_lock_piece();
}

void game_update(void) {
    // Placeholder if you later add timing or input reading
    game_step_gravity();
}

void game_draw(void (*draw_cell)(int,int,uint8_t)) {
    for (int y=0;y<GRID_H;y++)
        for (int x=0;x<GRID_W;x++)
            draw_cell(x, y, grid[y][x]);

    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++)
            if (piece_cell(current_piece.id,current_piece.rot,r,c)) {
                int px = current_piece.x + c;
                int py = current_piece.y + r;
                if (py >= 0) draw_cell(px, py, current_piece.id+1);
            }
}
