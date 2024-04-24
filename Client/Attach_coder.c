#include "Attach_coder.h"

#include <ProtocolIE-Field.h>
#include <S1AP-PDU.h>
#include <S1SetupRequest.h>
#include <InitialUEMessage.h>
#include <SupportedTAs-Item.h>
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

#define PAYLOAD_MAX 100

#define asn1cSeqAdd(VaR, PtR) \
    if (asn_sequence_add(VaR, PtR) != 0) assert(0)

void S1SetupRequest_coder(uint8_t **buffer, ssize_t *len) {
    S1AP_PDU_t pdu;
    S1SetupRequest_t *out = NULL;
    S1SetupRequestIEs_t *ie = NULL;
    SupportedTAs_Item_t *ta = NULL;
    PLMNidentity_t *plmn = NULL;

    int mcc = 320;
    int mnc = 230;
    int mnc_digit_length = 3;
    int tac = 5;
    int eNB_id = 1;
    long default_drx = 1;

    // Define the S1AP message to encode
    memset(&pdu, 0, sizeof(pdu));
    pdu.present = S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage.procedureCode = ProcedureCode_id_S1Setup;
    pdu.choice.initiatingMessage.criticality = Criticality_reject;
    pdu.choice.initiatingMessage.value.present = InitiatingMessage__value_PR_S1SetupRequest;
    out = &pdu.choice.initiatingMessage.value.choice.S1SetupRequest;

    /* optional */
    const char *eNB_name = "TEST_ENB";
    if (eNB_name) {
        ie = (S1SetupRequestIEs_t *)calloc(1, sizeof(S1SetupRequestIEs_t));
        ie->id = ProtocolIE_ID_id_eNBname;
        ie->criticality = Criticality_ignore;
        ie->value.present = S1SetupRequestIEs__value_PR_ENBname;
        OCTET_STRING_fromBuf(&ie->value.choice.ENBname, eNB_name,
                             strlen(eNB_name));
        asn1cSeqAdd(&out->protocolIEs.list, ie);
    }

    // mandatory
    ie = (S1SetupRequestIEs_t *)calloc(1, sizeof(S1SetupRequestIEs_t));
    ie->id = ProtocolIE_ID_id_Global_ENB_ID;
    ie->criticality = Criticality_reject;
    ie->value.present = S1SetupRequestIEs__value_PR_Global_ENB_ID;

    MCC_MNC_TO_PLMNID(mcc, mnc, mnc_digit_length, &ie->value.choice.Global_ENB_ID.pLMNidentity);
    ie->value.choice.Global_ENB_ID.eNB_ID.present = ENB_ID_PR_macroENB_ID;
    MACRO_ENB_ID_TO_BIT_STRING(eNB_id,
                               &ie->value.choice.Global_ENB_ID.eNB_ID.choice.macroENB_ID);
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    /* mandatory */
    ie = (S1SetupRequestIEs_t *)calloc(1, sizeof(S1SetupRequestIEs_t));
    ie->id = ProtocolIE_ID_id_SupportedTAs;
    ie->criticality = Criticality_reject;
    ie->value.present = S1SetupRequestIEs__value_PR_SupportedTAs;
    {
        ta = (SupportedTAs_Item_t *)calloc(1, sizeof(SupportedTAs_Item_t));
        INT16_TO_OCTET_STRING(tac, &ta->tAC);
        {
            for (int i = 0; i < 1; ++i) {
                plmn = (PLMNidentity_t *)calloc(1, sizeof(PLMNidentity_t));
                MCC_MNC_TO_TBCD(mcc, mnc, mnc_digit_length, plmn);
                asn1cSeqAdd(&ta->broadcastPLMNs.list, plmn);
            }
        }
        asn1cSeqAdd(&ie->value.choice.SupportedTAs.list, ta);
    }
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    /* mandatory */
    ie = (S1SetupRequestIEs_t *)calloc(1, sizeof(S1SetupRequestIEs_t));
    ie->id = ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = Criticality_ignore;
    ie->value.present = S1SetupRequestIEs__value_PR_PagingDRX;
    ie->value.choice.PagingDRX = default_drx;
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER, &asn_DEF_S1AP_PDU, &pdu);
    free(res.buffer);
    memset(&res, 0, sizeof(res));
    res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_S1AP_PDU, &pdu);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        fprintf(stderr, "Error enconing s1ap pdu\n");
        exit(1);
    } else {
        fprintf(stderr, "Encoded pdu\n");
    }

    xer_fprint(stdout, &asn_DEF_S1AP_PDU, &pdu);
}

