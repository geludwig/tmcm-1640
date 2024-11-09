/*

Serial wrapper by mbedded.ninja
https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp

*/

// tmcm headers
#include "tmcm16xx.h"

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

# define CRTSCTS 020000000000

int main() {
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyACM1", O_RDWR);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    // Send TMCM command example
    TMCM16XX tmcm;
    TMCM16XX();

    const unsigned char* command = tmcm.setMoveRotateRight(100);
    write(serial_port, command, 9);

    printf("WRITE ------------------------\n");
    printf("0 module address: %i\n", command[0]);
    printf("1 command:        %i\n", command[1]);
    printf("2 type:           %i\n", command[2]);
    printf("3 bank:           %i\n", command[3]);
    printf("4 value[0]        %i\n", command[4]);
    printf("5 value[1]:       %i\n", command[5]);
    printf("6 value[2]:       %i\n", command[6]);
    printf("7 value[3]:       %i\n", command[7]);
    printf("8 checksum:       %i\n", command[8]);
    sleep(1);

    // Allocate memory for read buffer, set size according to your needs
    char read_buf [9];

    // Read bytes.
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        return 1;
    }

    // Print TMCM receive message
    const signed int* rec = tmcm.decodeReceive(read_buf);
    printf("READ ------------------------\n");
    printf("Received %i bytes\n", num_bytes);
    printf("0 address   : %i\n", rec[0]);
    printf("1 m address : %i\n", rec[1]);
    printf("2 status    : %i\n", rec[2]);
    printf("3 command   : %i\n", rec[3]);
    printf("4 value     : %i\n", rec[4]);

    close(serial_port);
    return 0;
};