// IP_CalculatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IP_Calculator.h"
#include "IP_CalculatorDlg.h"
#include "IP_Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIP_CalculatorDlg dialog

CIP_CalculatorDlg::CIP_CalculatorDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CIP_CalculatorDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIP_CalculatorDlg)
    m_strIP1 = _T("");
    m_strIP2 = _T("");
    m_strIP3 = _T("");
    m_strIP4 = _T("");
    m_strBit = _T("");
    m_strUserP = _T("");
    m_strUserSR = _T("");
    m_strMask1 = _T("");
    m_strMask2 = _T("");
    m_strMask3 = _T("");
    m_strMask4 = _T("");
    m_strMaxP = _T("");
    m_strMaxSR = _T("");
    m_strPas = _T("");
    m_strPlage1 = _T("");
    m_strPlage2 = _T("");
    m_strNbBitsP = _T("");
    m_strNbBitsSR = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIP_CalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIP_CalculatorDlg)
    DDX_Control(pDX, IDC_NBBITSSR, m_ctrlNbBitsSR);
    DDX_Control(pDX, IDC_NBBITSP, m_ctrlNbBitsP);
    DDX_Control(pDX, IDC_PLAGE2, m_ctrlPlage2);
    DDX_Control(pDX, IDC_PLAGE1, m_ctrlPlage1);
    DDX_Control(pDX, IDC_PAS, m_ctrlPas);
    DDX_Control(pDX, IDC_MAXSR, m_ctrlMaxSR);
    DDX_Control(pDX, IDC_MAXP, m_ctrlMaxP);
    DDX_Control(pDX, IDC_MASK4, m_ctrlMask4);
    DDX_Control(pDX, IDC_MASK3, m_ctrlMask3);
    DDX_Control(pDX, IDC_MASK2, m_ctrlMask2);
    DDX_Control(pDX, IDC_MASK1, m_ctrlMask1);
    DDX_Control(pDX, IDC_USERSR, m_ctrlUserSR);
    DDX_Control(pDX, IDC_USERP, m_ctrlUserP);
    DDX_Control(pDX, IDC_NBBITS, m_ctrlBit);
    DDX_Control(pDX, IDC_IP4, m_ctrlIP4);
    DDX_Control(pDX, IDC_IP3, m_ctrlIP3);
    DDX_Control(pDX, IDC_IP2, m_ctrlIP2);
    DDX_Control(pDX, IDC_IP1, m_ctrlIP1);
    DDX_Text(pDX, IDC_IP1, m_strIP1);
    DDX_Text(pDX, IDC_IP2, m_strIP2);
    DDX_Text(pDX, IDC_IP3, m_strIP3);
    DDX_Text(pDX, IDC_IP4, m_strIP4);
    DDX_Text(pDX, IDC_NBBITS, m_strBit);
    DDX_Text(pDX, IDC_USERP, m_strUserP);
    DDX_Text(pDX, IDC_USERSR, m_strUserSR);
    DDX_Text(pDX, IDC_MASK1, m_strMask1);
    DDX_Text(pDX, IDC_MASK2, m_strMask2);
    DDX_Text(pDX, IDC_MASK3, m_strMask3);
    DDX_Text(pDX, IDC_MASK4, m_strMask4);
    DDX_Text(pDX, IDC_MAXP, m_strMaxP);
    DDX_Text(pDX, IDC_MAXSR, m_strMaxSR);
    DDX_Text(pDX, IDC_PAS, m_strPas);
    DDX_Text(pDX, IDC_PLAGE1, m_strPlage1);
    DDX_Text(pDX, IDC_PLAGE2, m_strPlage2);
    DDX_Text(pDX, IDC_NBBITSP, m_strNbBitsP);
    DDX_Text(pDX, IDC_NBBITSSR, m_strNbBitsSR);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIP_CalculatorDlg, CDialog)
    //{{AFX_MSG_MAP(CIP_CalculatorDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_RESET, OnReset)
    ON_EN_CHANGE(IDC_IP1, OnChangeIp1)
    ON_EN_CHANGE(IDC_IP2, OnChangeIp2)
    ON_EN_CHANGE(IDC_IP3, OnChangeIp3)
    ON_EN_CHANGE(IDC_IP4, OnChangeIp4)
    ON_EN_CHANGE(IDC_NBBITS, OnChangeNbbits)
    ON_EN_CHANGE(IDC_USERP, OnChangeUserp)
    ON_EN_CHANGE(IDC_USERSR, OnChangeUsersr)
    ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
    ON_EN_KILLFOCUS(IDC_NBBITS, OnKillfocusNbbits)
    ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIP_CalculatorDlg message handlers

