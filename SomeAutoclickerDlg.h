
// SomeAutoclickerDlg.h : header file
//

#pragma once

#include <sstream>

#include "VirtualKeys.h"
#include "ClickData.h"
#include "ClickingThread.h"


// CSomeAutoclickerDlg dialog
class CSomeAutoclickerDlg : public CDialogEx
{
// Construction
public:
	CSomeAutoclickerDlg(CWnd* pParent = nullptr);	// standard constructor
    ~CSomeAutoclickerDlg();    // added destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOMEAUTOCLICKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    //prevent escape or enter from closing the dialog window (solution from stackoverflow 29644317)
    afx_msg void OnOkOverride();
    afx_msg void OnCancelOverride();

public:
    afx_msg void OnHelpAbout();
    afx_msg void OnFileQuit();

    afx_msg void OnBnClickedButtonStartAutoclicking();
    afx_msg void OnBnClickedButtonStopAutoclicking();
    afx_msg void OnBnClickedButtonInsertBefore();
    afx_msg void OnBnClickedButtonInsertAtEnd();
    afx_msg void OnBnClickedButtonRemoveSelected();
    afx_msg void OnBnClickedButtonClearAll();
    afx_msg void OnBnClickedButtonSetStartKey();
    afx_msg void OnBnClickedButtonSetStopKey();

    afx_msg void OnBnClickedRadioRepeating();
    afx_msg void OnBnClickedRadioHeldDown();
    afx_msg void OnBnClickedRadioDown();
    afx_msg void OnBnClickedRadioUp();
    afx_msg void OnBnClickedRadioBoth();

    // database variables
    VKeys* VKeysDB;
    ClickDataStruct* ClickData;

    // control variables (corresponds to dialog elements)
    int RepeatModeRadio;
    CHotKeyCtrl StartHotkey;
    int StartHotkeyID;
    CHotKeyCtrl StopHotkey;
    int StopHotkeyID;
    //CComboBox AutokeyCombo;
    //CHotKeyCtrl AutokeyHotkey;
    CString KeyCodeEditbox;
    CString DelayValueEditbox;
    int KeyPressModeRadio;
    CListCtrl ClickSequenceListControl;
    CString OutputEditbox;

    // clickingthread stuff
    CClickingThread* pClickThread;
    HANDLE hClickThread;
    afx_msg LRESULT ChangeSelection(WPARAM wParam, LPARAM lParam);


    // helper function
    void set_all_buttons(BOOL enable);
    void insert_into_clickdata(int position);
    void insert_clickdata_row(int position, int keycode, int wait_time, bool key_down_or_up);
    void insert_display_row(int position, int keycode, int wait_time, bool key_down_or_up);

    // other variables
    std::stringstream debug_sstream;

};
