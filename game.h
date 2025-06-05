#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

extern int points;
void create_window();
void draw_block(int, int, int);
void draw_board();
void create_tetromino();
bool can_create_tetromino();
void draw_tetromino();
void clear_tetromino();
void put_tetromino();
bool can_move_down();
bool can_move_right();
bool can_move_left();
bool can_rotate();
void move_right();
void move_left();
void rotate_tetromino();
void check_lines();
void infomation();
void game();

#endif
