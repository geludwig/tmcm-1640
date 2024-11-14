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

#include <stdint.h>

class TMCM16XX {
    private:
        uint8_t cmd[9];
        int32_t rec[9];
        void calcValueChecksum(int32_t value);

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
            POSITIONACTUAL = 1,
            VELOCITYTARGET,
            VELOCITYGETACTUAL,
            VELOCITYRAMPMAX,
            CURRENTMAX = 6,
            VELOCITYHALTFLAG = 9,
            VELOCITYACCELERATION = 11,
            VELOCITYGETRAMPACTUAL = 13,
            RS485BAUDRATE = 65,
            RS485PAUSE = 75,
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
        void setSerialReceiveId(uint8_t);

        /* Decode Functions */

        const int32_t* decodeReceive(const uint8_t*);

        /* Command Functions */

        const uint8_t* initCommand(Commands, int32_t, int32_t);

        /* Parameter Functions */

        const uint8_t* setAxisParameter(Types, int32_t);
        const uint8_t* getAxisParameter(Types);
        const uint8_t* saveAxisParameter(Types);
        const uint8_t* loadAxisParameter(Types);
        const uint8_t* setGlobalParameter(Types, int32_t);
        const uint8_t* getGlobalParameter(Types);
        const uint8_t* saveGlobalParameter(Types);
        const uint8_t* loadGlobalParameter(Types);

        /* Move Functions */

        const uint8_t* setMoveRotateRight(int32_t);
        const uint8_t* setMoveRotateLeft(int32_t);
        const uint8_t* setMoveAbs(int32_t);
        const uint8_t* setMoveRel(int32_t);
        const uint8_t* setStop();
};

#endif