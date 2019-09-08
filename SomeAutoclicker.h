
// SomeAutoclicker.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSomeAutoclickerApp:
// See SomeAutoclicker.cpp for the implementation of this class
//

class CSomeAutoclickerApp : public CWinApp
{
public:
	CSomeAutoclickerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSomeAutoclickerApp theApp;
