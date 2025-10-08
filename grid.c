#include <stdint.h>
#include "grid.h"

uint8_t grid[GRID_H][GRID_W];

void grid_clear(void) {
    for (int y=0; y<GRID_H; ++y)
        for (int x=0; x<GRID_W; ++x)
            grid[y][x] = 0;
}

int grid_is_occupied(int x, int y) {
    if (x<0 || x>=GRID_W || y>=GRID_H) return 1;
    if (y<0) return 0;
    return grid[y][x] != 0;
}

void grid_set(int x, int y, uint8_t val) {
    if (x>=0 && x<GRID_W && y>=0 && y<GRID_H)
        grid[y][x] = val;
}

int grid_check_full_line(int y) {
    for (int x=0;x<GRID_W;x++)
        if (!grid[y][x]) return 0;
    return 1;
}

void grid_remove_line(int y) {
    for (int x=0;x<GRID_W;x++)
        grid[y][x] = 0;
}

void grid_shift_down(int y) {
    for (int yy=y; yy>0; yy--)
        for (int x=0;x<GRID_W;x++)
            grid[yy][x] = grid[yy-1][x];
    for (int x=0;x<GRID_W;x++)
        grid[0][x] = 0;
}

int grid_clear_full_lines(void) {
    int cleared = 0;
    for (int y=GRID_H-1;y>=0;y--) {
        if (grid_check_full_line(y)) {
            grid_remove_line(y);
            grid_shift_down(y);
            cleared++;
            y++; // recheck same row after shift
        }
    }
    return cleared;
}
