#ifndef DRAWMENU
#define DRAWMENU

#include "script.h"
#include "keyboard.h"
#include <string>

extern int trainerToggleHotkey;

bool trainer_switch_pressed();
void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r);
void update_status_text();
void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false);
void menu_beep();
void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7);
void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true);
std::string line_as_str(std::string text, bool *pState);

#endif