#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include "graphics.h"
#include "game.h"
#include "menu.h"

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BLOCK_SIZE = 30;

const int BLOCK_COLORS[] = {BLACK, BLUE, GREEN, CYAN, RED,
                                    MAGENTA, BROWN, LIGHTGRAY};

const int tetrominos[7][4][4] = {
       {
           {0, 0, 0, 0},
           {1, 1, 1, 1},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
       {
           {0, 0, 0, 0},
           {0, 2, 2, 0},
           {0, 2, 2, 0},
           {0, 0, 0, 0}
       },
       {
           {0, 3, 0, 0},
           {3, 3, 3, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
       {
           {0, 4, 4, 0},
           {4, 4, 0, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
       {
           {5, 5, 0, 0},
           {0, 5, 5, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
       {
           {6, 6, 6, 0},
           {6, 0, 0, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
       {
           {7, 7, 7, 0},
           {0, 0, 7, 0},
           {0, 0, 0, 0},
           {0, 0, 0, 0}
       },
   };

int board[BOARD_HEIGHT][BOARD_WIDTH];
bool exit_game = false;
int tetromino[4][4];
int tetromino_index;
int next_index;
int x = 3;
int y = 0;
int lines = 0;
int points = 0;


void create_window()
{
   setfillstyle(SOLID_FILL, COLOR(36, 38, 36)); 
   bar(BOARD_WIDTH * BLOCK_SIZE , 0,
      (BOARD_WIDTH + 10) * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE);

   setfillstyle(SOLID_FILL, BLACK); 
   bar(BOARD_WIDTH * BLOCK_SIZE + 15, 15, 
         BOARD_WIDTH * BLOCK_SIZE + 195, 135); 
   
   setcolor(WHITE);
   rectangle(BOARD_WIDTH * BLOCK_SIZE + 15, 15,
                 BOARD_WIDTH * BLOCK_SIZE + 195, 135); 

   setfillstyle(SOLID_FILL, BLACK);
   setbkcolor(BLACK);
   bar(BOARD_WIDTH * BLOCK_SIZE + 15, 175,
         BOARD_WIDTH * BLOCK_SIZE + 195, 255);
   
   rectangle(BOARD_WIDTH * BLOCK_SIZE + 15, 175, 
                 BOARD_WIDTH * BLOCK_SIZE + 195, 255);
   
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   setcolor(WHITE);
   outtextxy(BOARD_WIDTH * BLOCK_SIZE + 50, 190, "Points");

   setfillstyle(SOLID_FILL, BLACK);
   setbkcolor(BLACK);
    bar(BOARD_WIDTH * BLOCK_SIZE + 15, 290, 
          BOARD_WIDTH * BLOCK_SIZE + 195, 370);
         
   rectangle(BOARD_WIDTH * BLOCK_SIZE + 15, 290, 
                 BOARD_WIDTH * BLOCK_SIZE + 195, 370);
                  
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   setcolor(WHITE);
   outtextxy(BOARD_WIDTH * BLOCK_SIZE + 50, 305, "Lines");

   setfillstyle(SOLID_FILL, BLACK);
   setbkcolor(BLACK);
   bar(BOARD_WIDTH * BLOCK_SIZE + 15, 405, 
         BOARD_WIDTH * BLOCK_SIZE + 195, 485);
          
   rectangle(BOARD_WIDTH * BLOCK_SIZE + 15, 405,
                  BOARD_WIDTH * BLOCK_SIZE + 195, 485);
                   
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   setcolor(WHITE);
   outtextxy(BOARD_WIDTH * BLOCK_SIZE + 50, 420, "Speed");
}

void draw_block(int i, int j, int color)
{
   setfillstyle(SOLID_FILL, color);
   bar(i * BLOCK_SIZE, j * BLOCK_SIZE, 
      (i + 1) * BLOCK_SIZE, (j + 1) * BLOCK_SIZE);

   if (color != 0){
      rectangle((i + 0.05) * BLOCK_SIZE, (j + 0.05) * BLOCK_SIZE,
      (i + 0.95) * BLOCK_SIZE, (j + 0.95) * BLOCK_SIZE);
      
   }
}

void draw_board()
{
   for (int j = 0; j < BOARD_HEIGHT; j++) {
      for (int i = 0; i < BOARD_WIDTH; i++) {
         draw_block(i, j, BLOCK_COLORS[board[j][i]]);
      }
   }
}

void create_tetromino()
{
   srand(time(NULL));
   tetromino_index = next_index;
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         tetromino[i][j] = tetrominos[tetromino_index][i][j];
      }
   }
   
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         if (tetromino[i][j] != 0)
         {
            draw_block(12 + j,1 + i, BLACK);
         }
      }
   }
   
   next_index =  rand() % 7;
   
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         if (tetrominos[next_index][i][j] != 0)
            {
               draw_block(j + 12, i + 1, 
                  BLOCK_COLORS[tetrominos[next_index][i][j]]);
            }
      }
   }
}

bool can_create_tetromino()
{
   for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[i][j] != 0 && board[y + i][x + j] != 0)
            {
               return false;
            }
        }
    }
    return true;
}

void draw_tetromino()
{
   for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[i][j] != 0)
            {
               draw_block(x + j,y + i, 
                  BLOCK_COLORS[tetromino[i][j]]);
            }
        }
    }
}

void clear_tetromino()
{
   for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[i][j] != 0)
            {
               draw_block(x + j,y + i, BLACK);
            }
        }
    }
}

void put_tetromino()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tetromino[i][j] != 0)
            {
               board[y + i][x + j] = tetromino[i][j];
            }
        }
    }
    
    draw_board();
}

