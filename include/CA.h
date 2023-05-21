#include <U8x8lib.h>

class CA {
    public: 
        byte ubCamId; 
        byte ubPreset;

        // RS485 data unsigned bytes
        byte ubByte3;
        byte ubByte4;
        byte ubByte5;
        byte ubByte6;

        // timers for hold logic
        unsigned long ulTimeUp;
        unsigned long ulTimeDown;
        unsigned long ulTimeLeft;
        unsigned long ulTimeRight;

        String sAction;
        String sLastAction;

        bool bHoldStop;

        int iMode;
        int iBaud;
        bool bBleep;

    public:
        CA();

        void Transmit(byte ubCmd1, byte ubCmd2, byte ubData1, byte ubData2, String sAction);
        void Transmit();
};