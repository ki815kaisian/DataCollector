#pragma once


// CAdditionalNode ��ȭ �����Դϴ�.

class CAdditionalNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdditionalNodeDlg)

public:
	CAdditionalNodeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAdditionalNodeDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDNODE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
