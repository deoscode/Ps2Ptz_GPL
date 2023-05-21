#include "ps2ptz.h"

CA::CA()
{
    ubCamId=1;
    ubPreset = 1;

    // RS485 data unsigned bytes
    ubByte3 = 0;
    ubByte4 = 0;
    ubByte5 = 0;
    ubByte6 = 0;

    // timers for hold logic
    ulTimeUp = 0;
    ulTimeDown = 0;
    ulTimeLeft = 0;
    ulTimeRight = 0;


    sAction = "";
    sLastAction="";

    bHoldStop=false;

    iBaud=0;
    bBleep=true;

}

void CA::Transmit()
{
    Transmit(ubByte3, ubByte4, ubByte5, ubByte6, sAction);
}

void CA::Transmit(byte ubCmd1, byte ubCmd2, byte ubData1, byte ubData2, String sAction)
{
  sLastAction = sAction;

  u8x8.clear();
  u8x8.drawString(0, 0, sAction.c_str());

  Serial.println(sAction);

  // send the action
  unsigned int modSum = 0;
  byte dataVal[6] = {0xFF, ca.ubCamId, ubCmd1, ubCmd2, ubData1, ubData2};

  char sHex[30] = "";

  for (int i = 0; i < 6; i++)
  {
    tx485.write(dataVal[i]); //10 = uint8_t
    if (i > 0)               //dont add in the SYNC uint8_t to calculate the check sum
      modSum += dataVal[i];
  }
  modSum = modSum % 256;
  tx485.write((byte)modSum);

  for (int i = 0; i < 2; i++)
  {
    sprintf(&sHex[i * 3], "%02X,", dataVal[i]);
  }
  u8x8.drawString(10, 0, sHex);

  for (int i = 2; i < 6; i++)
  {
    sprintf(&sHex[i * 3], "%02X,", dataVal[i]);
  }
  sprintf(&sHex[6 * 3], "%02X", modSum);
  u8x8.drawString(0, 2, &sHex[6]);

  Serial.print("-->");
  Serial.println(sHex);

  //delay(100);
}