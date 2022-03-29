#include <atari.h>
#include <stdint.h>
#include <conio.h>
#include "errors.h"
#include "gw.h"


int main(int argc, char* argv[]) {
    uint8_t err;
    
    err = gw_init();
    if(err) {
        cprintf("Error: %d\r\n", err);
    } else {
        gw_loop();
    }
    
    gw_dest();
    
	return 0;
}