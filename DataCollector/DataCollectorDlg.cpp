#include "stdafx.h"
#include "DataCollector.h"
#include "DataCollectorDlg.h"
#include "afxdialogex.h"
#include <malloc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDataCollectorDlg::CDataCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataCollectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	FrameLimit = 0;
	NameCnt = 0;
	SPFlag = 0;
}

void CDataCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_RCVCAN, m_lstCan);
}

BEGIN_MESSAGE_MAP(CDataCollectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_SEND_DATA, OnBtnSendData)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)

	ON_BN_CLICKED(IDC_CHECK_LAB, &CDataCollectorDlg::OnBnClickedCheckLab)
	ON_BN_CLICKED(IDC_CHECK_TxID, &CDataCollectorDlg::OnBnClickedCheckTxID)
	ON_BN_CLICKED(IDC_CHECK_RxID, &CDataCollectorDlg::OnBnClickedCheckRxID)

END_MESSAGE_MAP()

BOOL CDataCollectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.c

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	LpCmdLine = theApp.m_lpCmdLine;

	CStdioFile canInfo;
	CString readBuf;
	CString tmpTxID;
	CString tmpRxID;
	if (!canInfo.Open(TEXT(".\\canID.txt"), CFile::modeRead)) {
		//������ ������
	}
	else {
		canInfo.ReadString(readBuf);
		tmpTxID.Format(readBuf);
		canInfo.ReadString(readBuf);
		tmpRxID.Format(readBuf);
		canInfo.Close();
		CheckDlgButton(IDC_CHECK_TxID, TRUE);
		CheckDlgButton(IDC_CHECK_RxID, TRUE);
		GetDlgItem(IDC_EDIT_Tx)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Rx)->EnableWindow(FALSE);
		SetDlgItemText(IDC_EDIT_Tx, tmpTxID);//Tx ID
		SetDlgItemText(IDC_EDIT_Rx, tmpRxID);//Rx ID
	}
	OnBtnInit();
	TmpDumpVal = (BYTE *)malloc(BANDMAX*DATASIZE*sizeof(BYTE));
	memset(TmpDumpVal,0,BANDMAX*DATASIZE*sizeof(BYTE));
	CreateDirectory(".\\data", NULL);
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDataCollectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDataCollectorDlg::OnPaint()
{
	if (IsIconic()){
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}else{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDataCollectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataCollectorDlg::OnClose() 
{
	KillTimer (0);
	DNK_CloseDriver ();
	DNK_CloseDLL ();	
	CDialog::OnClose();
}

void CDataCollectorDlg::OnTimer(UINT nIDEvent) 
{
	int infoBit = 0;
	static int frameIndex = 0;
	static CString query;
	static int valCnt = 0;
	static int frameBuf=0;
	int update=0;
	int sectionID=0;

	static int tmpQueryCnt=0;

	if (nIDEvent == 0){
		int cnt, i;
		int ret;
		CAN_FRAME rxPacket;
		CString str;
		int address;
		
		cnt = DNK_GetTotalRcvCanDataCount();
		if (cnt < ERR_OK)return CDialog::OnTimer(nIDEvent);
		else if (cnt == 0)return CDialog::OnTimer(nIDEvent);

		for (i = 0; i < cnt; i++){
			if ((ret = DNK_GetRcvCanData (&rxPacket)) < ERR_OK)return CDialog::OnTimer(nIDEvent);

			/**************My-SQL DB���ۿ� ����**************/
			/***table_new�� ����***/
			if((rxPacket.canId == RxID)){		
				address = (rxPacket.data[0] << 8) + rxPacket.data[1];
				if(!(address < INFO_BIT)){
					frameIndex  = (rxPacket.data[2] << 8) + rxPacket.data[3];
					if(frameBuf!=frameIndex){
						if(frameIndex!=0)query.Append("\n");
						frameBuf=frameIndex;
					}
					
					if(frameIndex==0){
						memset(TmpDumpVal,0,BANDMAX*DATASIZE*sizeof(BYTE));
					}
					valCnt=0;

					if(frameIndex==0&&SPFlag==0)SPFlag=1;
					else if(frameIndex==0&&SPFlag==1){
						SPFlag=2;
					}

					if(SPFlag==1){
						/*CString SPtmp;
						SPtmp.Format("%d,%02X%02X%02X%02X\n",frameIndex,rxPacket.data[4],rxPacket.data[5],rxPacket.data[6],rxPacket.data[7]);
						StackPointer.Open(TEXT(SPFileName),CFile::shareDenyNone | CFile::modeReadWrite);
						StackPointer.Seek(0L, CFile::end);
						StackPointer.Write(TEXT(SPtmp),SPtmp.GetLength());					
						StackPointer.Close();*/
					}
				}
				if((address < INFO_BIT)){
					if(frameIndex < FrameLimit){
						CString tmp;
						for(int k = 0; k < 6; k++){
							if(frameIndex>0)update = (TmpDumpVal[valCnt]==rxPacket.data[2+k])?0:1;
							else update = 0;
							TmpDumpVal[valCnt]=rxPacket.data[2+k];

							for(sectionID=0;(address + k) >= SectionInfo[sectionID].endAddr;sectionID++);
							tmp.Format("%d,%d,%d,%d,%d,%d,",address + k, rxPacket.data[2+k], frameIndex, LabId, update, sectionID);
							valCnt++;
							query.Append(tmp);
							tmpQueryCnt++;
						}
						//DataCollect.Open(TEXT(FileName),CFile::shareDenyNone | CFile::modeReadWrite);
						//DataCollect.Seek(0L, CFile::end);
						DataCollect.Write(TEXT(query),query.GetLength());
						query.Format("");
						tmpQueryCnt=0;
						//DataCollect.Close();

						if((frameIndex==FrameLimit-1)&&(NameCnt==Cascading)&&(address+6>=EndAddress)){
							DataCollect.Close();
							CsvFile[NameCnt - 1].writeFlag = 1;
						}
					}					
				}else if(address >= INFO_BIT){
					infoBit = (rxPacket.data[0] << 8) + rxPacket.data[1];
					address = 0;
					if(frameIndex==0){
						if (NameCnt > 0) {
							DataCollect.Close();
							CsvFile[NameCnt - 1].writeFlag = 1;
						}
						FileName.Format(".\\data\\"+LabName+"\\"+LabName+"_%d.csv",NameCnt++);
						sprintf(CsvFile[NameCnt-1].csvFileName, "%s", FileName);
						DataCollect.Open(TEXT(FileName),CFile::modeCreate | CFile::shareDenyNone | CFile::modeReadWrite);
						//DataCollect.Close();
					}
				}				
			}
		}
	}

	CDialog::OnTimer(nIDEvent);	
}


void CDataCollectorDlg::OnBtnSendData() 
{
	int ERR_SEND;
	CAN_FRAME txPacket;
	CString tmpTxID;
	CString tmpRxID;
	CString tmpFrame;
	CString tmpStart;
	int startAddress=0;
	CString tmpEnd;
	int endAddress=0;
	CString tmpLabName;
	CString tmpDataPacket;
	CString query;
	int queryResult=0;
	int queryCnt=0;
	
	CSVINFO thParam;

	GetDlgItemText(IDC_EDIT_Tx, tmpTxID);	
	GetDlgItemText(IDC_EDIT_Start, tmpStart);
	GetDlgItemText(IDC_EDIT_End, tmpEnd);
	GetDlgItemText(IDC_EDIT_Rx, tmpRxID);
	GetDlgItemText(IDC_EDIT_DataPacket, tmpDataPacket);
	if(IsDlgButtonChecked(IDC_CHECK_LAB))tmpLabName=GetLabName();
	else GetDlgItemText(IDC_EDIT_LAB, tmpLabName);
	SetDlgItemText(IDC_EDIT_LAB, tmpLabName);

	CStdioFile canInfo; 
	canInfo.Open(".\\canID.txt",CFile::modeCreate | CFile::shareDenyNone | CFile::modeReadWrite);
	canInfo.WriteString(tmpTxID + "\n");
	canInfo.WriteString(tmpRxID + "\n");
	canInfo.Close();

	TxID = xstrtoi((LPSTR)(LPCSTR)tmpTxID);	
	startAddress = xstrtoi((LPSTR)(LPCSTR)tmpStart);
	endAddress = xstrtoi((LPSTR)(LPCSTR)tmpEnd);
	EndAddress = endAddress;

	RxID = xstrtoi((LPSTR)(LPCSTR)tmpRxID);
	DataPacket = _ttoi(tmpDataPacket);

	Cascading = (endAddress-startAddress)/(DataPacket*DATASIZE)+1;
	CsvFile = (FILEMNG *)malloc(Cascading*sizeof(FILEMNG));

	for(queryCnt=0;queryResult==0;){
		if(queryCnt==0)query.Format("SELECT id FROM rtfm.labname where labname = ('%s')",tmpLabName);
		else query.Format("SELECT id FROM rtfm.labname where labname = ('%s%d')",tmpLabName,queryCnt);
		mysql_query(connection, query);
		if(m_res = mysql_store_result(&mysql)){
			if((row = mysql_fetch_row(m_res)) == NULL){
				if(queryCnt==0)query.Format("insert into labname (labname, startaddr, endaddr, workspace_id, testcase_id) values ('%s', %d, %d, %d, %d)",tmpLabName,startAddress,endAddress,WorkSpaceId,TestCaseId);
				else query.Format("insert into labname (labname, startaddr, endaddr, workspace_id, testcase_id) values ('%s%d', %d, %d, %d, %d)",tmpLabName,queryCnt,startAddress,endAddress,WorkSpaceId,TestCaseId);
				mysql_query(connection, query);
				queryResult=1;
			}else queryCnt++;
		}
	}
	if(queryCnt==0)query.Format("SELECT id FROM rtfm.labname where labname = ('%s')",tmpLabName);
	else query.Format("SELECT id FROM rtfm.labname where labname = ('%s%d')",tmpLabName,queryCnt);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if((row = mysql_fetch_row(m_res)) == NULL){
		AfxMessageBox ("Check DB Status");
		return;
	}
	LabId = _ttoi(row[0]);

	if(queryCnt==0)LabName = tmpLabName;
	else LabName.Format("%s%d",tmpLabName,queryCnt);
	SetDlgItemText(IDC_EDIT_LAB, LabName);
	NameCnt=0;
	SPFlag=0;
	CreateDirectory(".\\data\\"+LabName, NULL);
	/*SPFileName.Format(".\\data\\"+LabName+"\\"+LabName+"_SP.csv");
	StackPointer.Open(TEXT(SPFileName),CFile::modeCreate | CFile::shareDenyNone | CFile::modeReadWrite);
	StackPointer.Close();*/
	
	thParam.FileNum = Cascading;
	thParam.File = CsvFile;
	toDBThread = AfxBeginThread(CSVtoDB, (LPVOID)&thParam, 0, 0U, 0UL, (LPSECURITY_ATTRIBUTES)0);

	GetDlgItemText(IDC_EDIT_Frame, tmpFrame);
	FrameLimit = _ttoi(tmpFrame);
	txPacket = packetFrame(TxID, INFO_BIT|Cascading, FrameLimit, startAddress, endAddress);

	if((ERR_SEND = DNK_SendCanData(&txPacket)) == ERR_OK){
		CString msg;
		AfxMessageBox("Data sending Complete !!");
		msg.Format ("%d %08X %02d %02X%02X,%02X%02X,%02X%02X,%02X%02X ", txPacket.timeStamp, txPacket.canId, txPacket.dlc, txPacket.data[0],txPacket.data[1],txPacket.data[2],txPacket.data[3],txPacket.data[4],txPacket.data[5],txPacket.data[6],txPacket.data[7]);
		m_lstCan.AddString (msg);
	}else{
		CString msg;
		msg.Format("Data sending error !!\nError Code : %d", ERR_SEND);
		AfxMessageBox(msg);
	}
}

void CDataCollectorDlg::OnBtnInit() 
{
	static int initCAN=1;
	static int initDB=1;

	if(initCAN)initCAN = CanInit();
	if(initDB)initDB = DBInit();
	if(!(initCAN | initDB)){
		AfxMessageBox ("Initialization Complete");
		GetDlgItem(IDC_BTN_INIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND_DATA)->EnableWindow(TRUE);
		return;
	}
	GetDlgItem(IDC_BTN_SEND_DATA)->EnableWindow(FALSE);
}

void CDataCollectorDlg::OnBtnClear() 
{
	m_lstCan.ResetContent ();	
}

void CDataCollectorDlg::OnBnClickedCheckLab()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(IsDlgButtonChecked(IDC_CHECK_LAB))GetDlgItem(IDC_EDIT_LAB)->EnableWindow(FALSE);
	else GetDlgItem(IDC_EDIT_LAB)->EnableWindow(TRUE);
}

void CDataCollectorDlg::OnBnClickedCheckTxID()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (IsDlgButtonChecked(IDC_CHECK_TxID))GetDlgItem(IDC_EDIT_Tx)->EnableWindow(FALSE);
	else GetDlgItem(IDC_EDIT_Tx)->EnableWindow(TRUE);
}

