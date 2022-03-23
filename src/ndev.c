#include <atari.h>
#include <stdlib.h>
#include "sio.h"
#include "ndev.h"

unsigned char nopen(char *filespec, unsigned char mode, unsigned char translation) {
	unsigned char err = 0;
	
	OS.dcb.ddevic=0x71;
	OS.dcb.dunit=1;
	OS.dcb.dcomnd='O';
	OS.dcb.dstats=0x80;
	OS.dcb.dbuf=filespec;
	OS.dcb.dtimlo=0x1f;
	OS.dcb.dbyt=256;
	OS.dcb.daux1=mode;
	OS.dcb.daux2=translation;
	siov();
	
	if (OS.dcb.dstats!=1)
    {
		if (OS.dcb.dstats==144) {
			OS.dcb.dcomnd='S';
			OS.dcb.dstats=0x40;
			OS.dcb.dbuf=OS.dvstat;
			OS.dcb.dbyt=4;
			OS.dcb.daux=0;
			siov();

			//itoa(OS.dvstat[3],&err,10);
			err = OS.dvstat[3];
		} else {
			//itoa(OS.dcb.dstats,&err,10);
			err = OS.dcb.dstats;
		}
    }
	return err;
}