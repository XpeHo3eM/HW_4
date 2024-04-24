#ifndef _S1SETUPREQUESTCODER_H
#define _S1SETUPREQUESTCODER_H

#include <stdio.h>
#include <sys/types.h>
#include <S1SetupRequest.h>

void S1SetupRequest_coder(uint8_t **buffer, ssize_t *len);
void InitialUEMessage_coder(uint8_t **buffer, ssize_t *len);
void tx_send(uint8_t **buffer, ssize_t *len);

#endif