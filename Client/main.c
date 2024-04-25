#include <stdio.h>
#include <sys/types.h>

#include "RRCConnectionRequestCoder.h"
#include <unistd.h>
#include <time.h>


int main() {
    srand(time(NULL));

    uint8_t *buffer;
    ssize_t len;
    RRCConnectionRequestCoder(&buffer, &len);
    tx_send(&buffer, &len);
    free(buffer);
    
    return 0;
}
