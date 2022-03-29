#ifndef PSTATUS_H
#define PSTATUS_H

/* routines to manages the player's status bar
 */

/* player status bar is on text row 0
 */
#define PLAYER_STATUS 0
#define PSTATUS_WIDTH 40

typedef struct {
    bool connected;
    bool data_ready;
    char net_mode;
} pstatus_t;

uint8_t pstatus_init();
void pstatus_dest();

void pstatus_display();
void pset_net_mode(char mode);
void pset_connected(bool connected);
void pset_data_ready(bool ready);

#endif