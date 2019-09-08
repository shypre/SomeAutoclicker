// ClickingThread.cpp : implementation file
//

#include "framework.h"
#include "SomeAutoclickerDlg.h"
#include "ClickingThread.h"
#include <string>
#include <cstring>


// CClickingThread

IMPLEMENT_DYNCREATE(CClickingThread, CWinThread)

CClickingThread::CClickingThread()
    : ClickData(nullptr)
    , main_window_hwnd(NULL)
{
    //AfxMessageBox(_T("test1"));
    //RegisterHotKey();
}


CClickingThread::~CClickingThread()
{
}


BOOL CClickingThread::InitInstance()
{
    
    return TRUE;
}


//Thread only exits when app exits
int CClickingThread::ExitInstance()
{
    return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CClickingThread, CWinThread)
    ON_THREAD_MESSAGE(WM_HOTKEY, &CClickingThread::OnHotkey)
END_MESSAGE_MAP()


void CClickingThread::OnHotkey(WPARAM wParam, LPARAM lParam)
{

}