#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
//#include <conio.h>
#include <stdio.h>
#include "ndev.h"

int main(int argc, char* argv[]) {
	uint8_t err;
	char *devicespec = "N:TELNET://gw-dev:4000";
    char *buf;
    nstatus_t *status;
    
    buf = (char *)malloc(128);
    status = (nstatus_t *)malloc(sizeof(nstatus_t));
	
	printf("Connecting to:\n[%s]\n", devicespec);
	err = nopen(devicespec, 12, 3);
	if(err != 1) {
		printf("Open error: %d\n", err);
	} else {
		err = nstatus(status);
        if(err != 1) {
            printf("Status error: %d\n", err);
        } else {
            printf("Bytes waiting: %d\n", status->bytes);
            while(status->bytes) {
                err = nread(buf, 127);
                if(err != 1) {
                    printf("Read error: %d\n", err);
                } else {
                    printf("%s", buf);
                }
                err = nstatus(status);
                if(err != 1) {
                    printf("Status error: %d\n", err);
                }
            }
        }
        err = nclose();
        if(err != 1) {
            printf("Close error: %d\n", err);
        } else {
            printf("Connection closed.");
        }
    }
    
    free(buf);
    free(status);
    
	return 0;
}