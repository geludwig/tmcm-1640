// ################################################################################
// #                                                                              #
// #                         TMCM 16XX BLDC DRIVER LIBRARY                        #
// #                                      by                                      #
// #                                 Georg Ludwig                                 #
// #                         https://github.com/geludwig/                         #
// #                                                                              #
// ################################################################################

#include "tmcm16xx.h"

// ################################################################################
// #                                                                              #
// #                              PRIVATE FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Calculate Value And Checksum :
    int32_t value */
void TMCM16XX::calcValueChecksum(int32_t value) {
    cmd[4] = (value >> 24) & 0xFF;
    cmd[5] = (value >> 16) & 0xFF;
    cmd[6] = (value >> 8) & 0xFF;
    cmd[7] = value & 0xFF;

    uint8_t checksum = cmd[0];
    for (int8_t i = 1; i < 8; i++) {
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
    for (int8_t i = 0; i < 9; i++) {
        cmd[i] = 0;
    }
}

/*  Set serial receive device id.
    arg: uint8_t
    ret: signed int32_t[5] = {receive address, module address, status, command, value}
*/
void TMCM16XX::setSerialReceiveId(uint8_t value) {
    cmd[0] = value;
}

// ################################################################################
// #                                                                              #
// #                               DECODE FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Decode Receive Message.
    arg: uint8_t[9]
    ret: int32_t[5] = {receive address, module address, status, command, value}
*/
const int32_t* TMCM16XX::decodeReceive(const uint8_t* value) {
    /* calc checksum */
    uint8_t checksum = value[0];
    for (int8_t i = 1; i < 8; i++) {
        checksum += value[i];
    }
    /* return empty array when checksum check failed */
    if (checksum != value[8]) {
        for (int8_t i = 0; i < 9; i++) {
            rec[i] = 0;
        }
        return rec;
    }
    /* return decoded receive message */
    rec[0] = value[0];  // 0 receiver address
    rec[1] = value[1];  // 1 module address
    rec[2] = value[2];  // 2 status
    rec[3] = value[3];  // 3 last command

    /* cast hex values to single integer */
    rec[4] = ((uint32_t)(value[4] & 0xFF) << 24) |
             ((uint32_t)(value[5] & 0xFF) << 16) |
             ((uint32_t)(value[6] & 0xFF) << 8) |
             ((uint32_t)value[7] & 0xFF);

    return rec;
}

// ################################################################################
// #                                                                              #
// #                              COMMAND FUNCTIONS                               #
// #                                                                              #
// ################################################################################

/*  Description.
    arg:    enum Commands, enum Types, optional: int value
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::initCommand(Commands command, int32_t type, int32_t value = 0) {
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
    arg:    enum Types, int32_t value
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setAxisParameter(Types type, int32_t value) {
    initCommand(SAP, type, value);
    return cmd;
}

/*  Get Axis Parameter.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::getAxisParameter(Types type) {
    initCommand(GAP, type);
    return cmd;
}

/*  Store Axis Parameter previously set.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::saveAxisParameter(Types type) {
    initCommand(STAP, type);
    return cmd;
}

/*  Load Axis Parameter previously set.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::loadAxisParameter(Types type) {
    initCommand(RSAP, type);
    return cmd;
}

/*  Set Global Parameter.
    arg:    enum Types, int32_t value
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setGlobalParameter(Types type, int32_t value) {
    initCommand(SGP, type, value);
    return cmd;
}

/*  Get Global Parameter.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::getGlobalParameter(Types type) {
    initCommand(GGP, type);
    return cmd;
}

/*  Store Global Parameter.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::saveGlobalParameter(Types type) {
    initCommand(STGP, type);
    return cmd;
}

/*  Load Global Parameter.
    arg:    enum Types
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::loadGlobalParameter(Types type) {
    initCommand(RSGP, type);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                                MOVE FUNCTIONS                                #
// #                                                                              #
// ################################################################################

/*  Rotate Right.
    arg:    int32_t velocity
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setMoveRotateRight(int32_t value) {
    initCommand(ROR, 0, value);
    return cmd;
}

/*  Rotate Left.
    arg:    int32_t velocity
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setMoveRotateLeft(int32_t value) {
    initCommand(ROL, 0, value);
    return cmd;
}

/*  Stop rotation.
    arg:    na
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setStop() {
    initCommand(MST, 0);
    return cmd;
}

/*  Move Absolute.
    arg:    int32_t position
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setMoveAbs(int32_t value) {
    initCommand(MVP, ABSOLUTE, value);
    return cmd;
}

/*  Move Relative.
    arg:    int32_t offset
    ret:    uint8_t[9]
*/
const uint8_t* TMCM16XX::setMoveRel(int32_t value) {
    initCommand(MVP, RELATIVE, value);
    return cmd;
}
