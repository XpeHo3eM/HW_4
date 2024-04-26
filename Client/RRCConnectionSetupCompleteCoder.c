#include <conversions.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "RRCConnectionSetupCompleteCoder.h"


void RRCConnectionSetupCompleteCoder(uint8_t **buffer, ssize_t *len){
    RRCConnectionSetupComplete_t *request;
    request = (RRCConnectionSetupComplete_t*)calloc(1, sizeof(RRCConnectionSetupComplete_t));

    request->rrc_TransactionIdentifier = 1;
    request->criticalExtensions.present = RRCConnectionSetupComplete__criticalExtensions_PR_c1;
    request->criticalExtensions.choice.c1.present = RRCConnectionSetupComplete__criticalExtensions__c1_PR_rrcConnectionSetupComplete_r8;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.selectedPLMN_Identity = 1;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS.buf = (uint8_t*)calloc(1, 1);
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS.size = 1;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS._asn_ctx.phase = 1;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS._asn_ctx.step = 1;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS._asn_ctx.context = 1;
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS._asn_ctx.ptr = calloc(1, 10);
    request->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8.dedicatedInfoNAS._asn_ctx.left = -1;
    
    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER, &asn_DEF_RRCConnectionSetupComplete, request);
    free(res.buffer);
    memset(&res, 0, sizeof(res));
    res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_RRCConnectionSetupComplete, request);

    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        fprintf(stderr, "Error enconing rrc buffer\n");
        exit(1);
    } else {
        fprintf(stderr, "Encoded buffer\n");
    }

    xer_fprint(stdout, &asn_DEF_RRCConnectionSetupComplete, request);    
}