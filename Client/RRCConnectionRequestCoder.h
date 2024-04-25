#ifndef _RRCCONNECTIONREQUESTCODER_H
#define _RRCCONNECTIONREQUESTCODER_H

#include <stdio.h>
#include <sys/types.h>
#include <RRCConnectionRequest.h>

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *len);
void tx_send(uint8_t **buffer, ssize_t *len);

#endif