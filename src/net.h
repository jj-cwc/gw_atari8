#ifndef NET_H
#define NET_H

#define GW_URL "N:TELNET://gw-dev:4000"

typedef struct {
    uint16_t bytes;
    uint8_t connected;
    uint8_t error;
} nstatus_t;

uint8_t net_init();
void net_dest();
bool is_connected();
bool data_available();
void net_reset();
uint8_t net_status(nstatus_t *status);
uint8_t net_open(char *filespec, uint8_t mode, uint8_t translation);
uint8_t net_close();
uint8_t net_read(char *buf, uint16_t len);
uint8_t net_write(char *buf, uint16_t len);

#endif