BOOL CIP_CalculatorDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // TODO: Add extra initialization here

    m_ctrlIP1.SetLimitText(3);
    m_ctrlIP2.SetLimitText(3);
    m_ctrlIP3.SetLimitText(3);
    m_ctrlIP4.SetLimitText(3);
    m_ctrlBit.SetLimitText(2);
    m_ctrlUserP.SetLimitText(10);
    m_ctrlUserSR.SetLimitText(10);
    m_ctrlMask1.SetLimitText(3);
    m_ctrlMask2.SetLimitText(3);
    m_ctrlMask3.SetLimitText(3);
    m_ctrlMask4.SetLimitText(3);
    m_ctrlMaxP.SetLimitText(10);
    m_ctrlMaxSR.SetLimitText(10);
    m_ctrlPas.SetLimitText(2);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIP_CalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

void CIP_CalculatorDlg::OnDestroy()
{
    WinHelp(0L, HELP_QUIT);
    CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIP_CalculatorDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIP_CalculatorDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CIP_CalculatorDlg::OnReset() 
{
    // TODO: Add your control notification handler code here

    m_strIP1 = _T("");
    m_strIP2 = _T("");
    m_strIP3 = _T("");
    m_strIP4 = _T("");
    m_strBit = _T("");
    m_strUserP = _T("");
    m_strUserSR = _T("");
    m_strMask1 = _T("");
    m_strMask2 = _T("");
    m_strMask3 = _T("");
    m_strMask4 = _T("");
    m_strMaxP = _T("");
    m_strMaxSR = _T("");
    m_strPas = _T("");
    m_strPlage1 = _T("");
    m_strPlage2 = _T("");
    m_strNbBitsP=_T("");
    m_strNbBitsSR=_T("");

    m_ctrlIP1.SetWindowText(_T(""));
    m_ctrlIP2.SetWindowText(_T(""));
    m_ctrlIP3.SetWindowText(_T(""));
    m_ctrlIP4.SetWindowText(_T(""));
    m_ctrlBit.SetWindowText(_T(""));
    m_ctrlUserP.SetWindowText(_T(""));
    m_ctrlUserSR.SetWindowText(_T(""));
    m_ctrlMask1.SetWindowText(_T(""));
    m_ctrlMask2.SetWindowText(_T(""));
    m_ctrlMask3.SetWindowText(_T(""));
    m_ctrlMask4.SetWindowText(_T(""));
    m_ctrlMaxP.SetWindowText(_T(""));
    m_ctrlMaxSR.SetWindowText(_T(""));
    m_ctrlPas.SetWindowText(_T(""));
    m_ctrlNbBitsP.SetWindowText(_T(""));
    m_ctrlNbBitsSR.SetWindowText(_T(""));
    m_ctrlPlage1.SetWindowText(_T(""));
    m_ctrlPlage2.SetWindowText(_T(""));

    IP_Adr.IP_Reset();
}

void CIP_CalculatorDlg::OnChangeIp1() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlIP1.GetWindowText(m_strIP1);

    DWORD Result=ConvertStringToDWORD(m_strIP1);

    if (Result>255)
    {
        m_strIP1=_T("255");
        m_ctrlIP1.SetWindowText(m_strIP1);
        Result=255;
    }

    IP_Adr.SetIP1(Result);
}

void CIP_CalculatorDlg::OnChangeIp2() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlIP2.GetWindowText(m_strIP2);

    DWORD Result=ConvertStringToDWORD(m_strIP2);

    if (Result>255)
    {
        m_strIP2=_T("255");
        m_ctrlIP2.SetWindowText(m_strIP2);
        Result=255;
    }

    IP_Adr.SetIP2(Result);
}

void CIP_CalculatorDlg::OnChangeIp3() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlIP3.GetWindowText(m_strIP3);

    DWORD Result=ConvertStringToDWORD(m_strIP3);

    if (Result>255)
    {
        m_strIP3=_T("255");
        m_ctrlIP3.SetWindowText(m_strIP3);
        Result=255;
    }

    IP_Adr.SetIP3(Result);
}

void CIP_CalculatorDlg::OnChangeIp4() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlIP4.GetWindowText(m_strIP4);

    DWORD Result=ConvertStringToDWORD(m_strIP4);

    if (Result>255)
    {
        m_strIP4=_T("255");
        m_ctrlIP4.SetWindowText(m_strIP4);
        Result=255;
    }

    IP_Adr.SetIP4(Result);
}

void CIP_CalculatorDlg::OnChangeNbbits() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlBit.GetWindowText(m_strBit);

    DWORD Result=ConvertStringToDWORD(m_strBit);

    IP_Adr.SetBitMask(Result);
}

