#ifndef INPUT_H
#define INPUT_H

/* routines to manage the player input area
 */
#define TEXT_ENTRY    23
#define LINEBUF_LEN   32

uint8_t input_init();
void input_dest();