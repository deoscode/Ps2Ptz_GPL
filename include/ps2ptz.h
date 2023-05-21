#include "Arduino.h"
#include <PS2X_lib.h> //for v1.6
#include <U8x8lib.h>
#include <AvrSerTx.h>
#include "CA.h"


#define VERSION "v1.10"

const byte ubOsdPreset = 70;
const String csStop = "Stop";

// logical pin assignments
const int _iPin_SS = 10;   // D10
const int _iPin_MOSI = 11; // D11
const int _iPin_MISO = 12; // D12
const int _iPin_SCK = 13;  // D13

const int _iPin_DI = 14; // A0
const int _iPin_DE = 15; // A1
const int _iPin_RE = 16; // A2
const int _iPin_RO = 17; // A3


#ifdef MAIN_CPP
// global objects
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(U8X8_PIN_NONE);
PS2X ps2x;                // PS2 Controller 
AvrSerTx tx485(_iPin_DI); // Software Serial output for RS485 data

CA ca;
#else

// global objects
extern U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8;
extern PS2X ps2x;                // PS2 Controller 
extern AvrSerTx tx485; // Software Serial output for RS485 data

extern CA ca;

#endif


// function prototypes
bool CheckPtz();


