#ifndef MENU_H
#define MENU_H

#include "graphics.h"

extern int speeds;
extern std::string name;
enum button_values { NONE = -1, GAME, ABOUT, SETTINGS, RECORDS,
                        EXIT, N_BUTTONS };

struct Button
{
   int left;
   int top;
   int width;
   int height;
   IMAGE *image;
};

struct Group {
   std::string nickname;
   int score;
};

void load();
void start();
void menu();
void about();
void print_setting();
void settings();
void sort_groups();
void load_groups();
void print_groups();
void records();
void close();

void create_button(int, int, int, const char*);
void add_group(std::string, int);
int  select_button();

#endif