void InitialUEMessage_coder(uint8_t **buffer, ssize_t *len) {
    S1AP_PDU_t pdu;
    InitialUEMessage_t *out = NULL;
    InitialUEMessage_IEs_t *ie = NULL;
    SupportedTAs_Item_t *ta = NULL;
    PLMNidentity_t *plmn = NULL;

    int mcc = 320;
    int mnc = 230;
    int mnc_digit_length = 3;
    int tac = 5;
    int eNB_id = 1;
    int eNB_ue_s1ap_id = 10;

    // Define the S1AP message to encode
    memset(&pdu, 0, sizeof(pdu));
    pdu.present = S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage.procedureCode = ProcedureCode_id_initialUEMessage;
    pdu.choice.initiatingMessage.criticality = Criticality_reject;
    pdu.choice.initiatingMessage.value.present = InitiatingMessage__value_PR_InitialUEMessage;
    out = &pdu.choice.initiatingMessage.value.choice.InitialUEMessage;

    ie = (InitialUEMessage_IEs_t *)calloc(1, sizeof(InitialUEMessage_IEs_t));
    ie->id = ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = Criticality_reject;
    ie->value.present = InitialUEMessage_IEs__value_PR_ENB_UE_S1AP_ID;
    ie->value.choice.ENB_UE_S1AP_ID = eNB_ue_s1ap_id;
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    ie = (InitialUEMessage_IEs_t *)calloc(1, sizeof(InitialUEMessage_IEs_t));
    ie->id = ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = Criticality_reject;
    ie->value.present = InitialUEMessage_IEs__value_PR_NAS_PDU;
    uint8_t nas_pdu_buffer[] = {
        0x07, 0x41, 0x71, 0x08, 0x39, 0x02, 0x32, 0x10, 0x00, 0x00, 0x00, 0x40, 0x02, 0x80, 0x40, 0x00, 0x20,
        0x02, 0x01, 0xd0, 0x11, 0x27, 0x1a, 0x80, 0x80, 0x21, 0x10, 0x01, 0x00, 0x00, 0x10, 0x81, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x83, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x0a, 0x00};
    //07417108390232100000004002804000200201d011271a8080211001000010810600000000830600000000000d00000a00
    ie->value.choice.NAS_PDU.buf = nas_pdu_buffer;
    ie->value.choice.NAS_PDU.size = 49;
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    ie = (InitialUEMessage_IEs_t *)calloc(1, sizeof(InitialUEMessage_IEs_t));
    ie->id = ProtocolIE_ID_id_TAI;
    ie->criticality = Criticality_reject;
    ie->value.present = InitialUEMessage_IEs__value_PR_TAI;
    /* Assuming TAI is the TAI from the cell */
    INT16_TO_OCTET_STRING(tac, &ie->value.choice.TAI.tAC);
    MCC_MNC_TO_PLMNID(mcc, mnc, mnc_digit_length,&ie->value.choice.TAI.pLMNidentity);
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    ie = (InitialUEMessage_IEs_t *)calloc(1, sizeof(InitialUEMessage_IEs_t));
    ie->id = ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = Criticality_ignore;
    ie->value.present = InitialUEMessage_IEs__value_PR_EUTRAN_CGI;
    MACRO_ENB_ID_TO_CELL_IDENTITY(eNB_id,0,&ie->value.choice.EUTRAN_CGI.cell_ID);
    MCC_MNC_TO_TBCD(mcc,mnc,mnc_digit_length,&ie->value.choice.EUTRAN_CGI.pLMNidentity);
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    ie = (InitialUEMessage_IEs_t *)calloc(1, sizeof(InitialUEMessage_IEs_t));
    ie->id = ProtocolIE_ID_id_RRC_Establishment_Cause;
    ie->criticality = Criticality_ignore;
    ie->value.present = InitialUEMessage_IEs__value_PR_RRC_Establishment_Cause;
    ie->value.choice.RRC_Establishment_Cause = RRC_Establishment_Cause_mt_Access;
    asn1cSeqAdd(&out->protocolIEs.list, ie);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER, &asn_DEF_S1AP_PDU, &pdu);
    free(res.buffer);
    memset(&res, 0, sizeof(res));
    res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_S1AP_PDU, &pdu);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL) {
        fprintf(stderr, "Error enconing s1ap pdu\n");
        exit(1);
    } else {
        fprintf(stderr, "Encoded pdu\n");
    }

    xer_fprint(stdout, &asn_DEF_S1AP_PDU, &pdu);
}

void tx_send(uint8_t **buffer, ssize_t *len) {
    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(36412),
        .sin_addr.s_addr = inet_addr("10.18.10.221"),
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

    printf("Sent packet\n");

    close(sockfd);
}