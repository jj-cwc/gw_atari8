#ifndef PSTATUS_H
#define PSTATUS_H

/* routines to manages the player's status bar
 */

/* player status bar is on text row 0
 */
#define PLAYER_STATUS 0
 
uint8_t pstatus_init();
void pstatus_dest();

#endif