#include "ps2ptz.h"

enum MoAction
{
    maUp=0,
    maDown,
    maLeft,
    maRight,
    maSelect
};

enum MoMain
{
    moPTZ=0,
    moSnoop,
    moBaud,
    moBleep,
    moDelAllPre,

    moEnd
};

typedef struct 
{
    String sText;
    int iItemID;        // sequential
    int iParentID;
    int iLine;          // display line
} MenuItem;

String csPressCircle="Press Circle";

MenuItem arrItem[] = {
    {"PTZ Mode",            0,0,1},
    {   csPressCircle,      1,0,2},

    {"Snoop Mode",          2,0,1},
    {   csPressCircle,      3,2,2},

    {"Settings",            4,0,1},
    {   csPressCircle,      5,4,2},

    {   "Baud Rate",        6,4,1},
    {       "2400",         7,6,2},
    {       "4800",         8,6,2},
    {       "9600",         9,6,2},
    {       "19200",        10,6,2},

    {   "Bleep",            11,4,1},
    {       "ON",           12,11,2},
    {       "OFF",          13,11,2},

    {   "Del All Pre",      14,4,1},
    {       csPressCircle,  15,14,2}   
};