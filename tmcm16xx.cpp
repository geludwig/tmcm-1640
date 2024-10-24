// ################################################################################
// #                                                                              #
// #                         TMCM 16XX BLDC DRIVER LIBRARY                        #
// #                                      by                                      #
// #                                 Georg Ludwig                                 #
// #                         https://github.com/geludwig/                         #
// #                                                                              #
// ################################################################################

/* Example Init Pseudocode
    TMCM16XX tmcm;
    TMCM16XX();

    const unsigned char* send;
    send = tmcm.initCommand(tmcm.SAP, tmcm.CURRENTTARGET, 100); // SetAxisParameter with CurrentTarget to 100mA. Or use:
    send = tmcm.setAxisParameter(tmcm.CURRENTTARGET, 100);      // Same command, but one abstraction layer higher.
    
    serial.write(command, 9);
*/

/* Functions tree
.
├── Constructor
├── Decode Functions
├── Command Functions
│   ├── Parameter Functions
│   ├── Move Functions
*/

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
// #                                 CONSTRUCTOR                                  #
// #                                                                              #
// ################################################################################

/* Constructor */
TMCM16XX::TMCM16XX() {
    for (int i = 0; i < 9; i++) {
        cmd[i] = 0;
    }
}

// ################################################################################
// #                                                                              #
// #                               DECODE FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Decode Receive Message.
    arg: unsigned char[9]
    ret: signed int[5] = {receive address, module address, status, command, value}
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
    rec[4] = ((value[4] & 0xFF) << 24) |
             ((value[5] & 0xFF)  << 16) |
             ((value[6] & 0xFF) << 8) |
             (value[7] & 0xFF);

    return rec;
}

// ################################################################################
// #                                                                              #
// #                              COMMAND FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Description.
    arg:    enum Commands, enum Types, optional: int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::initCommand(Commands command, int type, int value = 0) {
    cmd[1] = command;
    cmd[2] = type;
    calcValueChecksum(value);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                             PARAMETER FUNCTIONS                              #
// #                                                                              #
// ################################################################################

/*  Set Axis Parameter.
    arg:    enum Types, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setAxisParameter(Types type, int value) {
    initCommand(SAP, type, value);
    return cmd;
}

/*  Get Axis Parameter.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getAxisParameter(Types type) {
    initCommand(GAP, type);
    return cmd;
}

/*  Store Axis Parameter previously set.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::saveAxisParameter(Types type) {
    initCommand(STAP, type);
    return cmd;
}

/*  Load Axis Parameter previously set.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::loadAxisParameter(Types type) {
    initCommand(RSAP, type);
    return cmd;
}

/*  Set Global Parameter.
    arg:    enum Types, int value
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setGlobalParameter(Types type, int value) {
    initCommand(SGP, type, value);
    return cmd;
}

/*  Get Global Parameter.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::getGlobalParameter(Types type) {
    initCommand(GGP, type);
    return cmd;
}

/*  Store Global Parameter.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::saveGlobalParameter(Types type) {
    initCommand(STGP, type);
    return cmd;
}

/*  Load Global Parameter.
    arg:    enum Types
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::loadGlobalParameter(Types type) {
    initCommand(RSGP, type);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                                MOVE FUNCTIONS                                #
// #                                                                              #
// ################################################################################

/*  Rotate Right.
    arg:    int velocity
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveRotateRight(int value) {
    initCommand(ROR, 0, value);
    return cmd;
}

/*  Rotate Left.
    arg:    int velocity
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveRotateLeft(int value) {
    initCommand(ROL, 0, value);
    return cmd;
}

/*  Stop rotation.
    arg:    na
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setStop() {
    initCommand(MST, 0);
    return cmd;
}

/*  Move Absolute.
    arg:    int position
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveAbs(int value) {
    initCommand(MVP, ABSOLUTE, value);
    return cmd;
}

/*  Move Relative.
    arg:    int offset
    ret:    unsigned char[9]
*/
const unsigned char* TMCM16XX::setMoveRel(int value) {
    initCommand(MVP, RELATIVE, value);
    return cmd;
}
