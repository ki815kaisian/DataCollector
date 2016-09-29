/*
 * D&K ICT
 * http://www.dnkict.com
 * CANTalker DLL Header FIle
 */
 
#ifndef __CANTALKER_H__
#define __CANTALKER_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* to support VB60 */
#define CC      __stdcall   

#ifdef CANTALKER_EXPORTS
#define DLL_API //__declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#pragma pack(1)

#ifndef __TYPE_H__
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;

typedef enum
{
    ERR_OK = 0,
    ERR_DLL_NOT_LOADED = -1,
    ERR_DLL_NOT_EXIST = -2,
    ERR_DLL_LOAD_FAILED = -3,
    ERR_DRIVER_LOAD_FAILED = -4,
    ERR_DRIVER_NOT_INITIALIZED = -5,
    ERR_DEVICE_DISCONNECTED = -6,
    ERR_INVALID_PARAMETER = -7,
    ERR_NOT_ENOUGH_MEMORY = -8,
    ERR_GENERAL_FAILURE = -9,
    ERR_TIMEOUT = -10,
    ERR_ALREADY_DRIVER_OPENED = -11
}
ERR_CODE;

typedef struct
{
    UINT16  port;
    UINT8   ipMode;
    UINT8   ipAddr[4];
    UINT8   subnetMask[4];
    UINT8   gateway[4];
    UINT8   macAddr[6];
}
TCPIP_INFO;
#endif /* __TYPE_H__ */

typedef struct
{
    UINT32      timeStamp;
    UINT32      canId;
    BYTE        dlc;
    BYTE        data[8];
}
CAN_FRAME;

typedef struct
{
    UINT8       modelName[8];
    UINT8       serialNo[8];
}
DEVICE_INFO;

typedef struct
{
    UINT32      mask[2];
    UINT32      filter[6];
}
FILTER_INFO;

typedef struct
{
    UINT8       brgcon[3];
}
BRGCON;

typedef struct
{
    UINT32      canId;    
    UINT8       status;    
    UINT8       brgcon[3];
}
CAN_STATUS;

#pragma pack()

/* basic dll & communiation functions */
DLL_API int CC DNK_InitDLL (void);
DLL_API int CC DNK_GetDeviceCount (void);
DLL_API int CC DNK_InitUSBDriver (int DeviceHandle);
DLL_API int CC DNK_InitRS232Driver (int portNo, int buadRate);    
DLL_API int CC DNK_CloseDriver (void);
DLL_API int CC DNK_CloseDLL (void);

/* Converter & Analyzer functions */
DLL_API int CC DNK_SetCanInit (void);
DLL_API int CC DNK_SendCanData (CAN_FRAME *canFrame);
DLL_API int CC DNK_GetTotalRcvCanDataCount (void);
DLL_API int CC DNK_ClearRcvCanaData (void);
DLL_API int CC DNK_GetRcvCanData (CAN_FRAME *can)    ;
DLL_API int CC DNK_SetClearCanError (void);
DLL_API int CC DNK_ReqCanStatus (CAN_STATUS *status);
DLL_API int CC DNK_ReqErrorCount  (UINT8 *txErrCnt, UINT8 *rxErrCnt);
DLL_API int CC DNK_SetCanID (BYTE extMode, UINT32 id);
DLL_API int CC DNK_SetCanBaudRate (UINT32 brate);
DLL_API int CC DNK_ReqDeviceInfo (DEVICE_INFO *devInfo);
DLL_API int CC DNK_SetRcvMode (UINT32 mode);
DLL_API int CC DNK_ReqRcvMode (UINT32 *rcvMode);
DLL_API int CC DNK_ReqCanFilters (FILTER_INFO *filterInfo);
DLL_API int CC DNK_ReqVersion (UINT8 *major, UINT8 *minor);
DLL_API int CC DNK_ReqSpeedInfo (UINT32 *canSpeed, UINT8 *rs232SpeedIdx);
DLL_API int CC DNK_ReqTcpIpInfo (TCPIP_INFO *pInfo);
DLL_API int CC DNK_ReqCurTimeStamp (UINT32 *timeStamp);
DLL_API int CC DNK_SetCanBitTiming (BRGCON *brgcon);
DLL_API int CC DNK_SetFilters (FILTER_INFO *filterInfo);
DLL_API int CC DNK_SetTcpIpInfo (TCPIP_INFO *pInfo);
DLL_API int CC DNK_SeRs232Baudrate (UINT8 baudrate);
DLL_API int CC DNK_CloseC2EConnection (void);   

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif


#endif //__CANTALKER_H__

