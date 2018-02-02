
// DataCollectorDlg.h : ��� ����
//

#pragma once


// CDataCollectorDlg ��ȭ ����
class CDataCollectorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDataCollectorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DATACOLLECTOR_DIALOG };
	CListBox	m_lstCan;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	HWND hWnd;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBtnInit();
	afx_msg void OnClose();
	afx_msg void OnBtnSendData();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnClear();
	afx_msg int CanInit();
	afx_msg int DBInit();
	afx_msg void OnBnClickedCheckLab();
	afx_msg void OnBnClickedCheckTxID();
	afx_msg void OnBnClickedCheckRxID();

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void AddNodeSendData();

public:

	CPtrList addNodeList;
	
	LPTSTR LpCmdLine;
	int TxID;
	int RxID;
	
	int FrameLimit;
	int DataPacket;

	CString LabName;
	int LabId;
	int WorkSpaceId;
	int TestCaseId;
	int Cascading;
	int EndAddress;
	CAN_FRAME txPacket;
	SECINFO SectionInfo[33];

	BOOL AddNodeFlag;
	int SumCascading;
	//CString AddLabName;
	//int AddLabId;
	//int AddWorkSpaceId;
	//int AddTestCaseId;
	//int AddCascading;
	//int AddStartAddress;
	//int AddEndAddress;
	//CAN_FRAME addPacket;
	//SECINFO AddSectionInfo[33];

	struct addNode {
		CString AddLabName;
		int AddLabId;
		int AddWorkSpaceId;
		int AddTestCaseId;
		int AddCascading;
		int AddStartAddress;
		int AddEndAddress;
		CAN_FRAME addPacket;
		SECINFO AddSectionInfo[33];
	};

	CFile DataCollect;
	CString FileName;
	//CFile StackPointer;
	//CString SPFileName;

	CStdioFile CsvToDB;
	

	int NameCnt;
	int SPFlag;
	FILEMNG *CsvFile;
	BYTE *TmpDumpVal;

	CWinThread* toDBThread;

	int GetSectionInfo(CString workSpaceName);
	afx_msg void OnBnClickedBtnGetAddnode();
	afx_msg void OnBnClickedCheckPacket();
};


