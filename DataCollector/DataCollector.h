
// DataCollector.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDataCollectorApp:
// �� Ŭ������ ������ ���ؼ��� DataCollector.cpp�� �����Ͻʽÿ�.
//

class CDataCollectorApp : public CWinApp
{
public:
	CDataCollectorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDataCollectorApp theApp;