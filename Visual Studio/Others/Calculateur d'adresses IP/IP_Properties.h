#if !defined(AFX_IP_PROPERTIES_H__4D06F1DD_B781_4F72_B74E_BA466582C84A__INCLUDED_)
#define AFX_IP_PROPERTIES_H__4D06F1DD_B781_4F72_B74E_BA466582C84A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IP_Properties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIP_Properties dialog

class CIP_Properties : public CDialog
{
    BOOL Verrou;

// Construction
public:
    CIP_Properties(BOOL UnlockState, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CIP_Properties)
    enum { IDD = IDD_PROPERTIES };
    CButton    m_ctrlBitMaskEntry;
    BOOL    m_boolBitMaskEntry;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIP_Properties)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CIP_Properties)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IP_PROPERTIES_H__4D06F1DD_B781_4F72_B74E_BA466582C84A__INCLUDED_)
