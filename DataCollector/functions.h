
#pragma once

#include "cantalker.h"

CAN_FRAME packetFrame(int canID, int infoBit, int frame, int startAddress, int endAddress);
char xtod(char c);
int HextoDec(char *hex); 
int xstrtoi(char *hex);
CString GetLabName(void);
int DBConnect(MYSQL *sql, MYSQL **con);