void CIP_CalculatorDlg::OnChangeUserp() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlUserP.GetWindowText(m_strUserP);

    if (m_strUserP.GetLength()==10)
    {
        CString FirstEntry=m_strUserP.GetAt(0);
        CString LastEntry=m_strUserP.GetAt(9);

        DWORD FirstNumber=ConvertStringToDWORD(FirstEntry);
        DWORD LastNumber=ConvertStringToDWORD(LastEntry);
        DWORD Entry=ConvertStringToDWORD(m_strUserP);

        if (LastNumber>2 || FirstNumber>1)
        {
            m_strUserP=_T("1073741822");
            m_ctrlUserP.SetWindowText(m_strUserP);
        }
        else if (Entry>1073741822)
        {
            m_strUserP=_T("1073741822");
            m_ctrlUserP.SetWindowText(m_strUserP);
        }
    }

    IP_Adr.SetNbPostes(ConvertStringToDWORD(m_strUserP));
}

void CIP_CalculatorDlg::OnChangeUsersr() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlUserSR.GetWindowText(m_strUserSR);

    if (m_strUserSR.GetLength()==10)
    {
        CString FirstEntry=m_strUserSR.GetAt(0);
        CString LastEntry=m_strUserSR.GetAt(9);

        DWORD FirstNumber=ConvertStringToDWORD(FirstEntry);
        DWORD LastNumber=ConvertStringToDWORD(LastEntry);
        DWORD Entry=ConvertStringToDWORD(m_strUserSR);

        if (LastNumber>2 || FirstNumber>1)
        {
            m_strUserSR=_T("1073741822");
            m_ctrlUserSR.SetWindowText(m_strUserSR);
        }
        else if (Entry>1073741822)
        {
            m_strUserSR=_T("1073741822");
            m_ctrlUserSR.SetWindowText(m_strUserSR);
        }
    }

    IP_Adr.SetNbReseaux(ConvertStringToDWORD(m_strUserSR));
}

void CIP_CalculatorDlg::OnCalculate() 
{
    // TODO: Add your control notification handler code here

    IP_Adr.IP_Calculate();

    m_ctrlBit.SetWindowText(IP_Adr.strBit);

    m_ctrlIP1.SetWindowText(IP_Adr.strIP1);
    m_ctrlIP2.SetWindowText(IP_Adr.strIP2);
    m_ctrlIP3.SetWindowText(IP_Adr.strIP3);
    m_ctrlIP4.SetWindowText(IP_Adr.strIP4);

    m_ctrlMask1.SetWindowText(IP_Adr.str_Mask1);
    m_ctrlMask2.SetWindowText(IP_Adr.str_Mask2);
    m_ctrlMask3.SetWindowText(IP_Adr.str_Mask3);
    m_ctrlMask4.SetWindowText(IP_Adr.str_Mask4);

    m_ctrlMaxP.SetWindowText(IP_Adr.strMaxP);
    m_ctrlNbBitsP.SetWindowText(IP_Adr.strNbBitsP);
    m_ctrlMaxSR.SetWindowText(IP_Adr.strMaxSR);
    m_ctrlNbBitsSR.SetWindowText(IP_Adr.strNbBitsSR);
    m_ctrlPas.SetWindowText(IP_Adr.strPas);

    m_ctrlPlage1.SetWindowText(IP_Adr.str_Plage1);
    m_ctrlPlage2.SetWindowText(IP_Adr.str_Plage2);

    m_ctrlUserP.SetWindowText(IP_Adr.str_UserP);
    m_ctrlUserSR.SetWindowText(IP_Adr.str_UserSR);

    IP_Adr.IP_Flush();

    m_strIP1 = _T("");
    m_strIP2 = _T("");
    m_strIP3 = _T("");
    m_strIP4 = _T("");
    m_strBit = _T("");
    m_strUserP = _T("");
    m_strUserSR = _T("");
    m_strMask1 = _T("");
    m_strMask2 = _T("");
    m_strMask3 = _T("");
    m_strMask4 = _T("");
    m_strMaxP = _T("");
    m_strMaxSR = _T("");
    m_strPas = _T("");
    m_strPlage1 = _T("");
    m_strPlage2 = _T("");
    m_strNbBitsP=_T("");
    m_strNbBitsSR=_T("");
}

void CIP_CalculatorDlg::OnKillfocusNbbits() 
{
    // TODO: Add your control notification handler code here

    DWORD Bit=IP_Adr.GetBitMask();

    m_ctrlBit.GetWindowText(m_strBit);

    DWORD Result=ConvertStringToDWORD(m_strBit);

    if (Result!=Bit)
    {
        m_strBit.Format(_T("%d"), Bit);
        m_ctrlBit.SetWindowText(m_strBit);
    }
}

void CIP_CalculatorDlg::OnOptions() 
{
    // TODO: Add your control notification handler code here

    CIP_Properties Options(IP_Adr.GetUnlock());

    if (Options.DoModal()==IDOK)
    {
        if (Options.m_boolBitMaskEntry==TRUE)
        {
            IP_SetWarning(War_UnlockMask);

            IP_Adr.SetUnlock(TRUE);
        }
        else IP_Adr.SetUnlock(FALSE);
    }
}