void CDataCollectorDlg::OnBnClickedCheckRxID()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (IsDlgButtonChecked(IDC_CHECK_RxID))GetDlgItem(IDC_EDIT_Rx)->EnableWindow(FALSE);
	else GetDlgItem(IDC_EDIT_Rx)->EnableWindow(TRUE);
}

int CDataCollectorDlg::CanInit()
{
	CString tmpTxID;
	CString tmpRxID;

	GetDlgItemText(IDC_EDIT_Tx, tmpTxID);//Tx ID
	if (tmpTxID.GetLength() < 1)return 1;
	GetDlgItemText(IDC_EDIT_Rx, tmpRxID);//Rx ID
	if (tmpRxID.GetLength() < 1)return 1;

	int DLL_ERR = DNK_InitDLL();
	if (DLL_ERR == ERR_OK);	// cantalker.dll �ε�
	else{
		AfxMessageBox ("CAN DLL Initialization Failed !!");	
		return 1;
	}
	if (DNK_GetDeviceCount() > 0){	// USB
		if (DNK_InitUSBDriver(0) < ERR_OK)	// DNK_GetDeviceCount()�κ��� ����� ��ġ �� ����
		{									// DNK_InitUSBDriver�� parameter�� ���� ��
			AfxMessageBox ("CAN Driver Initialization Failed");
			DNK_CloseDriver();
			DNK_CloseDLL ();
			return 1;
		}else{
			
		
			SetTimer (0, 100, NULL);
		}
	}else{
		AfxMessageBox ("Device is not connected !!");
		DNK_CloseDLL ();
		return 1;
	}
	return 0;
}

