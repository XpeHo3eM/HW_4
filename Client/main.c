#include <stdio.h>
#include <sys/types.h>

#include <Attach_coder.h>
#include <unistd.h>

int main() {
    uint8_t *buffer;
    ssize_t len;
    S1SetupRequest_coder(&buffer, &len);
    tx_send(&buffer, &len);
    free(buffer);

    usleep(5000);
    InitialUEMessage_coder(&buffer, &len);
    tx_send(&buffer, &len);
    free(buffer);
    
    return 0;
}
