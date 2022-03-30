#ifndef GW_H
#define GW_H

#define GW_URL "N:TCP://gw-dev:4000"
#define TRANSLATION 3
#define READBUF_LEN 1024

uint8_t gw_init();
void gw_dest();
void gw_loop();

void install_handler();
void remove_handler();

#endif