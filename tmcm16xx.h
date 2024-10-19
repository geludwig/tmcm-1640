#ifndef TMCM16XX_H
#define TMCM16XX_H

class TMCM16XX {
    private:
        unsigned char cmd[9];
        void calcValueChecksum(int value);

    public:
        TMCM16XX();

        const unsigned char* getFirmwareVersion(int type);
        const unsigned char* getLastCommand() const;
        const unsigned char* setRotateRight(int velocity);
        const unsigned char* setRotateLeft(int velocity);
        const unsigned char* setMoveAbs(int position);
        const unsigned char* setMoveRel(int offset);
        const unsigned char* setStop();
        const unsigned char* setAxisParameter(int type, int value);
        const unsigned char* getAxisParameter(int type);
        const unsigned char* saveAxisParameter(int type);
        const unsigned char* loadAxisParameter(int type);
        const unsigned char* setGlobalParameter(int type, int value);
        const unsigned char* getGlobalParameter(int type);
        const unsigned char* saveGlobalParameter(int type);
        const unsigned char* loadGlobalParameter(int type);

        const unsigned char* setCurrentMax(int value);
        const unsigned char* setCurrent(int value);
        const unsigned char* getCurrent(int value);
        const unsigned char* setCurrentPidDelay(int value);
        const unsigned char* setCurrentP(int value);
        const unsigned char* setCurrentI(int value);
        const unsigned char* getCurrentPidError(int value);


};

#endif