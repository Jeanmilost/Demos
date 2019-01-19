// IP_Attribuation.cpp : implementation file
//

#include "stdafx.h"
#include "IP_Calculator.h"
#include "IP_Attribuation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIP_Attribuation dialog


CIP_Attribuation::CIP_Attribuation(DWORD NbBits, CWnd* pParent /*=NULL*/)
    : CDialog(CIP_Attribuation::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIP_Attribuation)
    m_strPostes = _T("");
    m_strSR = _T("");
    m_strNbBits = _T("");
    //}}AFX_DATA_INIT

    Partage=NbBits;

    NbToPosts = Partage;
    NbToSR = 0;
}

void CIP_Attribuation::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIP_Attribuation)
    DDX_Control(pDX, IDC_EDIT2, m_ctrlNbBits);
    DDX_Control(pDX, IDC_SOUS_RESEAUX, m_ctrlSR);
    DDX_Control(pDX, IDC_POSTES, m_ctrlPostes);
    DDX_Text(pDX, IDC_POSTES, m_strPostes);
    DDX_Text(pDX, IDC_SOUS_RESEAUX, m_strSR);
    DDX_Text(pDX, IDC_EDIT2, m_strNbBits);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIP_Attribuation, CDialog)
    //{{AFX_MSG_MAP(CIP_Attribuation)
    ON_BN_CLICKED(IDC_PONLY, OnPonly)
    ON_BN_CLICKED(IDC_SRONLY, OnSronly)
    ON_BN_CLICKED(IDC_NP_SR, OnNpSr)
    ON_BN_CLICKED(IDC_NSR_P, OnNsrP)
    ON_EN_CHANGE(IDC_POSTES, OnChangePostes)
    ON_EN_CHANGE(IDC_SOUS_RESEAUX, OnChangeSousReseaux)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIP_Attribuation message handlers

BOOL CIP_Attribuation::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here

    m_strNbBits.Format("%d", Partage);
    m_ctrlNbBits.SetWindowText(m_strNbBits);

    Check1        = (CButton *) GetDlgItem(IDC_PONLY);
    Check2        = (CButton *) GetDlgItem(IDC_SRONLY);
    Check3        = (CButton *) GetDlgItem(IDC_NP_SR);
    Check4        = (CButton *) GetDlgItem(IDC_NSR_P);

    Check1->SetCheck(1);

    m_ctrlPostes.EnableWindow(FALSE);
    m_ctrlSR.EnableWindow(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CIP_Attribuation::OnPonly() 
{
    // TODO: Add your control notification handler code here

    m_ctrlPostes.EnableWindow(FALSE);
    m_ctrlSR.EnableWindow(FALSE);

    m_strPostes=_T("");
    m_ctrlPostes.SetWindowText(m_strPostes);

    m_strSR=_T("");
    m_ctrlSR.SetWindowText(m_strSR);

    NbToPosts = Partage;
    NbToSR = 0;
}


void CIP_Attribuation::OnSronly() 
{
    // TODO: Add your control notification handler code here

    m_ctrlPostes.EnableWindow(FALSE);
    m_ctrlSR.EnableWindow(FALSE);

    m_strPostes=_T("");
    m_ctrlPostes.SetWindowText(m_strPostes);

    m_strSR=_T("");
    m_ctrlSR.SetWindowText(m_strSR);

    NbToPosts = 0;
    NbToSR = Partage;
}

void CIP_Attribuation::OnNpSr() 
{
    // TODO: Add your control notification handler code here

    m_ctrlPostes.EnableWindow(TRUE);
    m_ctrlSR.EnableWindow(FALSE);

    m_strSR=_T("");
    m_ctrlSR.SetWindowText(m_strSR);

    NbToPosts = 0;
    NbToSR = Partage;
}

void CIP_Attribuation::OnNsrP() 
{
    // TODO: Add your control notification handler code here

    m_ctrlPostes.EnableWindow(FALSE);
    m_ctrlSR.EnableWindow(TRUE);

    m_strPostes=_T("");
    m_ctrlPostes.SetWindowText(m_strPostes);

    NbToPosts = Partage;
    NbToSR = 0;
}

void CIP_Attribuation::OnChangePostes() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlPostes.GetWindowText(m_strPostes);
    DWORD Posts=ConvertStringToDWORD(m_strPostes);

    if (Posts<0) Posts=0;
    if (Posts>Partage)
    {
        Posts=Partage;
        m_strPostes.Format(_T("%d"), Posts);
        m_ctrlPostes.SetWindowText(m_strPostes);
    }

    NbToPosts = Posts;
    NbToSR = Partage-Posts;
}

void CIP_Attribuation::OnChangeSousReseaux() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here

    m_ctrlSR.GetWindowText(m_strSR);
    DWORD SR=ConvertStringToDWORD(m_strSR);

    if (SR<0) SR=0;
    if (SR>Partage)
    {
        SR=Partage;
        m_strSR.Format(_T("%d"), SR);
        m_ctrlSR.SetWindowText(m_strSR);
    }

    NbToPosts = Partage-SR;
    NbToSR = SR;
}
