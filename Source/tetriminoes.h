#ifndef TETRIMINOES_H
#define TETRIMINOES_H

#include <stdint.h>

#define NUM_TETROMINOES 7

// 4 rotations × 4 rows each → 16 bytes per piece
extern const uint8_t tetromino_shapes[NUM_TETROMINOES][4][4];

static inline int piece_cell(int id, int rot, int row, int col) {
    return (tetromino_shapes[id][rot][row] >> (3 - col)) & 1;
}

#endif