int CDataCollectorDlg::DBInit()
{
	CString query;
	CString wsName;//WorkSpace
	CString tcName;//TestCase
	CString tmpGetLabName=GetLabName();
	SetDlgItemText(IDC_EDIT_LAB, tmpGetLabName);//LAB ID

	if(DBConnect(&mysql, &connection))return 1;

	GetDlgItemText(IDC_EDIT_WS, wsName);
	if(wsName.GetLength()==0){
		if(strlen(LpCmdLine)>0){
			AfxExtractSubString(wsName, LpCmdLine, 0, ' ');
			AfxExtractSubString(tcName, LpCmdLine, 1, ' ');
			SetDlgItemText(IDC_EDIT_WS, wsName);
			SetDlgItemText(IDC_EDIT_TC, tcName);
		}
	}
	GetDlgItemText(IDC_EDIT_TC, tcName);

	if(tcName.GetLength()==0){
		if(strlen(LpCmdLine)>0){
			AfxExtractSubString(wsName, LpCmdLine, 0, ' ');
			AfxExtractSubString(tcName, LpCmdLine, 1, ' ');
			SetDlgItemText(IDC_EDIT_WS, wsName);
			SetDlgItemText(IDC_EDIT_TC, tcName);
		}
	}
	GetDlgItemText(IDC_EDIT_TC, tcName);

	if(strlen(wsName)){
		if(GetSectionInfo(wsName))return 1;
	}else{
		AfxMessageBox("Check Work Space Name");
		return 1;
	}

	if(strlen(tcName)){
		query.Format("SELECT id FROM rtfm.testcasename where testcasename = ('%s')",tcName);
		mysql_query(connection, query);
		m_res = mysql_store_result(&mysql);
		if((row = mysql_fetch_row(m_res)) == NULL){
			AfxMessageBox("Check Test Case Name");
			return 1;
		}else TestCaseId = _ttoi(row[0]);
	}else{
		AfxMessageBox("Check Test Case Name");
		return 1;
	}
	
	SetDlgItemText(IDC_EDIT_Frame, "10");//Frame
	SetDlgItemText(IDC_EDIT_DataPacket, "15");//DataPacket

	return 0;
}

