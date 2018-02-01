// AdditionalNode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DataCollector.h"
#include "DataCollectorDlg.h"
#include "AdditionalNodeDlg.h"
#include "afxdialogex.h"


// CAdditionalNode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAdditionalNodeDlg, CDialogEx)

CAdditionalNodeDlg::CAdditionalNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDNODE_DIALOG, pParent)
{

}

CAdditionalNodeDlg::~CAdditionalNodeDlg()
{
}

void CAdditionalNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdditionalNodeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CAdditionalNodeDlg::OnBnClickedBtnCheck)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CAdditionalNodeDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_SET_ADDNODE, &CAdditionalNodeDlg::OnBnClickedBtnSetAddnode)
END_MESSAGE_MAP()


// CAdditionalNode 메시지 처리기입니다.


void CAdditionalNodeDlg::OnBnClickedBtnCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString wsName;//WorkSpace
	CString tcName;//TestCase
	CString query;

	GetDlgItemText(IDC_EDIT_TC_ADD, tcName);
	if (tcName.GetLength() == 0) {
		AfxMessageBox("Check Test Case Name");
		return;
	}
	else {
		query.Format("SELECT id FROM rtfm.testcasename where testcasename = ('%s')", tcName);
		mysql_query(connection, query);
		m_res = mysql_store_result(&mysql);
		if ((row = mysql_fetch_row(m_res)) == NULL) {
			AfxMessageBox("Check Test Case Name");
			return;
		}
		else TestCaseId = _ttoi(row[0]);
	}

	GetDlgItemText(IDC_EDIT_WS_ADD, wsName);
	if (wsName.GetLength() == 0) {
		AfxMessageBox("Check Work Space Name");
		return;
	}
	else {
		if (strlen(wsName)) {
			if (GetSectionInfo(wsName))return;
		}
		else {
			AfxMessageBox("Check Work Space Name");
			return;
		}
	}

	GetDlgItem(IDC_EDIT_WS_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TC_ADD)->EnableWindow(FALSE);

	return;
}


void CAdditionalNodeDlg::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


int CAdditionalNodeDlg::GetSectionInfo(CString workSpaceName)
{
	CString query;
	CString tmpAddr;
	CString tmpBase;
	CString tmpSize;
	char tmpAddrArray[10];
	int tmpAddress;
	int workSpaceId;

	query.Format("SELECT id FROM rtfm.workspacename where workspacename = ('%s')", workSpaceName);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if ((row = mysql_fetch_row(m_res)) == NULL) {
		AfxMessageBox("Check Work Space Name");
		return 1;
	}
	else  workSpaceId = _ttoi(row[0]);

	this->WorkSpaceId = workSpaceId;
	int sectionIndex = 0;
	query.Format("SELECT count(*) FROM rtfm.sectiontable where workspace_id = '%d';", workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if ((row = mysql_fetch_row(m_res)) == NULL) {
		AfxMessageBox("Check Section Table");
		return 1;
	}
	else sectionIndex = _ttoi(row[0]);

	for (int i = 0; i<10; i++)tmpAddrArray[i] = 0;
	query.Format("SELECT section_index, section, baseaddress, size FROM rtfm.sectiontable where workspace_id = '%d';", workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	for (int i = 0; i<sectionIndex; i++) {
		if (!(row = mysql_fetch_row(m_res)) == NULL) {
			((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].sectionID = _ttoi(row[0]);
			((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].sectionName = row[1];
			tmpAddr = row[2];
			((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].size = _ttoi(row[3]);
			tmpAddr.Delete(0, 4);
			strcpy_s(tmpAddrArray, tmpAddr.GetLength() + 1, (const char*)tmpAddr.GetBuffer(0));
			((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].startAddr = xstrtoi(tmpAddrArray);
			((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].endAddr = xstrtoi(tmpAddrArray) + ((CDataCollectorDlg*)GetParent())->AddSectionInfo[i].size;
		}
	}

	for (int i = 0; i<10; i++)tmpAddrArray[i] = 0;
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
	SetDlgItemText(IDC_EDIT_Start_ADD, tmpBase);//startAddress

	for (int i = 0; i<10; i++)tmpAddrArray[i] = 0;
	query.Format("SELECT baseaddress, size FROM rtfm.sectiontable where workspace_id = '%d' and section_index = (SELECT max(section_index) from rtfm.sectiontable where workspace_id = '%d');", workSpaceId, workSpaceId);
	mysql_query(connection, query);
	m_res = mysql_store_result(&mysql);
	if (!(row = mysql_fetch_row(m_res)) == NULL) {
		tmpAddr = row[0];
		tmpSize = row[1];
		tmpAddr.Delete(0, 4);
		strcpy_s(tmpAddrArray, tmpAddr.GetLength() + 1, (const char*)tmpAddr.GetBuffer(0));
		tmpAddress = xstrtoi(tmpAddrArray) + _ttoi(tmpSize) - 1;
		tmpBase.Format("%x", tmpAddress);
	}
	else tmpBase.Format("");

	SetDlgItemText(IDC_EDIT_End_ADD, tmpBase);//EndAddress

	return 0;
}


void CAdditionalNodeDlg::OnBnClickedBtnSetAddnode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmpTxID;
	GetDlgItemText(IDC_EDIT_Tx_ADD, tmpTxID);
	CString tmpStart;
	GetDlgItemText(IDC_EDIT_Start_ADD, tmpStart);
	CString tmpEnd;
	GetDlgItemText(IDC_EDIT_End_ADD, tmpEnd);

	TxID = xstrtoi((LPSTR)(LPCSTR)tmpTxID);
	StartAddress = xstrtoi((LPSTR)(LPCSTR)tmpStart);
	EndAddress = xstrtoi((LPSTR)(LPCSTR)tmpEnd);

	CDialogEx::OnOK();
}
