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
    int serial_port = open("/dev/ttyACM0", O_RDWR);

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

    // TMCM command example
    // #############################################################################################################################
    TMCM16XX tmcm;
    TMCM16XX();

    write(serial_port, tmcm.rotateLeft(500), 9);

    const unsigned char* command = tmcm.rawByteArray();
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

    // #############################################################################################################################

    // Allocate memory for read buffer, set size according to your needs
    char read_buf [256];

    // Normally you wouldn't do this memset() call, but since we will just receive
    // ASCII data for this example, we'll set everything to 0 so we can
    // call printf() easily.
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        return 1;
    }

    printf("READ ------------------------\n");
    printf("Received %i bytes\n", num_bytes);
    printf("0 receiver address: %i\n", read_buf[0]);
    printf("1 module address:   %i\n", read_buf[1]);
    printf("2 status:           %i\n", read_buf[2]);
    printf("3 command:          %i\n", read_buf[3]);
    printf("4 value[0]:         %i\n", read_buf[4]);
    printf("5 value[1]:         %i\n", read_buf[5]);
    printf("6 value[2]:         %i\n", read_buf[6]);
    printf("7 value[3]:         %i\n", read_buf[7]);
    printf("8 checksum:         %i\n", read_buf[8]);

    close(serial_port);
    return 0;
};