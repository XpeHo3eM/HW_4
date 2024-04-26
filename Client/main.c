#include <stdio.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "RRCConnectionRequestCoder.h"
#include "RRCConnectionSetupCompleteCoder.h"
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024


void tx_send(uint8_t **buffer, ssize_t *len) {
    struct sockaddr_in servaddr = {
        servaddr.sin_family = AF_INET,
        servaddr.sin_port = htons(2500),
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"),
    };

    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sockfd < 0) {
        printf("Error when opening socket\n");
        exit(1);
    }

    int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        printf("Error when connecting socket\n");
        exit(1);
    }

    ret = sctp_sendmsg(sockfd, *buffer, *len, NULL, 0, 0, 0, 0, 0, 0);
    if (ret < 0) {
        printf("Error when sending msg\n");
        exit(1);
    }

    char resp[BUFFER_SIZE];
    ssize_t valread = read(sockfd, buffer, BUFFER_SIZE);
    if (valread > 0) {
        printf("Response from server: %s\n", *buffer);
    } else {
        printf("No response received from server.\n");
    }

    printf("Sent packet\n");

    close(sockfd);
}


int main() {
    srand(time(NULL));

    uint8_t *buffer;
    ssize_t len;
    RRCConnectionRequestCoder(&buffer, &len);
    //tx_send(&buffer, &len);
    free(buffer);

    RRCConnectionSetupCompleteCoder(&buffer, &len);
    //tx_send(&buffer, &len);

    
    return 0;
}
