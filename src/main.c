#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include "ndev.h"
#include "output.h"

int main(int argc, char* argv[]) {
    output_init();
	if(!ndev_init()) {
        output_display("FujiNet init error.\n");
    } else {
        output_display("FujiNet init successful.\n");
        if(is_connected()) {
            output_display("Connected.\n");
        } else {
            output_display("Not connected.\n");
        }
        ndev_dest();
    }
    
	return 0;
}