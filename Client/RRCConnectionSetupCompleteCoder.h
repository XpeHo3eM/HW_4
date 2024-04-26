#ifndef _RRCCONNECTIONSETUPCOMPLETECODER_H
#define _RRCCONNECTIONSETUPCOMPLETECODER_H

#include <stdio.h>
#include <sys/types.h>
#include <RRCConnectionSetupComplete.h>

void RRCConnectionSetupCompleteCoder(uint8_t **buffer, ssize_t *len);
void tx_send(uint8_t **buffer, ssize_t *len);

#endif