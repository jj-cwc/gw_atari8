#ifndef NDEV_H
#define NDEV_H

typedef struct {
    uint16_t bytes;
    uint8_t connected;
    uint8_t error;
} nstatus_t;

void init_sio();
uint8_t nstatus(nstatus_t *status);
uint8_t nopen(char *filespec, uint8_t mode, uint8_t translation);
uint8_t nclose();
uint8_t nread(char *buf, uint16_t len);
uint8_t nwrite(char *buf, uint16_t len);

#endif