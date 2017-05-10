#include "stdafx.h"
#include "functions.h"



CAN_FRAME packetFrame(int canID, int infoBit, int frame, int startAddress, int endAddress){
	CAN_FRAME txPacket;
	txPacket.canId = canID;
	txPacket.dlc = 8;
	txPacket.data[0] = (infoBit >> 8) & 0xFF;
	txPacket.data[1] = infoBit & 0xFF;
	txPacket.data[2] = (frame >> 8) & 0xFF;
	txPacket.data[3] = frame & 0xFF;
	txPacket.data[4] = (startAddress >> 8) & 0xFF;
	txPacket.data[5] = startAddress & 0xFF;
	txPacket.data[6] = (endAddress >> 8) & 0xFF;
	txPacket.data[7] = endAddress & 0xFF;

	return txPacket;
}

char xtod(char c) {   
	if (c>='0' && c<='9') return c-'0';   
	if (c>='A' && c<='F') return c-'A'+10;   
	if (c>='a' && c<='f') return c-'a'+10;   
	return c=0;        // not Hex digit   
}   

int HextoDec(char *hex)   
{   
	if (*hex==0) return 0;   
	return  HextoDec(hex-1)*16 + xtod(*hex) ;    
}   

int xstrtoi(char *hex)      // hex string to integer   
{   
	return HextoDec(hex+strlen(hex)-1);   
}   

CString GetLabName(){
	CString tmpName;
	CTime m_Date;
	CString m_strMin;
	CString m_strHour;
	CString m_strDay;
	CString m_strMonth;
	CString m_strYear;
	CTime time = CTime::GetCurrentTime();

	m_Date = time;
	m_strMin.Format("%02d",m_Date.GetMinute()); //분
	m_strHour.Format("%02d",m_Date.GetHour()); //시
	m_strDay.Format("%02d",m_Date.GetDay()); //일
	m_strMonth.Format("%02d",m_Date.GetMonth()); //월
	m_strYear.Format("%d",m_Date.GetYear()); //년
	tmpName.Format("%s%s%s%s%s",m_strYear,m_strMonth,m_strDay,m_strHour,m_strMin);

	return tmpName;

}

int DBConnect(MYSQL *sql, MYSQL **con)
{
	CStdioFile dbInfo;
	CString readBuf;

	CString DbId;
	CString DbPass;
	CString DbName;
	CString DbAddr;
	CString DbPortStr;
	unsigned int DbPortNum;
	
	if(!dbInfo.Open(TEXT(".\\info.txt"),CFile::modeRead)){
		AfxMessageBox("Check DB Info");
		return 1;
	}
	dbInfo.ReadString(readBuf);
	DbName.Format(readBuf);
	dbInfo.ReadString(readBuf);
	DbId.Format(readBuf);
	dbInfo.ReadString(readBuf);
	DbPass.Format(readBuf);
	dbInfo.ReadString(readBuf);
	AfxExtractSubString(DbAddr, readBuf, 0, ':');
	AfxExtractSubString(DbPortStr, readBuf, 1, ':');
	dbInfo.Close();

	DbPortNum = _ttoi(DbPortStr);

	mysql_init(sql);
	*con = mysql_real_connect(sql, DbAddr, DbId, DbPass, DbName, DbPortNum, NULL, 0);
	if(*con == NULL){
		CString msg;
		msg.Format("%s",mysql_error(sql));
		AfxMessageBox(msg);
		return 1;
	}
	return 0;
}
