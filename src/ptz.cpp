#include "ps2ptz.h"

bool CheckLeftAxis();
bool CheckRightAxis();


// constants
const byte cbMidPoint = 128;
const byte cbTolerance = 32;

const byte ubSpeedSlow = 0x25;
const byte ubSpeedFast = 0xFF;

unsigned long ulTurboDelay = 2000;

bool CheckPtz()
{
  bool bActive = CheckLeftAxis();
  if (!bActive)
    bActive = CheckRightAxis();
  return bActive;
}

bool CheckLeftAxis()
{
  bool bActive = false;

  unsigned long ulTime = millis();

  byte iX = ps2x.Analog(PSS_LX);
  byte iY = ps2x.Analog(PSS_LY);

  if (ps2x.Button(PSB_PAD_UP) || (iY < (cbMidPoint - cbTolerance)))
  {
    bActive = true;
    ca.ulTimeDown = 0;

    // UP: 0x00, 0x08, HorzSpeed, VertSpeed
    ca.ubByte4 |= 0x08;
    ca.ubByte6 = ubSpeedSlow;

    if (ca.ulTimeUp == 0)
    {
      // first press, start timer
      ca.ulTimeUp = ulTime;
      ca.sAction += "U,";
    }
    else if ((ulTime - ca.ulTimeUp) > ulTurboDelay)
    {
      // beyond delay
      ca.ubByte6 = ubSpeedFast;
      ca.sAction += "U+,";
    }
    else
    {
      // within delay
      ca.sAction += "U,";
    }
  }
  else if (ps2x.Button(PSB_PAD_DOWN) || (iY > (cbMidPoint + cbTolerance)))
  {
    bActive = true;
    ca.ulTimeUp = 0;

    // DOWN: 0x00, 0x100, HorzSpeed, VertSpeed
    ca.ubByte4 |= 0x10;
    ca.ubByte6 = ubSpeedSlow;

    if (ca.ulTimeDown == 0)
    {
      // start timer
      ca.ulTimeDown = ulTime;
      ca.sAction += "D,";
    }
    else if ((ulTime - ca.ulTimeDown) > ulTurboDelay)
    {
      // beyond delay
      ca.ubByte6 = ubSpeedFast;
      ca.sAction += "D+,";
    }
    else
    {
      // within delay
      ca.sAction += "D,";
    }
  }

  if (ps2x.Button(PSB_PAD_LEFT) || (iX < (cbMidPoint - cbTolerance)))
  {
    bActive = true;
    ca.ulTimeRight = 0;

    // LEFT: 0x00, 0x04, HorzSpeed, VertSpeed
    ca.ubByte4 |= 0x04;
    ca.ubByte5 = ubSpeedSlow;

    if (ca.ulTimeLeft == 0)
    {
      // start timer
      ca.ulTimeLeft = ulTime;
      ca.sAction += "L,";
    }
    else if ((ulTime - ca.ulTimeLeft) > ulTurboDelay)
    {
      // beyond delay
      ca.ubByte5 = ubSpeedFast;
      ca.sAction += "L+,";
    }
    else
    {
      ca.sAction += "L,";
    }
  }
  else if (ps2x.Button(PSB_PAD_RIGHT) || (iX > (cbMidPoint + cbTolerance)))
  {
    bActive = true;
    ca.ulTimeLeft = 0;

    // RIGHT: 0x00, 0x02, HorzSpeed, VertSpeed
    ca.ubByte4 |= 0x02;
    ca.ubByte5 = ubSpeedSlow;

    if (ca.ulTimeRight == 0)
    {
      // start timer
      ca.ulTimeRight = ulTime;
      ca.sAction += "R,";
    }
    else if ((ulTime - ca.ulTimeRight) > ulTurboDelay)
    {
      // beyond delay
      ca.ubByte5 = ubSpeedFast;
      ca.sAction += "R+,";
    }
    else
    {
      ca.sAction += "R,";
    }
  }

  return bActive;
}

bool CheckRightAxis()
{
  byte iX = ps2x.Analog(PSS_RX);
  byte iY = ps2x.Analog(PSS_RY);

  bool bActive = false;

  if (ps2x.Button(PSB_RED) || (iX > (cbMidPoint + cbTolerance)))
  {
    bActive = true;
    ca.sAction += "F+,";
    ca.ubByte3 |= 0x01;
    ca.ubByte4 = 0;
  }
  else if (ps2x.Button(PSB_PINK) || (iX < (cbMidPoint - cbTolerance)))
  {
    bActive = true;
    ca.sAction += "F-,";
    ca.ubByte3 = 0;
    ca.ubByte4 |= 0x80;
  }

  if (ps2x.Button(PSB_BLUE) || (iY > (cbMidPoint + cbTolerance)))
  {
    bActive = true;
    ca.sAction += "Z-,";
    ca.ubByte3 = 0;
    ca.ubByte4 |= 0x40;
  }
  else if (ps2x.Button(PSB_GREEN) || (iY < (cbMidPoint - cbTolerance)))
  {
    bActive = true;
    ca.sAction += "Z+,";
    ca.ubByte3 = 0;
    ca.ubByte4 |= 0x20;
  }

  return bActive;
}
