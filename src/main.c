#include <atari.h>
#include <stdint.h>
//#include <conio.h>
#include <stdio.h>
#include "ndev.h"

int main(int argc, char* argv[]) {
	uint8_t err;
	char *devicespec = "N:TCP://192.168.50.57:4000";
	
	printf("Connecting to:\n[%s]\n", devicespec);
	err = nopen(devicespec, 12, 0);
	if(err) {
		printf("SIO Error: %d\n", err);
	} else {
		printf("SIO Success\n");
	}
	return 0;
}