bool can_move_down()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino[i][j] != 0) {
                if (y + i >= BOARD_HEIGHT - 1) {
                    return false;
                }
                else if (board[y + i + 1][x + j] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool can_move_right()
{
   for (int i = 0; i < 4; i++) 
   {
      for (int j = 0; j < 4; j++)
      {
         if (tetromino[i][j] != 0)
         {
            if (x + j >= BOARD_WIDTH - 1 || 
               board[y + i][x + j + 1] != 0) 
            {
               return false;
            }
         }
      }
   }
   return true;
}

bool can_move_left()
{
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++) 
      {
         if (tetromino[i][j] != 0) 
         {
            if (x + j <= 0 || board[y + i][x + j - 1] != 0) 
            {
               return false;
            }
         }
      }
   }
    return true;
}

bool can_rotate() 
{
    for (int i = 0; i < 4; i++)
   {
        for (int j = 0; j < 4; j++) 
       {
            if (tetromino[i][j] != 0) 
            {
               if (x + j > BOARD_WIDTH-1 || 
                  board[y + i][x + j] != 0 || x + j < 0) 
               {
                    return false;
               }
            }
        }
    }
    return true;
}

void move_right()
{
   if (can_move_right())
   {
      clear_tetromino();
      x++;
      draw_tetromino();
   }
}

void move_left()
{
   if (can_move_left())
   {
      clear_tetromino();
      x--;
      draw_tetromino();
   }
}

void rotate_tetromino() 
{
   int temp_tetromino[4][4];
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++) 
      {
         temp_tetromino[i][j] = tetromino[i][j];
      }
   }
   
   if (tetromino_index != 0 && tetromino_index != 1)
   {
      for (int i = 0; i < 3; i++)
      {
         for (int j = 0; j < 3; j++)
         {
            tetromino[i][j] = temp_tetromino[2-j][i];
         }
      }
   }
   else
   {
      for (int i = 0; i < 4; i++)
      {
         for (int j = 0; j < 4; j++)
         {
            tetromino[i][j] = temp_tetromino[3-j][i];
         }
      }
   }
    
    if (!can_rotate()) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tetromino[i][j] = temp_tetromino[i][j];
            }
        }
    }
}

void check_lines() 
{
   int c = 0;
   for (int i = BOARD_HEIGHT - 1; i >= 0; i--) 
   {
      bool full_line = true;
      for (int j = 0; j < BOARD_WIDTH; j++)
      {
         if (board[i][j] == 0)
         {
            full_line = false;
            if (c != 0)
            {
               if (c == 1)
                  points += 100;
               else if (c == 2)
                  points += 300;
               else if (c == 3)
                  points += 500;
               else if (c == 4)
                  points += 1000;
               lines += c;
               c = 0;
            }
            break;
         }
      }
        
      if (full_line) 
      {
         for (int k = i; k > 0; k--) 
         {
            for (int j = 0; j < BOARD_WIDTH; j++)
            {
               board[k][j] = board[k - 1][j];
            }
         }
         for (int j = 0; j < BOARD_WIDTH; j++)
         {
            board[0][j] = 0;
         }
         i++;
         c++;
      }
   }
}

void infomation()
{
   stringstream point;
   point << points;
   string numStr = point.str();
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy((BOARD_WIDTH + 2.5) * BLOCK_SIZE , 
                  7 * BLOCK_SIZE, numStr.c_str());
      
   stringstream line;
   line << lines;
   string lineStr = line.str();
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy((BOARD_WIDTH + 2.5) * BLOCK_SIZE , 
                  11 * BLOCK_SIZE, lineStr.c_str());
   
   stringstream speed;
   speed << speeds;
   string speedStr = speed.str();
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy((BOARD_WIDTH + 2.5) * BLOCK_SIZE , 
                  15 * BLOCK_SIZE, speedStr.c_str());
}

void clear_board()
{
   exit_game = false;
   x = 3;
   y = 0;
   lines = 0;
   points = 0;
   for (int i = 0; i < BOARD_WIDTH; i++)
   {
      for (int j = 0; j < BOARD_HEIGHT; j++)
      {
         board[j][i] = 0 ;
      }
   }
}

void move_down()
{
   if (can_move_down())
   {
      clear_tetromino();
      y++;
      draw_tetromino();
   }
   else
   {
      put_tetromino();
      check_lines();
      if (lines >= 10 && speeds > 300)
      {
         speeds = 300;
      }
      if (lines >= 20 && speeds > 200)
      {
         speeds = 200;
      }
      if (lines >= 50 && speeds > 100)
      {
         speeds = 100;
      }
      infomation();
      x = 3;
      y = 0;
      if (can_create_tetromino())
      {
         create_tetromino();
      }
      else
      {
         exit_game = true;
      }
   }
}

void game()
{
   clear_board();
   create_window();
   next_index = rand() % 7;
   infomation();
   create_tetromino();
   draw_tetromino();
   while (!exit_game)
   {
      draw_board();
      move_down();
      
      auto start = chrono::steady_clock::now();
      while (chrono::duration_cast<chrono::milliseconds>
         (chrono::steady_clock::now() - start).count() < speeds)
      {
         if (kbhit())
         {
            int key = getch();
            switch(key)
            {
               case KEY_RIGHT:
                  move_right();
                  break;
               case KEY_LEFT:
                  move_left();
                  break;
               case KEY_DOWN:
                  move_down();
                  break;
               case KEY_UP:
                  rotate_tetromino();
                  break;
               case KEY_ESC:
                  speeds = 500;
                  return;
            }
         }
      }
   }
   ofstream outfile("records.txt", ios::app);
   string data = name + " " + to_string(points);
   outfile << data << endl;
   outfile.close();
   add_group(name, points);
   clear_board();
   speeds = 500;
}