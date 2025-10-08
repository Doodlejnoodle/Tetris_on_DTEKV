#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef struct {
    int x, y;
    int id;
    int rot;
} Piece;

extern Piece current_piece;
extern int score;

void game_init(void);
void game_spawn_piece(void);
int  game_check_collision(int nx, int ny, int id, int rot);
void game_lock_piece(void);
void game_rotate(void);
void game_move_left(void);
void game_move_right(void);
void game_soft_drop(void);
void game_step_gravity(void);
void game_update(void);
void game_draw(void (*draw_cell)(int,int,uint8_t));

#endif
