#ifndef TMCM16XX_H
#define TMCM16XX_H

class TMCM16XX {
    private:
        unsigned char cmd[9];
        int rec[9];
        void calcValueChecksum(int value);

    public:
        /* Command list */
        enum command {
            ROR = 1,
            ROL,
            MST,
            MVP,
            SAP,
            GAP,
            STAP,
            RSAP,
            SGP,
            GGP,
            STGP,
            RSGP,
            SIO = 14,
            GIO,
            CALC = 19,
            COMP,
            JC,
            JA,
            CSUB,
            RSUB,
            WAIT = 27,
            STOP,
            CALCX = 33,
            AAP,
            AGP
        };

        /* Type list for MVP command */
        enum mvp {
            ABS,
            REL
        };

        /* Type list for SAP, GAP, STAP, RSAP, AAP commands */
        enum parameter {
            TARGETPOS,
            ACTUALPOS,
            TARGETVELOCITY,
            ACTUALVELOCITY,
            MAXRAMPVELOCITY,
            MAXCURRENT = 6,
            TARGETVELOCITYFLAG,
            VELOCITYHALTFLAG = 9,
            TARGETREACHDISTANCE,
            ACCELERATIONVELOCITY,
            RAMPGENERATORSPEED = 13,
            THERMALWINDINGTIME = 25,
            I2TLIMIT,
            I2TSUM,
            I2TEXCEEDCOUNTER,
            I2TCLEARFLAG,
            MINUTECOUNTER,
            BLDCINIT,
            DVELOCITY = 133,
            DCURRENT,
            ENABLEVELOCITYRAMP = 146,
            ACTUALCURRENT = 150,
            SUPPLYVOLTAGE,
            TEMP,
            TARGETCURRENT = 155,
            ERRORFLAGS,
            COMMUTATIONMODE = 159,
            ENCODERSETNULL = 161,
            SWITCHSETNULL,
            ENCODERCLEAR,
            STOPSWITCH,
            ENCODEROFFSET,
            STOPSWITCHPOLARITY,
            PCURRENT = 172,
            ICURRENT,
            STARTCURRENT = 177,
            PIDCURRENTERROR = 200,
            PIDCURRENTERRORSUM,
            HALLANGLE = 210,
            ENCODERANGLE,
            CONTROLANGLE,
            PIDPOSITIONERROR = 226,
            PIDVELOCITYERROR = 228,
            PIDVELOCITYERRORSUM,
            PPOSITION,
            PVELOCITY = 234,
            IVELOCITY,
            SINEINITSPEED = 241,
            SINEINITDELAY = 244,
            OVERVOLTAGEPROT,
            INITSINEMODE = 249,
            ENCODERSTEPS,
            ENCODERDIR,
            MOTORPOLES = 253,
            HALLINVERT,
            ENABLEDRIVER
        };

        /* Constructor */

        TMCM16XX();

        /* Decode Functions */

        const int* decodeReceive(const char*);

        /* Move Functions */

        const unsigned char* setRotateRight(int);
        const unsigned char* setRotateLeft(int);
        const unsigned char* setMoveAbs(int);
        const unsigned char* setMoveRel(int);
        const unsigned char* setStop();

        /* Parameter Functions */

        const unsigned char* setAxisParameter(int, int);
        const unsigned char* getAxisParameter(int);
        const unsigned char* saveAxisParameter(int);
        const unsigned char* loadAxisParameter(int);
        const unsigned char* setGlobalParameter(int, int);
        const unsigned char* getGlobalParameter(int);
        const unsigned char* saveGlobalParameter(int);
        const unsigned char* loadGlobalParameter(int);

        /* Current Functions */

        const unsigned char* setCurrentMax(int);
        const unsigned char* setCurrent(int);
        const unsigned char* getCurrent();
        const unsigned char* setCurrentD(int);
        const unsigned char* setCurrentP(int);
        const unsigned char* setCurrentI(int);
        const unsigned char* getCurrentError();
        const unsigned char* getCurrentErrorSum();

        /* Velocity Functions */

        const unsigned char* setVelocity(int);
        const unsigned char* getVelocity();
        const unsigned char* setVelocityHaltFlag(int);
        const unsigned char* setVelocityD(int);
        const unsigned char* setVelocityP(int);
        const unsigned char* setVelocityI(int);
        const unsigned char* getVelocityError();
        const unsigned char* getVelocityErrorSum();
        const unsigned char* setVelocityRampMax(int);
        const unsigned char* setVelocityAcceleration(int);
        const unsigned char* getVelocityRamp();
        const unsigned char* setVelocityRampEnable(int);
};

#endif