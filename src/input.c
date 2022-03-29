#include <atari.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "errors.h"
//#include "cursor.h" not saving cursor position for now because this is where the cursor should be most of the time
#include "input.h"

unsigned char *linebuf;
unsigned char *outbuf;
uint8_t pos;

/* initialize the user input area
 */
uint8_t input_init() {
    linebuf = (unsigned char *)malloc(LINEBUF_LEN);
    if(!linebuf) {
        return OUT_OF_MEMORY_ERROR;
    }
    outbuf = (unsigned char *)malloc(OUTBUF_LEN);
    if(!outbuf) {
        free(linebuf);
        return OUT_OF_MEMORY_ERROR;
    }
    clear_input();
    return NO_ERROR;
}

/* clean up when done with user input area */
void input_dest() {
    free(linebuf);
}

/* clear the line buffer and input display
 */
void clear_input() {
    pos = 0;
    gotoxy(0, TEXT_ENTRY);
    cclear(40);
    gotoxy(0, TEXT_ENTRY);
}

/* check for input and mark line buffer as ready for processing if enter pressed
 */
bool update_input() {
    char c;
    bool status = false;
    
    if(kbhit()) {
        c = cgetc();
        if(c == 0x9b) {
            status = true;
            memcpy(outbuf, linebuf, pos);
            outbuf[pos] = 0x9b;
            outbuf[pos+1] = '\0';
            clear_input();
        } else if(c == 0x7E) {
            if(pos > 0) {
                gotoxy(pos-1, TEXT_ENTRY);
                cputc(' ');
                linebuf[--pos]='\0';
            }
        } else if(pos < LINEBUF_LEN) {
            gotoxy(pos,TEXT_ENTRY);
            cputc(c);
            linebuf[pos++] = c;
        }
    }
    return status;   
}

unsigned char *get_line() {
    return outbuf;
}