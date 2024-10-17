#ifndef TMC16XX_H
#define TMC16XX_H

class TMC16XX {
    private:
        unsigned char cmd[9];
        void calcValueChecksum(int value);

    public:
        TMC16XX();
        const unsigned char* rawByteArray() const;

        const unsigned char* rotateRight(int velocity);
        const unsigned char* rotateLeft(int velocity);
        const unsigned char* moveAbs(int position);
        const unsigned char* moveRel(int offset);
        const unsigned char* stop();
};

#endif