int CDataCollectorDlg::GetSectionInfo(CString workSpaceName)
{
	CString query;
	CString tmpAddr;
	CString tmpBase;
	CString tmpSize;
	char tmpAddrArray[10];
	int tmpAddress;
	int workSpaceId;

	query.Format("SELECT id FROM rtfm.workspacename where workspacename = ('%s')",workSpaceName);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if((row = mysql_fetch_row(m_res)) == NULL){
		AfxMessageBox("Check Work Space Name");
		return 1;
	}else  workSpaceId = _ttoi(row[0]);
	
	this->WorkSpaceId = workSpaceId;
	int sectionIndex=0;
	query.Format("SELECT count(*) FROM rtfm.sectiontable where workspace_id = '%d';",workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if((row = mysql_fetch_row(m_res)) == NULL){
		AfxMessageBox("Check Section Table");
		return 1;
	}else sectionIndex = _ttoi(row[0]);

	query.Format("SELECT section_index, section, baseaddress, size FROM rtfm.sectiontable where workspace_id = '%d';",workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	for(int i=0;i<sectionIndex;i++){
		if(!(row = mysql_fetch_row(m_res))==NULL){
			this->SectionInfo[i].sectionID = _ttoi(row[0]);
			this->SectionInfo[i].sectionName = row[1];
			tmpAddr = row[2];
			this->SectionInfo[i].size = _ttoi(row[3]);
			tmpAddr.Delete(0,4);
			strcpy_s(tmpAddrArray, tmpAddr.GetLength()+1, (const char*)tmpAddr.GetBuffer(0));
			this->SectionInfo[i].startAddr=xstrtoi(tmpAddrArray);
			this->SectionInfo[i].endAddr=xstrtoi(tmpAddrArray) + this->SectionInfo[i].size;
		}
	}
		
	query.Format("SELECT baseaddress FROM rtfm.sectiontable where workspace_id = '%d' and section_index = (SELECT min(section_index) from rtfm.sectiontable where workspace_id = '%d');", workSpaceId, workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if (!(row = mysql_fetch_row(m_res)) == NULL) {
		tmpAddr = row[0];
		tmpAddr.Delete(0, 4);
		strcpy_s(tmpAddrArray, tmpAddr.GetLength() + 1, (const char*)tmpAddr.GetBuffer(0));
		tmpAddress = xstrtoi(tmpAddrArray);
		tmpBase.Format("%x", tmpAddress);
	}
	else tmpBase.Format("");
	SetDlgItemText(IDC_EDIT_Start, tmpBase);//startAddress

	query.Format("SELECT baseaddress, size FROM rtfm.sectiontable where workspace_id = '%d' and section_index = (SELECT max(section_index) from rtfm.sectiontable where workspace_id = '%d');",workSpaceId, workSpaceId);
	mysql_query(connection,query);
	m_res = mysql_store_result(&mysql);
	if(!(row = mysql_fetch_row(m_res))==NULL){
		tmpAddr = row[0];
		tmpSize = row[1];
		tmpAddr.Delete(0,4);
		strcpy_s(tmpAddrArray, tmpAddr.GetLength()+1, (const char*)tmpAddr.GetBuffer(0));
		tmpAddress = xstrtoi(tmpAddrArray) + _ttoi(tmpSize);
		tmpBase.Format("%x",tmpAddress);
	}else tmpBase.Format("");

	SetDlgItemText(IDC_EDIT_End, tmpBase);//EndAddress
	
	return 0;
}
