#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include "ndev.h"

int main(int argc, char* argv[]) {
    clrscr();
	if(!ndev_init()) {
        cputs("FujiNet initialization error.");
    } else {
        cputs("FujiNet initialization successful.");
        ndev_dest();
    }
    
	return 0;
}