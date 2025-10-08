#ifndef GRID_H
#define GRID_H

#include <stdint.h>

#define GRID_W 10
#define GRID_H 20

extern uint8_t grid[GRID_H][GRID_W];

void grid_clear(void);
int  grid_is_occupied(int x, int y);
void grid_set(int x, int y, uint8_t val);
void grid_remove_line(int y);
void grid_shift_down(int y);
int  grid_check_full_line(int y);
int  grid_clear_full_lines(void);

#endif
