// IP_Properties.cpp : implementation file
//

#include "stdafx.h"
#include "IP_Calculator.h"
#include "IP_Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIP_Properties dialog


CIP_Properties::CIP_Properties(BOOL UnlockState, CWnd* pParent /*=NULL*/)
    : CDialog(CIP_Properties::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIP_Properties)
    m_boolBitMaskEntry = FALSE;
    //}}AFX_DATA_INIT

    Verrou=UnlockState;
}


void CIP_Properties::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIP_Properties)
    DDX_Control(pDX, IDC_CHECK1, m_ctrlBitMaskEntry);
    DDX_Check(pDX, IDC_CHECK1, m_boolBitMaskEntry);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIP_Properties, CDialog)
    //{{AFX_MSG_MAP(CIP_Properties)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIP_Properties message handlers

BOOL CIP_Properties::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here

    m_boolBitMaskEntry=Verrou;
    m_ctrlBitMaskEntry.SetCheck(Verrou);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
