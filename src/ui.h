#ifndef UI_H
#define UI_H

#define OUTPUT_WIDTH 32  // 32-column output in case Adam or Spectrum clients join the fray 
#define LEFT_MARGIN  4  
#define UPPER_MARGIN 1   // Leave room for player status bar
#define LOWER_MARGIN 21  // Leave room for separator and input bar
#define OUTPUT_STATUS 22 // A status bar for the output area
#define PLAYER_STATUS 0

void ui_init();
void ui_dest();
void output_clear();                // clear the output area
void output_display(char *text);    // display text in the output area
void output_done();                 // clear before next display

void ostatus_init();

void pstatus_init();

#endif