#include <arpa/inet.h>
#include <conversions.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "RRCConnectionRequestCoder.h"

#define BUFFER_SIZE 1024

#define asn1cSeqAdd(VaR, PtR) \
    if (asn_sequence_add(VaR, PtR) != 0) assert(0)

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *len){
    RRCConnectionRequest_t *request;
    request = (RRCConnectionRequest_t*)calloc(1, sizeof(RRCConnectionRequest_t));

    request->criticalExtensions.present = RRCConnectionRequest__criticalExtensions_PR_rrcConnectionRequest_r8;

    request->criticalExtensions.choice.rrcConnectionRequest_r8.establishmentCause = EstablishmentCause_emergency;

    const size_t spareElementSize = 5;
    const size_t spareElementsCount = 2;
    request->criticalExtensions.choice.rrcConnectionRequest_r8.spare.buf = (uint8_t*)calloc(spareElementsCount, spareElementSize);
    request->criticalExtensions.choice.rrcConnectionRequest_r8.spare.size = spareElementsCount;
    request->criticalExtensions.choice.rrcConnectionRequest_r8.spare.bits_unused = 0;

    request->criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present = InitialUE_Identity_PR_randomValue;

    const size_t rndValuesCount = 10;
    uint8_t *values = (uint8_t*)calloc(rndValuesCount, 1);

    for (int i = 0; i < rndValuesCount; ++i) {
        values[i] = rand() % 256;
    }

    request->criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.buf = values;
    request->criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.size = rndValuesCount;
    request->criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.bits_unused = 0;

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER, &asn_DEF_RRCConnectionRequest, request);
    free(res.buffer);
    memset(&res, 0, sizeof(res));
    res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_RRCConnectionRequest, request);

    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        fprintf(stderr, "Error enconing rrc buffer\n");
        exit(1);
    } else {
        fprintf(stderr, "Encoded buffer\n");
    }

    xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, request);    
}


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