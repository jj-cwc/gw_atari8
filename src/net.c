#include <atari.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "errors.h"
#include "sio.h"
#include "net.h"

// interrupt handler related stuff
uint8_t trip;
void *old_vprced; // the old PROCEED vector
bool old_enabled; // old interrupt state
extern void ih(); // new interrupt handler

nstatus_t *status;

uint8_t net_init() {
    uint8_t err;
    
    // allocate a status buffer
    status = (nstatus_t *)malloc(sizeof(nstatus_t));
    if(!status) {
        return OUT_OF_MEMORY_ERROR;
    }
    
    // check status of FujiNet
    err = net_status(status);
    if(err != 1) {
        free(status);
        return FUJINET_STATUS_ERROR;
    }
    
    // set up PROCEED interrupt handler
    old_vprced  = OS.vprced;
    old_enabled = PIA.pactl & 1;
    PIA.pactl  &= (~1);
    OS.vprced   = ih;
    PIA.pactl  |= 1;
    
    err = net_open(GW_URL, 12, 3);
    if(err != 1) {
        net_dest();
        return FUJINET_OPEN_ERROR;
    }
    
    return SUCCESS;
}

void net_dest() {
    net_close();
    free(status);
    PIA.pactl &= ~1;
    OS.vprced=old_vprced; 
    PIA.pactl |= old_enabled; 
}

bool is_connected() {
    return status->connected;
}

bool data_available() {
    return (trip!=0);
}

void net_reset() {
    trip=0;
    PIA.pactl |= 1;
}

uint8_t net_status(nstatus_t *status) {
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'S';
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = OS.dvstat;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = 4;
    OS.dcb.daux1 = 0;
    OS.dcb.daux2 = 0;
    siov();
    
    status->bytes = (OS.dvstat[1]<<8)+OS.dvstat[0];
    status->connected = OS.dvstat[2];
    status->error = OS.dvstat[3];
    return OS.dcb.dstats;
}

uint8_t net_open(char *filespec, uint8_t mode, uint8_t translation) {
	unsigned char err = 0;
	
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'O';
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = filespec;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = 256;
    OS.dcb.daux1 = mode;
    OS.dcb.daux2 = translation;
	siov();
	
    err = OS.dcb.dstats;
    if (err==144) {
        net_status(status);
        err = status->error;
    }
	return err;
}

uint8_t net_close() {
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'S';
    OS.dcb.dstats = 0x00;
    OS.dcb.dbuf = NULL;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = 0;
    OS.dcb.daux1 = 0;
    OS.dcb.daux2 = 0;
    siov();
    
    return OS.dcb.dstats;
}

uint8_t net_read(char *buf, uint16_t len) {
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'R';
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = buf;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = len;
    OS.dcb.daux = len;
    siov();
    
    net_reset();
    
    return OS.dcb.dstats;
}

uint8_t net_write(char *buf, uint16_t len) {
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'W';
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = buf;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = len;
    OS.dcb.daux = len;
    siov();
    
    return OS.dcb.dstats;
}