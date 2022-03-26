#include <atari.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cursor.h"
#include "pstatus.h"

uint8_t pstatus_init() {
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

void pstatus_dest() {
}