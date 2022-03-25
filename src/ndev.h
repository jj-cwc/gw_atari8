#ifndef NDEV_H
#define NDEV_H

#define GW_URL "N:TELNET://gw-dev:4000"

typedef struct {
    uint16_t bytes;
    uint8_t connected;
    uint8_t error;
} nstatus_t;

bool ndev_init();
void ndev_dest();
bool is_connected();
bool data_available();
void nreset();
uint8_t nstatus(nstatus_t *status);
uint8_t nopen(char *filespec, uint8_t mode, uint8_t translation);
uint8_t nclose();
uint8_t nread(char *buf, uint16_t len);
uint8_t nwrite(char *buf, uint16_t len);

#endif