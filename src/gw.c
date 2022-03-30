#include <atari.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include "errors.h"
#include "nio.h"
#include "output.h"
#include "pstatus.h"
#include "input.h"
#include "gw.h"

unsigned char trip=0;           // if trip=1, fujinet is asking us for attention.
bool old_enabled=false;         // were interrupts enabled for old vector
void* old_vprced;               // old PROCEED vector, restored on exit.
extern void ih();               // defined in intr.s

unsigned char *readbuf;
//char *url = GW_URL;
uint16_t bytes_waiting;

/* bring up all of the components
 */
uint8_t gw_init() {
    uint8_t err;
    
    if(!(readbuf = (unsigned char *)malloc(READBUF_LEN+1))) {
        return OUT_OF_MEMORY_ERROR;
    }    
    readbuf[READBUF_LEN] = '\0';
    
    /*
    err = net_init();
    if(err) {
        return err;
    }
    */
    
    err = output_init();
    if(err) {
        return err;
    }
    
    err = pstatus_init();
    if(err) {
        output_dest();
        return err;
    }
    
    err = input_init();
    if(err) {
        output_dest();
        pstatus_dest();
        return err;
    }
    
    err = nopen(GW_URL, TRANSLATION);
    if(err != SUCCESS) {
        input_dest();
        output_dest();
        pstatus_dest();
        return FUJINET_OPEN_ERROR;
    }
    
    install_handler();
    return NO_ERROR;
}

/* shut down all components
 */
void gw_dest() {
    remove_handler();
    free(readbuf);
    input_dest();
    pstatus_dest();
    output_dest();
}

/* main loop
 */
void gw_loop() {
    uint8_t err;
    //uint16_t bytes_waiting;
    //uint16_t bytes_read;
    unsigned char *usertext;
    bool running = true;
    bool incomplete = false;
    
    while(running) {
        pset_connected(true);
        pset_data_ready(running);
        if(update_input()) {
            usertext = get_line();
            pset_net_mode('W');
            pstatus_display();
            err = nwrite(GW_URL, usertext, strlen(usertext));
            if(err != 1) {
                //output_done();
                output_display("Write error\n");
                running = false;
                continue;
            }
            pset_net_mode(' ');
            pstatus_display();
        }
        if(trip == 0) {
            continue;
        }
        err = nstatus(GW_URL);
        if(err == 136) {
            //output_done();
            output_display("DISCONNECTED.\n");
            running = false;
            continue;
        } else if(err != 1) {
            //output_done();
            output_display("Status error\n");
            running = false;
            continue;
        }
        
        bytes_waiting = OS.dvstat[1]*256+OS.dvstat[0];
        if(bytes_waiting > 0) {
            if(bytes_waiting > READBUF_LEN) {
                bytes_waiting = READBUF_LEN;
                incomplete = true;
            }
            
            pset_net_mode('R');
            pstatus_display();
            err = nread(GW_URL, readbuf, bytes_waiting);
            if(err != 1) {
                //output_done();
                output_display("Read error\n");
                running = false;
                continue;
            }
            readbuf[bytes_waiting] = '\0';
            pset_net_mode(' ');
            pstatus_display();
            output_display(readbuf);

            pset_net_mode(' ');
            pset_data_ready(false);
            pstatus_display();
            if(!incomplete) {
                output_done();
            } else {
                incomplete = false;
            }
            
            trip=0;
            PIA.pactl |= 1; // Flag interrupt as serviced, ready for next one.
        }
    }
    pset_connected(false);
    pset_data_ready(false);
    pstatus_display();
    
    gotoxy(0,22);
}

void install_handler() {
    old_vprced  = OS.vprced;     // save the old interrupt vector 
    old_enabled = PIA.pactl & 1; // keep track of old interrupt state
    PIA.pactl  &= (~1);          // Turn off interrupts before changing vector
    OS.vprced   = ih;            // Set PROCEED interrupt vector to our interrupt handler.
    PIA.pactl  |= 1;             // Indicate to PIA we are ready for PROCEED interrupt.
}

void remove_handler() {
    // Restore old PROCEED interrupt.
    PIA.pactl &= ~1; // disable interrupts
    OS.vprced=old_vprced; 
    PIA.pactl |= old_enabled; 
}