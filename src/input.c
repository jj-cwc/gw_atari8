#include <atari.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include "errors.h"
#include "cursor.h"
#include "input.h"

char *linebuf;

/* initialize the user input area
 */
uint8_t input_init() {
    linebuf = (char *)malloc(LINEBUF_LEN);
    if(!linebuf)
        return OUT_OF_MEMORY_ERROR;
    return SUCCESS;
}

/* clean up when done with user input area */
void input_dest() {
    free(linebuf);
}