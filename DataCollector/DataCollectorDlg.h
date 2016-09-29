
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
	afx_msg void OnBnClickedCheck1();
	afx_msg int GetSectionInfo(CString workSpaceName);

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	LPTSTR LpCmdLine;
	int TxID;
	int RxID;
	
	int EndAddress;
	int FrameLimit;
	int DataPacket;
	CString LabName;
	int LabId;
	int WorkSpaceId;
	int TestCaseId;
	int Cascading;
	CFile DataCollect;
	CString FileName;
	CFile StackPointer;
	CString SPFileName;

	CStdioFile CsvToDB;
	

	int NameCnt;
	int SPFlag;
	SECINFO SectionInfo[33];
	FILEMNG *CsvFile;
	BYTE *TmpDumpVal;

	CWinThread* toDBThread;
};


