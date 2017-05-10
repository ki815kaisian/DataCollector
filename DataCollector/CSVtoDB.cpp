#include "stdafx.h"

UINT CSVtoDB(LPVOID lParam)
{
	CStdioFile CsvToDB;
	CSVINFO* CsvParam = (CSVINFO*)lParam;

	int cnt;
	int FileNumber = CsvParam->FileNum;
	FILEMNG *CsvFile = CsvParam->File;
	CString readBuf;
	CString tmpQuery;
	CString tmpVal;
	CString tempStr[6];
	CString tmpCnt;
	int colSize;
	
	for(cnt=0;cnt<FileNumber;cnt++){
		while(CsvFile[cnt].writeFlag!=1);
		if(CsvFile[cnt].writeFlag==1){
			CsvToDB.Open(TEXT(CsvFile[cnt].csvFileName),CFile::modeRead);
			while(CsvToDB.ReadString(readBuf)){
				tmpCnt.Format(readBuf);
				colSize = tmpCnt.Remove(',')/6;
				for(int i=0;i<colSize;i++){
					if(i==0)tmpQuery.Format("insert into rtfm.memorydata (address, val, frame, labid, updated, section) values ");
					else tmpQuery.Append(",");
					for(int j=0;j<6;j++){
						AfxExtractSubString(tempStr[j], readBuf, i*6+j, ',');
					}
					tmpVal.Format("(%d, %d, %d, %d, %d, %d)",_ttoi(tempStr[0]),_ttoi(tempStr[1]),_ttoi(tempStr[2]),_ttoi(tempStr[3]),_ttoi(tempStr[4]),_ttoi(tempStr[5]));
					tmpQuery.Append(tmpVal);
				}
				tmpQuery.Append(";");
				mysql_query(connection, tmpQuery);
			}
			CsvToDB.Close();
			CsvFile[cnt].writeFlag=0;
		}
	}
	AfxMessageBox("Data Capture Complete");
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	return 0;
}