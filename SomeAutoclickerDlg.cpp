
// SomeAutoclickerDlg.cpp : implementation file
//

#include <string>
#include <sstream>
#include <stdlib.h>


#include "framework.h"
#include "SomeAutoclicker.h"
#include "SomeAutoclickerDlg.h"
#include "afxdialogex.h"

#include "ClickData.h"
#include "ClickingThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSomeAutoclickerDlg dialog



CSomeAutoclickerDlg::CSomeAutoclickerDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SOMEAUTOCLICKER_DIALOG, pParent)
    , RepeatModeRadio(0)
    , KeyCodeEditbox(_T("1"))
    , DelayValueEditbox(_T("10"))
    , KeyPressModeRadio(0)
    //TODO: replace with unique atoms generated at runtime to avoid potential conflicts
    , StartHotkeyID(601)
    , StopHotkeyID(602)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // pointer to vkeys struct
    VKeysDB = new VKeys;
    // pointer to clickdata struct
    ClickData = new ClickDataStruct;
}

CSomeAutoclickerDlg::~CSomeAutoclickerDlg()
{
    delete VKeysDB;
    delete ClickData;
}

void CSomeAutoclickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    // associate UI controls with variables
    DDX_Text(pDX, IDC_EDIT_OUTPUT_BOX, OutputEditbox);
    DDX_Text(pDX, IDC_EDIT_KEYCODE, KeyCodeEditbox);
    DDV_MaxChars(pDX, KeyCodeEditbox, 3);
    DDX_Text(pDX, IDC_EDIT_DELAY_TIME, DelayValueEditbox);
    DDV_MaxChars(pDX, DelayValueEditbox, 7);
    DDX_Control(pDX, IDC_HOTKEY_START, StartHotkey);
    DDX_Control(pDX, IDC_HOTKEY_STOP, StopHotkey);
    DDX_Control(pDX, IDC_LIST_CLICKLIST, ClickSequenceListControl);
    //DDX_Control(pDX, IDC_COMBO_HOTKEY, AutokeyCombo);
    //DDX_Control(pDX, IDC_HOTKEY_AUTO_KEY, AutokeyHotkey);
    DDX_Radio(pDX, IDC_RADIO_BOTH, KeyPressModeRadio);
    DDX_Radio(pDX, IDC_RADIO_HELD_DOWN, RepeatModeRadio);
}

BEGIN_MESSAGE_MAP(CSomeAutoclickerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

    // associate events (messages) with handler functions
    ON_COMMAND(IDOK, &CSomeAutoclickerDlg::OnOkOverride)
    ON_COMMAND(IDCANCEL, &CSomeAutoclickerDlg::OnCancelOverride)
    ON_COMMAND(ID_HELP_ABOUT, &CSomeAutoclickerDlg::OnHelpAbout)
    ON_COMMAND(ID_FILE_QUIT, &CSomeAutoclickerDlg::OnFileQuit)
    ON_BN_CLICKED(IDC_BUTTON_START_AUTOCLICKING, &CSomeAutoclickerDlg::OnBnClickedButtonStartAutoclicking)
    ON_BN_CLICKED(IDC_BUTTON_STOP_AUTOCLICKING, &CSomeAutoclickerDlg::OnBnClickedButtonStopAutoclicking)
    ON_BN_CLICKED(IDC_BUTTON_INSERT_BEFORE, &CSomeAutoclickerDlg::OnBnClickedButtonInsertBefore)
    ON_BN_CLICKED(IDC_BUTTON_INSERT_AT_END, &CSomeAutoclickerDlg::OnBnClickedButtonInsertAtEnd)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE_SELECTED, &CSomeAutoclickerDlg::OnBnClickedButtonRemoveSelected)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CSomeAutoclickerDlg::OnBnClickedButtonClearAll)
    ON_BN_CLICKED(IDC_BUTTON_SET_START_KEY, &CSomeAutoclickerDlg::OnBnClickedButtonSetStartKey)
    ON_BN_CLICKED(IDC_BUTTON_SET_STOP_KEY, &CSomeAutoclickerDlg::OnBnClickedButtonSetStopKey)
    ON_BN_CLICKED(IDC_RADIO_REPEATING, &CSomeAutoclickerDlg::OnBnClickedRadioRepeating)
    ON_BN_CLICKED(IDC_RADIO_HELD_DOWN, &CSomeAutoclickerDlg::OnBnClickedRadioHeldDown)
    ON_BN_CLICKED(IDC_RADIO_BOTH, &CSomeAutoclickerDlg::OnBnClickedRadioBoth)
    ON_BN_CLICKED(IDC_RADIO_DOWN, &CSomeAutoclickerDlg::OnBnClickedRadioDown)
    ON_BN_CLICKED(IDC_RADIO_UP, &CSomeAutoclickerDlg::OnBnClickedRadioUp)

    ON_MESSAGE(UWM_CHANGESELECTION, &CSomeAutoclickerDlg::ChangeSelection)
