#ifndef INPUT_H
#define INPUT_H

/* routines to manage the player input area
 */
#define TEXT_ENTRY    23
#define LINEBUF_LEN   32
#define OUTBUF_LEN    64

uint8_t input_init();
void input_dest();

void clear_input();
bool update_input();
unsigned char *get_line();