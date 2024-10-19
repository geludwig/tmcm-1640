
#include "tmcm16xx.h"

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

/*  Calculate Value And Checksum :
    int value */
void TMCM16XX::calcValueChecksum(int value) {
    cmd[4] = (value >> 24) & 0xFF;
    cmd[5] = (value >> 16) & 0xFF;
    cmd[6] = (value >> 8) & 0xFF;
    cmd[7] = value & 0xFF;

    unsigned char checksum = cmd[0];
    for (int i = 0; i < 8; i++) {
        checksum += cmd[i];
    }
    cmd[8] = checksum;
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
    cmd[0] = 0;     // 1 byte: Module address
    cmd[3] = 0;     // 1 byte: Motor or Bank number
}

/*  Get Firmware version.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::getFirmwareVersion(int type) {
    cmd[1] = 136;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Get last command.
    arg:    na
    ret:    char[9]
*/
const unsigned char* TMCM16XX::getLastCommand() const {
    return cmd;
}

/*  Rotate Right.
    arg:    int velocity
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setRotateRight(int value) {
    cmd[1] = 1;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Rotate Left.
    arg:    int velocity
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setRotateLeft(int value) {
    cmd[1] = 2;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Move Absolute.
    arg:    int position
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setMoveAbs(int value) {
    cmd[1] = 4;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Move Relative.
    arg:    int offset
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setMoveRel(int value) {
    cmd[1] = 4;
    cmd[2] = 1;
    calcValueChecksum(value);
    return cmd;
}

/*  Stop rotation.
    arg:    na
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setStop() {
    cmd[1] = 3;
    cmd[2] = 0;
    calcValueChecksum(0);
    return cmd;
}

// ################################################################################
// #                                                                              #
// #                             PARAMETER FUNCTIONS                              #
// #                                                                              #
// ################################################################################

/*  Set Axis Parameter.
    arg:    int type, int value
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setAxisParameter(int type, int value) {
    cmd[1] = 5;
    cmd[2] = type;
    calcValueChecksum(value);
    return cmd;
}

/*  Get Axis Parameter.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::getAxisParameter(int type) {
    cmd[1] = 6;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Store Axis Parameter previously set.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::saveAxisParameter(int type) {
    cmd[1] = 7;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Load Axis Parameter previously set.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::loadAxisParameter(int type) {
    cmd[1] = 8;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Set Global Parameter.
    arg:    int type, int value
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setGlobalParameter(int type, int value) {
    cmd[1] = 9;
    cmd[2] = type;
    calcValueChecksum(value);
    return cmd;
}

/*  Get Global Parameter.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::getGlobalParameter(int type) {
    cmd[1] = 10;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Store Global Parameter.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::saveGlobalParameter(int type) {
    cmd[1] = 11;
    cmd[2] = type;
    calcValueChecksum(0);
    return cmd;
}

/*  Load Global Parameter.
    arg:    int type
    ret:    char[9]
*/
const unsigned char* TMCM16XX::loadGlobalParameter(int type) {
    cmd[1] = 12;
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
    ret:    char[9]
*/
const unsigned char* TMCM16XX::setCurrentMax(int value) {
    setAxisParameter(6, value);
    return cmd;
}