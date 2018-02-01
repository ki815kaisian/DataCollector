#pragma once


// CAdditionalNode 대화 상자입니다.

class CAdditionalNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdditionalNodeDlg)

public:
	CAdditionalNodeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAdditionalNodeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDNODE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	int TxID;
	int WorkSpaceId;
	int TestCaseId;
	int StartAddress;
	int EndAddress;

	afx_msg void OnBnClickedBtnCheck();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnSetAddnode();

	int GetSectionInfo(CString workSpaceName);
};
