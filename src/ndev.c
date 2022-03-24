#include <atari.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "sio.h"
#include "ndev.h"

void init_sio() {
    OS.dcb.ddevic = 0x71;
    OS.dcb.dunit = 1;
    OS.dcb.dcomnd = 'S';
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = OS.dvstat;
    OS.dcb.dtimlo = 0x1f;
    OS.dcb.dbyt = 4;
    OS.dcb.daux1 = 0;
    OS.dcb.daux2 = 0;
}

uint8_t nstatus(nstatus_t *status) {
    init_sio();
    siov();
    
    status->bytes = (OS.dvstat[1]<<8)+OS.dvstat[0];
    status->connected = OS.dvstat[2];
    status->error = OS.dvstat[3];
    return OS.dcb.dstats;
}

uint8_t nopen(char *filespec, uint8_t mode, uint8_t translation) {
	unsigned char err = 0;
	
    init_sio();
	OS.dcb.dcomnd='O';
	OS.dcb.dstats=0x80;
	OS.dcb.dbuf=filespec;
	OS.dcb.dbyt=256;
	OS.dcb.daux1=mode;
	OS.dcb.daux2=translation;
	siov();
	
    err = OS.dcb.dstats;
    if (err==144) {
        init_sio();
        siov();
        err = OS.dvstat[3];
    }
	return err;
}

uint8_t nclose() {
    init_sio();
    OS.dcb.dcomnd = 'C';
    OS.dcb.dstats = 0x00;
    OS.dcb.dbuf = NULL;
    OS.dcb.dbyt = 0;
    siov();
    
    return OS.dcb.dstats;
}

uint8_t nread(char *buf, uint16_t len) {
    init_sio();
    OS.dcb.dcomnd = 'R';
    OS.dcb.dbuf = buf;
    OS.dcb.dbyt = len;
    OS.dcb.daux = len;
    siov();
    
    return OS.dcb.dstats;
}

uint8_t nwrite(char *buf, uint16_t len) {
    init_sio();
    OS.dcb.dcomnd = 'W';
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = buf;
    OS.dcb.dbyt = len;
    OS.dcb.daux = len;
    
    return OS.dcb.dstats;
}