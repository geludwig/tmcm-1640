// ################################################################################
// #                                                                              #
// #                         TMCM 16XX BLDC DRIVER LIBRARY                        #
// #                                      BY                                      #
// #                                   GELUDWIG                                   #
// #                         https://github.com/geludwig/                         #
// #                                                                              #
// ################################################################################


/* Example Init Pseudocode

    TMCM16XX tmcm;
    TMCM16XX();

    const unsigned char* command;

    command = tmcm.setCurrentParameter(tmcm.CURRENTTARGET, 100);
    serial.write(command, 9);

*/

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

#include "tmcm16xx.h"

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
    ret: signed char[5] = {receive address, module address, status, command, value}
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
// #                                MOVE FUNCTIONS                                #
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
const unsigned char* TMCM16XX::setMoveRotateRight(int value) {
    cmd[1] = ROR;
    calcValueChecksum(value);
    return cmd;
}

/*  Rotate Left.
    arg:    int velocity
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveRotateLeft(int value) {
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
// #                             CURRENT FUNCTIONS                                #
// #                                                                              #
// ################################################################################

/*  Set Current Parameter.
    arg:    int parameter, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setCurrentParameter(CurrentParameter type, int value) {
    setAxisParameter(type, value);
    return cmd;
}

/*  Get Current Parameter.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getCurrentParameter(CurrentParameter type) {
    getAxisParameter(type);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                            VELOCITY FUNCTIONS                                #
// #                                                                              #
// ################################################################################

/*  Set Velocity Parameter.
    arg:    int parameter, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setVelocityParameter(VelocityParameter type, int value) {
    setAxisParameter(type, value);
    return cmd;
}

/*  Get Velocity Parameter.
    arg:    int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getVelocityParameter(VelocityParameter type) {
    getAxisParameter(type);
    return cmd;
}