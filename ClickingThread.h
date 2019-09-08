
#pragma once

#include "ClickData.h"

//Custom messages and IDs
#define UWM_CHANGESELECTION WM_USER+301

// CClickingThread

class CClickingThread : public CWinThread
{
    DECLARE_DYNCREATE(CClickingThread)

protected:
    CClickingThread();           // protected constructor used by dynamic creation
    virtual ~CClickingThread();

    //TODO: declare and handle messages from UI thread to start and stop clicking
    //four messages in total, start and stop key, down and up, handle depending on repeat mode radio

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    HWND main_window_hwnd;
    ClickDataStruct* ClickData;

    afx_msg void CClickingThread::OnHotkey(WPARAM wParam, LPARAM lParam);

protected:
    DECLARE_MESSAGE_MAP()
};
