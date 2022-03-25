#include <atari.h>
#include <conio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "output.h"

bool done = false;
uint8_t cursor_x, cursor_y; // virtual coordinates of cursor
uint8_t old_x, old_y;

void save_cursor() {
    old_x = wherex();
    old_y = wherey();
}

void restore_cursor() {
    gotoxy(old_x, old_y);
}

void output_init() {
    uint8_t y;
    
    save_cursor();
    clrscr();
    for(y=UPPER_MARGIN; y<=LOWER_MARGIN; y++) {
        gotoxy(0,y);
        cputc(0x19);
        gotox(39);
        cputc(0x99);
    }
    restore_cursor();
    cursor_x = LEFT_MARGIN;
    cursor_y = UPPER_MARGIN;
    ostatus_init();
    pstatus_init();
}

void output_clear() {
    uint8_t y;
    
    save_cursor();
    for(y=UPPER_MARGIN; y<=LOWER_MARGIN; y++) {
        cclearxy(LEFT_MARGIN, y, OUTPUT_WIDTH);
    }
    cursor_x = LEFT_MARGIN;
    cursor_y = UPPER_MARGIN;
    restore_cursor();
}

void output_display(char *text) {
    uint8_t i;
    //uint16_t delay;
    
    save_cursor();
    gotoxy(cursor_x, cursor_y);
    if(done) {
        output_clear();
        done = false;
    }
    
    gotoxy(cursor_x, cursor_y);
    for(i=0; i<strlen(text); i++) {
        if(text[i] == 0x9B) {
            cursor_x = LEFT_MARGIN;
            cursor_y++;
            if(cursor_y > LOWER_MARGIN) {
                cursor_y = LOWER_MARGIN;
            }
            gotoxy(cursor_x, cursor_y);
        } else {
            cputc(text[i]);
            cursor_x++;
            if(cursor_x > LEFT_MARGIN+OUTPUT_WIDTH) {
                cursor_x = LEFT_MARGIN+OUTPUT_WIDTH;
                gotox(cursor_x);
            }
        }
        //for(delay=0; delay<511; delay++);
    }
    restore_cursor();
}

void output_done() {
    done = true;
}

void ostatus_init() {
    uint8_t i;
    
    save_cursor();
    revers(1);
    gotoxy(0, OUTPUT_STATUS);
    for(i=0; i<40; i++) {
        cputc(' ');
    }
    revers(0);
    restore_cursor();
}

void pstatus_init() {
    uint8_t i;
    
    save_cursor();
    revers(1);
    gotoxy(0, PLAYER_STATUS);
    for(i=0; i<40; i++) {
        cputc(' ');
    }
    revers(0);
    restore_cursor();
}