END_MESSAGE_MAP()


// CSomeAutoclickerDlg message handlers

BOOL CSomeAutoclickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here

    // Initial values
    //AutokeyCombo.SetCurSel(0);

    ClickSequenceListControl.SetExtendedStyle(ClickSequenceListControl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMN lvColumn;

    lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    lvColumn.fmt = LVCFMT_LEFT | LVCFMT_FIXED_WIDTH;

    lvColumn.cx = 58;
    lvColumn.pszText = _T("Key");
    ClickSequenceListControl.InsertColumn(1, &lvColumn);

    lvColumn.cx = 58;
    lvColumn.pszText = _T("Wait (ms)");
    ClickSequenceListControl.InsertColumn(2, &lvColumn);

    lvColumn.cx = 39;
    lvColumn.pszText = _T("D/U");
    ClickSequenceListControl.InsertColumn(3, &lvColumn);

    /*
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = 0;
    lvItem.iSubItem = 0;
    lvItem.pszText = _T("");
    ClickSequenceListControl.InsertItem(&lvItem);
    ClickSequenceListControl.SetItemText(0, 0, _T("1"));
    ClickSequenceListControl.SetItemText(0, 1, _T("LMB"));
    ClickSequenceListControl.SetItemText(0, 2, _T("10"));
    ClickSequenceListControl.SetItemText(0, 3, _T("Down"));

    lvItem.iItem = 1;
    lvItem.pszText = _T("");
    ClickSequenceListControl.InsertItem(&lvItem);
    ClickSequenceListControl.SetItemText(1, 0, _T("2"));
    ClickSequenceListControl.SetItemText(1, 1, _T("LMB"));
    ClickSequenceListControl.SetItemText(1, 2, _T("10"));
    ClickSequenceListControl.SetItemText(1, 3, _T("Up"));
    */

    insert_clickdata_row(0, 1, 10, KEYPRESS_DOWN);
    insert_display_row(0, 1, 10, KEYPRESS_DOWN);
    insert_clickdata_row(1, 1, 10, KEYPRESS_UP);
    insert_display_row(1, 1, 10, KEYPRESS_UP);

    ClickSequenceListControl.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    //lastposition = 0;

    UpdateData(FALSE);
    ////Start the thread
    //pClickThread = (CClickingThread*)AfxBeginThread(RUNTIME_CLASS(CClickingThread), THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
    ////Pass the handle to the main window
    //pClickThread->mainwindowhwnd = m_hWnd;
    //pClickThread->ResumeThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSomeAutoclickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
        CSomeAutoclickerDlg::OnHelpAbout();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSomeAutoclickerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSomeAutoclickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//prevent escape or enter from closing the dialog window (solution from stackoverflow 29644317)
void CSomeAutoclickerDlg::OnOkOverride() 
{}


void CSomeAutoclickerDlg::OnCancelOverride() 
{
    // call base implementation if escape is not down
    if ((GetKeyState(VK_ESCAPE) & 0x8000) == 0)
    {
        OnCancel();
    }
}


void CSomeAutoclickerDlg::OnHelpAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}


void CSomeAutoclickerDlg::OnFileQuit() 
{
    OnCancel();
}


