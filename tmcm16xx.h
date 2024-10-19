#ifndef TMCM16XX_H
#define TMCM16XX_H

class TMCM16XX {
    private:
        unsigned char cmd[9];
        void calcValueChecksum(int value);

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
            TARPOS,
            ACTPOS,
            TARSPEED,
            ACTSPEED,
            MAXRAMPVEL,
            MAXCUR = 6,
            TARREACHVEL,
            MOTORHALTVEL = 9,
            TARREACHDIST,
            ACCVEL,
            RAMPSPEED = 13,
            THERMWINDTIME = 25,
            I2TLIM,
            I2TSUM,
            I2TEXCEEDCOUNT,
            I2TCLEARFLAG,
            RUNMIN,
            BLDCRST,
            PIDDELAY = 133,
            PIDCURRENTDELAY,
            ENVELRAMP = 146,
            ACTCUR = 150,
            SUPVOLTAGE,
            TEMP,
            TARCUR = 155,
            ERRFLAGS,
            COMMUTATION = 159,
            ENCSETNULL = 161,
            SWSETNULL,
            ENCCLEAR,
            STOPSW,
            ENCOFFSET,
            STOPSWPOL,
            PCUR = 172,
            ICUR,
            STARTCUR = 177,
            PIDCURERR = 200,
            PIDCURERRSUM,
            HALLANGLE = 210,
            ENCANGLE,
            CONTROLANGLE,
            PIDPOSERR = 226,
            PIDVELERR = 228,
            PIDVELERRSUM,
            PPOS,
            PVEL = 234,
            IVEL,
            SINEINITSPEED = 241,
            SINEINITDELAY = 244,
            OVERVOLTAGEPROT,
            INITSINEMODE = 249,
            ENCSTEPS,
            ENCDIR,
            MOTORPOLES = 253,
            HALLINV,
            ENDRIVER
        };

    public:
        TMCM16XX();
        void debug();

        /* HIGH LEVEL FUNCTIONS */

        const unsigned char* setRotateRight(int velocity);
        const unsigned char* setRotateLeft(int velocity);
        const unsigned char* setMoveAbs(int position);
        const unsigned char* setMoveRel(int offset);
        const unsigned char* setStop();

        /* PARAMETER FUNCTIONS */

        const unsigned char* setAxisParameter(int type, int value);
        const unsigned char* getAxisParameter(int type);
        const unsigned char* saveAxisParameter(int type);
        const unsigned char* loadAxisParameter(int type);
        const unsigned char* setGlobalParameter(int type, int value);
        const unsigned char* getGlobalParameter(int type);
        const unsigned char* saveGlobalParameter(int type);
        const unsigned char* loadGlobalParameter(int type);

        /* CURRENT FUNCTIONS */

        const unsigned char* setCurrentMax(int value);
        const unsigned char* setCurrent(int value);
        const unsigned char* getCurrent();
        const unsigned char* setCurrentPidDelay(int value);
        const unsigned char* setCurrentP(int value);
        const unsigned char* setCurrentI(int value);
        const unsigned char* getCurrentPidError();
        const unsigned char* getCurrentPidErrorSum();


};

#endif