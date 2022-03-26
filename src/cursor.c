#include <atari.h>
#include <conio.h>
#include <stdint.h>
#include "cursor.h"

uint8_t saved_x, saved_y;

void save_cursor() {
    saved_x = wherex();
    saved_y = wherey();
}

void restore_cursor() {
    gotoxy(saved_x, saved_y);
}