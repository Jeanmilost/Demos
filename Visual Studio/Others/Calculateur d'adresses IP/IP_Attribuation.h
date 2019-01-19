#if !defined(AFX_IP_ATTRIBUATION_H__CD82B866_5158_4DB0_8472_C3FC33352BB8__INCLUDED_)
#define AFX_IP_ATTRIBUATION_H__CD82B866_5158_4DB0_8472_C3FC33352BB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IP_Attribuation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIP_Attribuation dialog

class CIP_Attribuation : public CDialog
{
    DWORD Partage;

    CButton *Check1;
    CButton *Check2;
    CButton *Check3;
    CButton *Check4;

// Construction
public:

    DWORD NbToPosts;
    DWORD NbToSR;

    CIP_Attribuation(DWORD NbBits, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CIP_Attribuation)
    enum { IDD = IDD_ATTRIBUATION };
    CEdit    m_ctrlNbBits;
    CEdit    m_ctrlSR;
    CEdit    m_ctrlPostes;
    CString    m_strPostes;
    CString    m_strSR;
    CString    m_strNbBits;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIP_Attribuation)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CIP_Attribuation)
    virtual BOOL OnInitDialog();
    afx_msg void OnPonly();
    afx_msg void OnSronly();
    afx_msg void OnNpSr();
    afx_msg void OnNsrP();
    afx_msg void OnChangePostes();
    afx_msg void OnChangeSousReseaux();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IP_ATTRIBUATION_H__CD82B866_5158_4DB0_8472_C3FC33352BB8__INCLUDED_)
