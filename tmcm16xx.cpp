
//#define DEBUG

#include "tmcm16xx.h"
#include <stdio.h>

/*
cmd[0] = 0;                     // 1 byte: Module address
cmd[1] = 1;                     // 1 byte: Command number
cmd[2] = 0;                     // 1 byte: Type number
cmd[3] = 0;                     // 1 byte: Motor or Bank number
cmd[4] = (value >> 24) & 0xFF;  // 4 bytes value, MSB first
cmd[5] = (value >> 16) & 0xFF;
cmd[6] = (value >> 8) & 0xFF;
cmd[7] = value & 0xFF;
*/

// ################################################################################
// #                                                                              #
// #                              PRIVATE FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Calculate Value And Checksum :
    int value */
void TMCM16XX::calcValueChecksum(int value) {
    cmd[4] = (value >> 24) & 0xFF;
    cmd[5] = (value >> 16) & 0xFF;
    cmd[6] = (value >> 8) & 0xFF;
    cmd[7] = value & 0xFF;

    unsigned char checksum = cmd[0];
    for (int i = 1; i < 8; i++) {
        checksum += cmd[i];
    }
    cmd[8] = checksum;
}

// ################################################################################
// #                                                                              #
// #                               DECODE FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Decode Receive Message.
    arg: unsigned char[9]
    ret: signed char[5]
*/
const int* TMCM16XX::decodeReceive(const char *value) {    
    /* calc checksum */
    char checksum = value[0];
    for (int i = 1; i < 8; i++) {
        checksum += value[i];
    }
    /* return empty array when checksum check failed */
    if (checksum != value[8]) {
        for (int i = 0; i < 9; i++) {
            rec[i] = 0;
        }
        return rec;
    }
    /* return decoded receive message */
    rec[0] = value[0];  // 0 receiver address
    rec[1] = value[1];  // 1 module address
    rec[2] = value[2];  // 2 status
    rec[3] = value[3];  // 3 last command

    /* cast 2 signed values to single integer */
    rec[4] = ((value[4] & 0xFF) << 24) | ((value[5] & 0xFF)  << 16) | ((value[6] & 0xFF) << 8) | (value[7] & 0xFF);

    #ifdef DEBUG
    printf("[4] hex %x\n", value[4]);
    printf("[5] hex %x\n", value[5]);
    printf("[6] hex %x\n", value[6]);
    printf("[7] hex %x\n", value[7]);

    printf("rec hex %x\n", rec[4]);
    printf("rec int %i\n", rec[4]);
    #endif

    return rec;
}

// ################################################################################
// #                                                                              #
// #                             HIGH LEVEL FUNCTIONS                             #
// #                                                                              #
// ################################################################################

/* Initialize TMCM16XX */
TMCM16XX::TMCM16XX() {
    for (int i = 0; i < 9; i++) {
        cmd[i] = 0;
    }
}

/*  Rotate Right.
    arg:    int velocity
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setRotateRight(int value) {
    cmd[1] = ROR;
    calcValueChecksum(value);
    return cmd;
}

/*  Rotate Left.
    arg:    int velocity
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setRotateLeft(int value) {
    cmd[1] = ROL;
    calcValueChecksum(value);
    return cmd;
}

/*  Stop rotation.
    arg:    na
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setStop() {
    cmd[1] = MST;
    calcValueChecksum(0);
    return cmd;
}

/*  Move Absolute.
    arg:    int position
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveAbs(int value) {
    cmd[1] = MVP;
    cmd[1] = ABS;
    calcValueChecksum(value);
    return cmd;
}

/*  Move Relative.
    arg:    int offset
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveRel(int value) {
    cmd[1] = MVP;
    cmd[2] = REL;
    calcValueChecksum(value);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                             PARAMETER FUNCTIONS                              #
// #                                                                              #
// ################################################################################

/*  Set Axis Parameter.
    arg:    int type, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setAxisParameter(int type, int value) {
    cmd[1] = SAP;
    cmd[2] = type;
    calcValueChecksum(value);
    return cmd;
}

/*  Get Axis Parameter.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getAxisParameter(int type) {
    cmd[1] = GAP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Store Axis Parameter previously set.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::saveAxisParameter(int type) {
    cmd[1] = STAP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Load Axis Parameter previously set.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::loadAxisParameter(int type) {
    cmd[1] = RSAP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Set Global Parameter.
    arg:    int type, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setGlobalParameter(int type, int value) {
    cmd[1] = SGP;
    cmd[2] = type;
    calcValueChecksum(value);
    return cmd;
}

/*  Get Global Parameter.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getGlobalParameter(int type) {
    cmd[1] = GGP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Store Global Parameter.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::saveGlobalParameter(int type) {
    cmd[1] = STGP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Load Global Parameter.
    arg:    int type
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::loadGlobalParameter(int type) {
    cmd[1] = RSGP;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                             CURRENT FUNCTIONS                                #
// #                                                                              #
// ################################################################################

/*  Set Max Current.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrentMax(int value) {
    setAxisParameter(MAXCURRENT, value);
    return cmd;
}

/*  Get Actual Current.
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getCurrent() {
    getAxisParameter(ACTUALCURRENT);
    return cmd;
}

/*  Set Target Current.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrent(int value) {
    setAxisParameter(TARGETCURRENT, value);
    return cmd;
}

/*  Set PID Current Delay.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrentPidDelay(int value) {
    setAxisParameter(DCURRENT, value);
    return cmd;
}

/*  Set PID Current P Parameter.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrentP(int value) {
    setAxisParameter(PCURRENT, value);
    return cmd;
}

/*  Set PID Current I Parameter.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrentI(int value) {
    setAxisParameter(ICURRENT, value);
    return cmd;
}

/*  Get PID Error.
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getCurrentPidError() {
    getAxisParameter(PIDCURRENTERROR);
    return cmd;
}

/*  Get PID Error Sum.
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getCurrentPidErrorSum() {
    getAxisParameter(PIDCURRENTERRORSUM);
    return cmd;
}
