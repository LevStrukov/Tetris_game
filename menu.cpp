#include <string>
#include <fstream>
#include "menu.h"
#include "game.h"
#include "graphics.h"

using namespace std;

Button buttons[N_BUTTONS];
IMAGE *menu_image, *about_image, *start_image;
int speeds = 500;
string name = "noname";
const int MAX_GROUPS = 100;
int num_groups = 0;
Group groups[MAX_GROUPS];

void add_group(string name1, int points1) {
   Group g;
   g.nickname = name1;
   g.score = points1;
   groups[num_groups] = g;
   num_groups++;
}

void load_groups() {
    ifstream infile("records.txt");
    while (true) {
        Group g;
        if (!(infile >> g.nickname >> g.score)) {
            if (infile.eof()) {
                break;  
            }
        }
        groups[num_groups] = g;
        num_groups++;
    }

    infile.close();
}

void load()
{
   load_groups();
   menu_image  = loadBMP("images/menu.bmp");
   about_image = loadBMP("images/about.bmp");
   start_image = loadBMP("images/screensaver.bmp");
   
   create_button(GAME,  170, 100, "buttons/game.bmp");
   create_button(ABOUT, 170, 200, "buttons/about.bmp");
   create_button(SETTINGS, 170, 300, "buttons/settings.bmp");
   create_button(RECORDS, 170, 400, "buttons/records.bmp");
   create_button(EXIT,  170, 500, "buttons/exit.bmp");
}

void start()
{
   int r = 255;
   int g = 255;
   int b = 255;
   bool shift = true;
   
   while (true)
   {
      putimage(0, 0, start_image, COPY_PUT);
      setbkcolor(COLOR(238, 228, 203));
      
      if (shift)
      {
         r -= 15;
         g -= 15;
         b -= 15;
         if (r <= 0)
            shift = false;
      }
      else
      {
         r += 15;
         g += 15;
         b += 15;
         if (r >= 255)
            shift = true;
      }

      settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
      setcolor(COLOR(r,g,b));
      outtextxy(75, 550, "[Press any button!]");

      delay(50);
      
      if (kbhit())
      {
         break; 
      }
   }
}


void menu()
{
   int state;
   
   while (true)
   {
      putimage(0, 0, menu_image, COPY_PUT);
      for (int i = 0; i < N_BUTTONS; i++)
      {
         putimage(buttons[i].left, buttons[i].top,
                  buttons[i].image, COPY_PUT);
      }
      
      state = NONE;
      while (state == NONE)
      {
         while (mousebuttons() != 1);
         state = select_button();
      }
      
      switch (state)
      {
         case GAME:  game();  break;
         case ABOUT: about(); break;
         case SETTINGS: settings(); break;
         case RECORDS: records(); break;
         case EXIT: return;
      }
   }
}

void about()
{
   putimage(0, 0, about_image, COPY_PUT);
   getch();
}

void print_setting()
{
   setcolor(BLACK);
   setbkcolor(COLOR(238, 228, 203));
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
   outtextxy(120 , 50, "Settings");
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy(220 , 170, "Name");
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy(170 , 270, "Speed");
   rectangle(50, 100, 490, 500);
   rectangle(170, 190, 360, 230);
   rectangle(200, 300, 330, 350);
   rectangle(350, 300, 400, 350);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   outtextxy(360 , 320, "->");
   rectangle(130, 300, 180, 350);
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   outtextxy(140 , 320, "<-");
}

void settings()
{
   cleardevice();
   putimage(0, 0, menu_image, COPY_PUT);
   print_setting();
   stringstream speed;
   speed<< speeds;
   string numStr = speed.str();
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
   outtextxy(230 , 320, numStr.c_str());
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   outtextxy(180, 200, name.c_str());
   

      while (true) {
         if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
               
            if (mx >= 170 && mx <= 360 && my >= 190 && my <= 230)
            {
               name = "";
               cleardevice();
               print_setting();
               settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
               outtextxy(230 , 320, numStr.c_str());
               while (name.length() < 10)
               {
                  char c = getch();
                  if (c == KEY_BACKSPACE)
                  {
                     name.pop_back();
                     cleardevice();
                     print_setting();
                     settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                     outtextxy(230 , 320, numStr.c_str());
                     settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                     outtextxy(180, 200, name.c_str()); 
                  }
                  else if (c == KEY_ENTER || c == KEY_ESC)
                  {
                     break;
                  }
                  else if (isalpha(c))
                  {
                     name += c;
                     settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                     outtextxy(180, 200, name.c_str()); 
                  }
               }
            }
            
            if (mx >= 350 && mx <= 400 &&
                my >= 300 && my <= 350 && speeds > 100)
            {
               speeds -= 50;
               stringstream speed1;
               speed1<< speeds;
               numStr = speed1.str();
               settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
               outtextxy(230 , 320, numStr.c_str());
            }
            
            if (mx >= 130 && mx <= 180 && 
                my >= 300 && my <= 350 && speeds < 500)
            {
               speeds += 50;
               stringstream speed2;
               speed2<< speeds;
               numStr = speed2.str();
               settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
               outtextxy(230 , 320, numStr.c_str());
            }
            
            clearmouseclick(WM_LBUTTONDOWN);
         }
         if (kbhit())
         {
            int key = getch();
            if (key == KEY_ESC)
            {
               break;
            }
         }
      }
}

void sort_groups() {
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < num_groups - 1; i++) {
            if (groups[i].score< groups[i+1].score) {
                swap(groups[i], groups[i+1]);
                swapped = true;
            }
        }
    } while (swapped);
}

void print_groups() {
   setcolor(BLACK);
   setbkcolor(COLOR(238, 228, 203));
    for (int i = 0; i < num_groups; i++) {
      settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
      outtextxy(20, 100 + i * 50, groups[i].nickname.c_str());
      outtextxy(310, 100 + i * 50, 
                    std::to_string(groups[i].score).c_str()); 
    }
}

void records()
{
   cleardevice();
   putimage(0, 0, menu_image, COPY_PUT);
   setcolor(BLACK);
   setbkcolor(COLOR(238, 228, 203));
   settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
   outtextxy(20, 25, "Name");
   outtextxy(310, 25, "Points");
   line (0, 50, 540, 50);
   line (300, 0, 300, 600);
   sort_groups();
   print_groups();
   getch();
}

void close()
{
   freeimage(menu_image);
   freeimage(about_image);
   freeimage(start_image);
   for (int i = 0; i < N_BUTTONS; i++)
   {
      freeimage(buttons[i].image);
   }
}

void create_button(int i, int left, int top,
                   const char *file_name)
{
   buttons[i].image  = loadBMP(file_name);
   buttons[i].left   = left;
   buttons[i].top    = top;
   buttons[i].width  = imagewidth(buttons[i].image);
   buttons[i].height = imageheight(buttons[i].image);
}

int select_button()
{
   int x, y;
   
   x = mousex();
   y = mousey();
   
   for (int i = 0; i < N_BUTTONS; i++)
   {
      if (x > buttons[i].left &&
          x < buttons[i].left + buttons[i].width &&
          y > buttons[i].top &&
          y < buttons[i].top + buttons[i].height)
      {
         return i;
      }
   }
   
   return NONE;
}