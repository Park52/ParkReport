
// ParkReport_Viewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CParkReportViewerApp:
// See ParkReport_Viewer.cpp for the implementation of this class
//

class CParkReportViewerApp : public CWinApp
{
public:
	CParkReportViewerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CParkReportViewerApp theApp;
