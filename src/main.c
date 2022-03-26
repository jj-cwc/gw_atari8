#include <atari.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include "errors.h"
#include "net.h"
#include "output.h"

int main(int argc, char* argv[]) {
    uint8_t err;
    char *buffer = (char *)malloc(1024);
    
    output_init();
	if(net_init() != SUCCESS) {
        output_display("FujiNet init error.\n");
    } else {
        output_display("FujiNet init successful.\n");
        if(is_connected()) {
            output_display("Connected.\n");
        } else {
            output_display("Not connected.\n");
        }
        while(!data_available());
        if(data_available()) {
            output_display("Data available.\n");
            err = net_read(buffer, 1023);
            if(err == 1) {
                output_done();
                output_display(buffer);
            } else {
                output_display("nread() error.\n");
            }
        } else {
            output_display("No data available.");
        }
        net_dest();
        output_dest();
        free(buffer);
    }
    
	return 0;
}