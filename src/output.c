#include <atari.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"
#include "cursor.h"
#include "output.h"

bool done = false;
uint8_t cursor_x, cursor_y; // virtual coordinates of cursor

/* initialize the output area
 */
uint8_t output_init() {
    uint8_t y;
    uint8_t err;
    
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
    err = ostatus_init();
    if(err) return err;
    return NO_ERROR;
}

/* clean up when done with output area
 */
void output_dest() {
    ostatus_dest();
}

/* clear the output area
 */

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

/* display some text in the output area
 */

void output_display(char *text) {
    uint8_t i;
    //uint16_t delay;
    char *start = text;
    
    while(start[0] == 0xFF) {
        start += 3;
    }
    
    save_cursor();
    gotoxy(cursor_x, cursor_y);
    if(done) {
        output_clear();
        done = false;
    }
    
    gotoxy(cursor_x, cursor_y);
    for(i=0; i<strlen(start); i++) {
        if(start[i] == 0x9B) {
            cursor_x = LEFT_MARGIN;
            cursor_y++;
            if(cursor_y > LOWER_MARGIN) {
                cursor_y = LOWER_MARGIN;
            }
            gotoxy(cursor_x, cursor_y);
        } else {
            cputc(start[i]);
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

/* clear the output area just before next output_display()
 */

void output_done() {
    done = true;
}

/* initialize the output area's status bar
 */

uint8_t ostatus_init() {
    uint8_t i;
    
    save_cursor();
    revers(1);
    gotoxy(0, OUTPUT_STATUS);
    for(i=0; i<40; i++) {
        cputc(' ');
    }
    revers(0);
    restore_cursor();
    return NO_ERROR;
}

/* clean up when done with output area's status bar
 */

void ostatus_dest() {
}