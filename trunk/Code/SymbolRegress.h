// SymbolRegress.h : main header file for the SymbolRegress application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSymbolRegressApp:
// See SymbolRegress.cpp for the implementation of this class
//

class CSymbolRegressApp : public CWinApp
{
public:
	CSymbolRegressApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSymbolRegressApp theApp;