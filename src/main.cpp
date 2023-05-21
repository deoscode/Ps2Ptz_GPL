/* 
*
* Project: PS2PTZ
* Author:  Ravi Deo
* Desc:    Arduino project to control a wired RS485 PTZ camera using a PS2 controller
*          For more details see hackaday.io:
*          https://hackaday.io/project/181371-psx2ptz-ptz-camera-control-using-psx-controller
*
* History: v1.10 14-Jan-22 Public Release 
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* <http://www.gnu.org/licenses/>
*  
*/

#define MAIN_CPP
#include "ps2ptz.h"

void WaitUntilReleased(unsigned int uiButton);
void CheckSelect();
int  WaitMenuButton();
void DisplayOption(int iOption);

void setup()
{
  pinMode(_iPin_DI, OUTPUT);

  // DE HIGH
  pinMode(_iPin_DE, OUTPUT);
  digitalWrite(_iPin_DE, 1); 

  pinMode(_iPin_RO, INPUT);

  // RE _HIGH (disable)
  pinMode(_iPin_RE, OUTPUT);
  digitalWrite(_iPin_RE, 1); 

  // OLED setup 8x8 block mode
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_8x13B_1x2_r);
  u8x8.setCursor(0, 0);
  u8x8.print("PS2PTZ " + String(VERSION));
  u8x8.drawString(0, 2, "by Ravi Deo");

  tx485.begin(9600);

  int iError = 0;
  uint8_t type = 0;

    Serial.begin(9600);
  Serial.println("Ps2Ptz " + String(VERSION));

  delay(1000);
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("Searching PSC");
  
  int x=0;
  do
  {
    u8x8.drawString(x%12, 2, "*");
    delay(1000);

    //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?)
    iError = ps2x.config_gamepad(_iPin_SCK, _iPin_MOSI, _iPin_SS, _iPin_MISO, true, true);
    if (iError == 1) {
      u8x8.drawString(x%12, 2, " ");
      Serial.println("No controller found, retry in a sec");   
    }
    x++;

  } while (iError == 1); // retry loop
  
  if (iError == 0)
    Serial.println("Found Controller, configured successful");
  else if (iError == 2)
  {
    Serial.println("Controller found but not accepting commands");
    exit(0);
  }
  else if (iError == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch (type)
  {
  case 0:
    Serial.println("Unknown Controller type");
    exit(0);
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    exit(0);
    break;
  }
  
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("Found PSC");

}

void loop()
{
  bool bActive = false;

  ps2x.read_gamepad(false, false); //read controller and dont 'vibrate'

  ca.sAction = "";
  ca.ubByte3 = 0;
  ca.ubByte4 = 0;
  ca.ubByte5 = 0;
  ca.ubByte6 = 0;

  bActive = CheckPtz();
  
  if (bActive)
  {
    if (ca.sAction != ca.sLastAction) {
      ca.Transmit();
      ca.bHoldStop=false;
    }
  }
  else if (!ca.bHoldStop)
  {
    // axis in idle state
    ca.sAction = csStop;

    ca.ulTimeUp = 0;
    ca.ulTimeDown = 0;
    ca.ulTimeLeft = 0;
    ca.ulTimeRight = 0;

    if (ca.sAction != ca.sLastAction)
      ca.Transmit(0, 0, 0, 0, ca.sAction);
  }

  ca.ubByte3 = 0;
  ca.ubByte4 = 0;
  ca.ubByte5 = 0;
  ca.ubByte6 = 0;

  delay(50); // essential, give PS2 time to breathe before next read!
}


void WaitUntilReleased(unsigned int uiButton)
{
    while (ps2x.Button(uiButton))
    {
      delay(50);
      ps2x.read_gamepad(false, false);
    } 
}
