
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

/* Initialize TMCM16XX */
TMCM16XX::TMCM16XX() {
    for (int i = 0; i < 9; i++) {
        cmd[i] = 0;
    }
    cmd[0] = 0;     // 1 byte: Module address
    cmd[3] = 0;     // 1 byte: Motor or Bank number
}

/*  Get current raw byte array.
    arg: na
    ret: char[9]
*/
const unsigned char* TMCM16XX::rawByteArray() const {
    return cmd;
}

/*  Rotate Right.
    arg: int velocity
    ret: char[9]
*/
const unsigned char* TMCM16XX::rotateRight(int value) {
    cmd[1] = 1;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Rotate Left.
    arg: int velocity
    ret: char[9]
*/
const unsigned char* TMCM16XX::rotateLeft(int value) {
    cmd[1] = 2;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Move Absolute.
    arg: int position
    ret: char[9]
*/
const unsigned char* TMCM16XX::moveAbs(int value) {
    cmd[1] = 4;
    cmd[2] = 0;
    calcValueChecksum(value);
    return cmd;
}

/*  Move Relative.
    arg: int offset
    ret: char[9]
*/
const unsigned char* TMCM16XX::moveRel(int value) {
    cmd[1] = 4;
    cmd[2] = 1;
    calcValueChecksum(value);
    return cmd;
}

/*  Stop.
    arg: na
    ret: char[9]
*/
const unsigned char* TMCM16XX::stop() {
    cmd[1] = 3;
    cmd[2] = 0;
    int value = 0;
    calcValueChecksum(value);
    return cmd;
}