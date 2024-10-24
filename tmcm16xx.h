// ################################################################################
// #                                                                              #
// #                         TMCM 16XX BLDC DRIVER LIBRARY                        #
// #                                      by                                      #
// #                                 Georg Ludwig                                 #
// #                         https://github.com/geludwig/                         #
// #                                                                              #
// ################################################################################

#ifndef TMCM16XX_H
#define TMCM16XX_H

class TMCM16XX {
    private:
        unsigned char cmd[9];
        int rec[9];
        void calcValueChecksum(int value);

    public:
        /* Commands */
        enum Commands {
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

        /* Types */
        enum Types {
            VELOCITYTARGET = 2,
            VELOCITYGETACTUAL,
            VELOCITYRAMPMAX,
            CURRENTMAX = 6,
            VELOCITYHALTFLAG = 9,
            VELOCITYACCELERATION = 11,
            VELOCITYGETRAMPACTUAL = 13,
            VELOCITYD = 133,
            CURRENTD = 134,
            VELOCITYRAMPENABLE = 146,
            CURRENTGETACTUAL = 150,
            CURRENTTARGET = 155,
            CURRENTP = 172,
            CURRENTI,
            CURRENTGETERROR = 200,
            CURRENTGETERRORSUM,
            VELOCITYGETERROR = 228,
            VELOCITYGETERRORSUM,
            VELOCITYP = 234,
            VELOCITYI
        };

        
        /* Move Parameter */
        enum TypesMove {
            ABSOLUTE,
            RELATIVE
        };

        /* Constructor */

        TMCM16XX();

        /* Decode Functions */

        const int* decodeReceive(const char*);

        /* Command Functions */

        const unsigned char* initCommand(Commands, int, int);

        /* Parameter Functions */

        const unsigned char* setAxisParameter(Types, int);
        const unsigned char* getAxisParameter(Types);
        const unsigned char* saveAxisParameter(Types);
        const unsigned char* loadAxisParameter(Types);
        const unsigned char* setGlobalParameter(Types, int);
        const unsigned char* getGlobalParameter(Types);
        const unsigned char* saveGlobalParameter(Types);
        const unsigned char* loadGlobalParameter(Types);

        /* Move Functions */

        const unsigned char* setMoveRotateRight(int);
        const unsigned char* setMoveRotateLeft(int);
        const unsigned char* setMoveAbs(int);
        const unsigned char* setMoveRel(int);
        const unsigned char* setStop();
};

#endif