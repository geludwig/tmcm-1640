#ifndef TMCM16XX_H
#define TMCM16XX_H

class TMCM16XX {
    private:
        unsigned char cmd[9];
        void calcValueChecksum(int value);

    public:
        TMCM16XX();
        const unsigned char* rawByteArray() const;

        const unsigned char* rotateRight(int velocity);
        const unsigned char* rotateLeft(int velocity);
        const unsigned char* moveAbs(int position);
        const unsigned char* moveRel(int offset);
        const unsigned char* stop();
};

#endif