void CSomeAutoclickerDlg::OnBnClickedButtonStartAutoclicking()
{
    // TODO: Add your control notification handler code here

    this->set_all_buttons(FALSE);
    this->GetDlgItem(IDC_BUTTON_STOP_AUTOCLICKING)->EnableWindow(TRUE);

    // debug
    CString debug_string = _T("");
    CString debug_string2 = _T("");
    OutputEditbox = _T("");

    // RepeatModeRadio debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_sstream << RepeatModeRadio;
    debug_string = _T("RepeatModeRadio: ");
    debug_string += debug_sstream.str().c_str();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;


    // StartHotkey debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("StartHotkey: ");
    debug_string += StartHotkey.GetHotKeyName();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;


    // StopHotkey debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("StopHotkey: ");
    debug_string += StopHotkey.GetHotKeyName();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;

    /*
    // AutokeyCombo debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("StopHotkey: ");
    AutokeyCombo.GetLBText(AutokeyCombo.GetCurSel(), debug_string2);
    debug_string += debug_string2;
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;


    // AutokeyHotkey debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("AutokeyHotkey: ");
    debug_string += AutokeyHotkey.GetHotKeyName();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;
    */

    // KeyCodeEditbox debug

    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("KeyCodeEditbox: ");
    debug_string += KeyCodeEditbox;
    debug_string += _T(" (Num: ");
    int debug_num = -1;
    debug_num = _ttoi(KeyCodeEditbox);
    debug_sstream << debug_num;
    debug_string2 = debug_sstream.str().c_str();
    debug_string += debug_string2;
    debug_string += _T(")\r\n");
    OutputEditbox += debug_string;

    // DelayValueEditbox debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_string = _T("DelayValueEditbox: ");
    debug_string += DelayValueEditbox;
    debug_string += _T(" (Num: ");
    int debug_num2 = -1;
    debug_num2 = _ttoi(DelayValueEditbox);
    debug_sstream << debug_num2;
    debug_string2 = debug_sstream.str().c_str();
    debug_string += debug_string2;
    debug_string += _T(")\r\n");
    OutputEditbox += debug_string;


    // KeyPressModeRadio debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_sstream << KeyPressModeRadio;
    debug_string = _T("KeyPressModeRadio: ");
    debug_string += debug_sstream.str().c_str();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;


    // ClickSequenceListControl debug
    debug_sstream.str(std::string());
    debug_string = _T("");
    debug_string2 = _T("");

    debug_sstream << ClickSequenceListControl.GetSelectionMark();
    debug_string = _T("ClickSequenceListControl: ");
    debug_string += debug_sstream.str().c_str();
    debug_string += _T("\r\n");
    OutputEditbox += debug_string;


    // debug output to output edit box
    UpdateData(FALSE);
    //SetDlgItemText(IDC_EDIT_OUTPUT_BOX, OutputEditbox);

    //start clicking thread
    pClickThread = (CClickingThread*)AfxBeginThread(RUNTIME_CLASS(CClickingThread), THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
    hClickThread = pClickThread->m_hThread;
    pClickThread->main_window_hwnd = m_hWnd;
    pClickThread->ClickData = ClickData;

    // try to start clicking thread
    // if succeed, enable stop button
    // if fail, open modal popup, then reenable everything


    pClickThread->ResumeThread();
}


void CSomeAutoclickerDlg::OnBnClickedButtonStopAutoclicking()
{
    // TODO: Add your control notification handler code here

    UpdateData(FALSE);

    this->GetDlgItem(IDC_BUTTON_STOP_AUTOCLICKING)->EnableWindow(FALSE);
    // try to stop clicking thread
    // if succeed, enable all buttons
    // if fail, open modal popup asking to kill everything and quit program

    this->set_all_buttons(TRUE);
    this->GetDlgItem(IDC_BUTTON_STOP_AUTOCLICKING)->EnableWindow(FALSE);
}


void CSomeAutoclickerDlg::OnBnClickedButtonInsertBefore()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    insert_into_clickdata(ClickSequenceListControl.GetSelectionMark());

    //debug
    CString str_debug(_T("\r\n"));
    for (int i = 0; i < ClickData->data_rows.size(); ++i)
    {
        CString str_debug2;
        str_debug2.Format(_T("row %d: keycode(%d), wait_time(%d), key_down_or_up(%d) \r\n"), i, ClickData->data_rows[i].keycode, ClickData->data_rows[i].wait_time, ClickData->data_rows[i].key_down_or_up);
        str_debug += str_debug2;
    }
    str_debug += _T("----------\r\n");
    TRACE(str_debug);
}


void CSomeAutoclickerDlg::OnBnClickedButtonInsertAtEnd()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    insert_into_clickdata(ClickData->data_rows.size());

    //debug
    CString str_debug(_T("\r\n"));
    for (int i = 0; i < ClickData->data_rows.size(); ++i)
    {
        CString str_debug2;
        str_debug2.Format(_T("row %d: keycode(%d), wait_time(%d), key_down_or_up(%d) \r\n"), i, ClickData->data_rows[i].keycode, ClickData->data_rows[i].wait_time, ClickData->data_rows[i].key_down_or_up);
        str_debug += str_debug2;
    }
    str_debug += _T("----------\r\n");
    TRACE(str_debug);
}


void CSomeAutoclickerDlg::OnBnClickedButtonRemoveSelected()
{
    // TODO: Add your control notification handler code here
    UpdateData(FALSE);
}


void CSomeAutoclickerDlg::OnBnClickedButtonClearAll()
{
    // TODO: Add your control notification handler code here
    UpdateData(FALSE);
}


void CSomeAutoclickerDlg::OnBnClickedButtonSetStartKey()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    WORD StartKeyCode, StartKeyModifiers;
    StartHotkey.GetHotKey(StartKeyCode, StartKeyModifiers);

    //debug
    CString debug_string;
    debug_string.Format(_T("keycode: %x    modifiers: %x"), StartKeyCode, StartKeyModifiers);
    AfxMessageBox(debug_string);

    //reserve hotkey until thread start
    RegisterHotKey(NULL, StartHotkeyID, StartKeyModifiers, StartKeyCode);
}


