// ################################################################################
// #                                                                              #
// #                         TMCM 16XX BLDC DRIVER LIBRARY                        #
// #                                      BY                                      #
// #                                   GELUDWIG                                   #
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
        /* Command list */
        enum Command {
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
        
        /* Move Parameter */
        enum Mvp {
            ABS,
            REL
        };

        /* Module Parameter */
        enum ModuleParameter {

        };

        /* Encoder parameter */
        enum EncoderParameter {

        };

        /* Current Parameter */
        enum CurrentParameter {
            CURRENTMAX = 6,
            CURRENTD = 134,
            CURRENTGETACTUAL = 150,
            CURRENTTARGET = 155,
            CURRENTP = 172,
            CURRENTI,
            CURRENTGETERROR = 200,
            CURRENTGETERRORSUM
        };

        /* Velocity Parameter */
        enum VelocityParameter {
            VELOCITYTARGET = 2,
            VELOCITYGETACTUAL,
            VELOCITYRAMPMAX,
            VELOCITYHALTFLAG = 9,
            VELOCITYACCELERATION = 11,
            VELOCITYGETRAMPACTUAL = 13,
            VELOCITYD = 133,
            VELOCITYRAMPENABLE = 146,
            VELOCITYGETERROR = 228,
            VELOCITYGETERRORSUM,
            VELOCITYP = 234,
            VELOCITYI
        };

        /* Position Parameter */
        enum PositionParameter {

        };

        /* Status Parameter */
        enum StatusParameter {

        };

        /* Constructor */

        TMCM16XX();

        /* Decode Functions */

        const int* decodeReceive(const char*);

        /* Parameter Functions */

        const unsigned char* setAxisParameter(int, int);
        const unsigned char* getAxisParameter(int);
        const unsigned char* saveAxisParameter(int);
        const unsigned char* loadAxisParameter(int);
        const unsigned char* setGlobalParameter(int, int);
        const unsigned char* getGlobalParameter(int);
        const unsigned char* saveGlobalParameter(int);
        const unsigned char* loadGlobalParameter(int);

        /* Move Functions */

        const unsigned char* setMoveRotateRight(int);
        const unsigned char* setMoveRotateLeft(int);
        const unsigned char* setMoveAbs(int);
        const unsigned char* setMoveRel(int);
        const unsigned char* setStop();

        /* Current Functions */

        const unsigned char* setCurrentParameter(CurrentParameter, int);
        const unsigned char* getCurrentParameter(CurrentParameter);

        /* Velocity Functions */

        const unsigned char* setVelocityParameter(VelocityParameter, int);
        const unsigned char* getVelocityParameter(VelocityParameter);
};

#endif