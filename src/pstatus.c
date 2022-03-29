#include <atari.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "errors.h"
#include "cursor.h"
#include "pstatus.h"

pstatus_t *pstatus;
char *pstatus_bar;

uint8_t pstatus_init() {
    pstatus = (pstatus_t *)malloc(sizeof(pstatus_t));
    if(!pstatus) {
        return OUT_OF_MEMORY_ERROR;
    }
    pstatus_bar = (char *)malloc(PSTATUS_WIDTH+1);
    
    if(!pstatus_bar) {
        free(pstatus);
        return OUT_OF_MEMORY_ERROR;
    }
    memset(pstatus_bar, ' ', PSTATUS_WIDTH);
    pstatus_bar[PSTATUS_WIDTH] = '\0';
    
    pstatus->connected = false;
    pstatus->net_mode = ' ';
    pstatus->data_ready = false;
    pstatus_display();
    return NO_ERROR;
}

void pstatus_dest() {
    free(pstatus);
    free(pstatus_bar);
}

void pstatus_display() {
    save_cursor();
    pstatus_bar[39] = pstatus->connected?0x14:' ';
    pstatus_bar[38] = pstatus->net_mode;
    pstatus_bar[37] = pstatus->data_ready?'*':' ';
    revers(1);
    gotoxy(0, PLAYER_STATUS);
    cputs(pstatus_bar);
    restore_cursor();
    revers(0);
}

void pset_connected(bool connected) {
    pstatus->connected = connected;
}

void pset_net_mode(char mode) {
    pstatus->net_mode = mode;
}

void pset_data_ready(bool ready) {
    pstatus->data_ready = ready;
}