void CSomeAutoclickerDlg::OnBnClickedButtonSetStopKey()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
}


void CSomeAutoclickerDlg::OnBnClickedRadioRepeating()
{
    UpdateData(TRUE);
}


void CSomeAutoclickerDlg::OnBnClickedRadioHeldDown()
{
    UpdateData(TRUE);
}


void CSomeAutoclickerDlg::OnBnClickedRadioBoth()
{
    UpdateData(TRUE);
}


void CSomeAutoclickerDlg::OnBnClickedRadioDown()
{
    UpdateData(TRUE);
}


void CSomeAutoclickerDlg::OnBnClickedRadioUp()
{
    UpdateData(TRUE);
}


LRESULT CSomeAutoclickerDlg::ChangeSelection(WPARAM wParam, LPARAM lParam)
{
    //wParam will be index of row that was last processed
    int index = static_cast<int>(wParam);
    ClickSequenceListControl.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    return LRESULT();
}


void CSomeAutoclickerDlg::set_all_buttons(BOOL enable)
{
    CWnd* pWndChild = GetWindow(GW_CHILD);
    while(pWndChild)
    {
        pWndChild->EnableWindow(enable);
        pWndChild = pWndChild->GetNextWindow();
    };
    this->GetDlgItem(IDC_EDIT_OUTPUT_BOX)->EnableWindow(TRUE);
}


void CSomeAutoclickerDlg::insert_into_clickdata(int position)
{
    ClickSequenceListControl.SetItemState(position, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    int keycode = _ttoi(KeyCodeEditbox);
    int wait_time = _ttoi(DelayValueEditbox);

    if (KeyCodeEditbox == _T("") || (keycode <= 0 || keycode >= 255))
    {
        AfxMessageBox(_T("Please enter a value for \"Key Code\" between 1 and 254"));
        return;
    }
    if (DelayValueEditbox == _T(""))
    {
        AfxMessageBox(_T("Please enter a value in ms for \"Delay afterwards\" (0 for no delay)"));
        return;
    }

    switch (KeyPressModeRadio)
    {
    case 0:
        insert_clickdata_row(position, keycode, wait_time, KEYPRESS_DOWN);
        insert_display_row(position, keycode, wait_time, KEYPRESS_DOWN);

        insert_clickdata_row(position+1, keycode, wait_time, KEYPRESS_UP);
        insert_display_row(position+1, keycode, wait_time, KEYPRESS_UP);
        break;

    case 1:
        insert_clickdata_row(position, keycode, wait_time, KEYPRESS_DOWN);
        insert_display_row(position, keycode, wait_time, KEYPRESS_DOWN);
        break;

    case 2:
        insert_clickdata_row(position, keycode, wait_time, KEYPRESS_UP);
        insert_display_row(position, keycode, wait_time, KEYPRESS_UP);
        break;

    default:
        AfxMessageBox(_T("something went wrong in insert_into_clickdata"));
        ASSERT(FALSE);
        return;
    }
}


void CSomeAutoclickerDlg::insert_clickdata_row(int position, int keycode, int wait_time, bool key_down_or_up)
{
    ClickDataRow temp_data_row(keycode, wait_time, key_down_or_up);
    ClickData->data_rows.insert(ClickData->data_rows.begin() + position, temp_data_row);
}


void CSomeAutoclickerDlg::insert_display_row(int position, int keycode, int wait_time, bool key_down_or_up)
{
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = position;
    lvItem.iSubItem = 0;
    lvItem.pszText = _T("");
    ClickSequenceListControl.InsertItem(&lvItem);

    CString str_position;
    str_position.Format(_T("%d"), position);
    CString str_wait_time;
    str_wait_time.Format(_T("%d"), wait_time);
    CString str_key_down_or_up;
    if (key_down_or_up == KEYPRESS_DOWN)
    {
        str_key_down_or_up = _T("Down");
    }
    else if (key_down_or_up == KEYPRESS_UP)
    {
        str_key_down_or_up = _T("Up");
    }
    else
    {
        AfxMessageBox(_T("something went wrong in insert_row"));
        ASSERT(FALSE);
        return;
    }

    ClickSequenceListControl.SetItemText(position, 0, VKeysDB->get_vkeystruct_from_vkeyid(keycode).Description);
    ClickSequenceListControl.SetItemText(position, 1, str_wait_time);
    ClickSequenceListControl.SetItemText(position, 2, str_key_down_or_up);

    ClickSequenceListControl.SetItemState(position, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}
