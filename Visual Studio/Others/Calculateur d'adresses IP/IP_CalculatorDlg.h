// IP_CalculatorDlg.h : header file
//

#if !defined(AFX_IP_CALCULATORDLG_H__B3A11E06_CF34_4A88_A2F1_EC539B4B6910__INCLUDED_)
#define AFX_IP_CALCULATORDLG_H__B3A11E06_CF34_4A88_A2F1_EC539B4B6910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIP_CalculatorDlg dialog

class CIP_CalculatorDlg : public CDialog
{
// Construction
    IP_Algorithms    IP_Adr;

public:
    CIP_CalculatorDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CIP_CalculatorDlg)
    enum { IDD = IDD_IP_CALCULATOR_DIALOG };
    CEdit    m_ctrlNbBitsSR;
    CEdit    m_ctrlNbBitsP;
    CEdit    m_ctrlPlage2;
    CEdit    m_ctrlPlage1;
    CEdit    m_ctrlPas;
    CEdit    m_ctrlMaxSR;
    CEdit    m_ctrlMaxP;
    CEdit    m_ctrlMask4;
    CEdit    m_ctrlMask3;
    CEdit    m_ctrlMask2;
    CEdit    m_ctrlMask1;
    CEdit    m_ctrlUserSR;
    CEdit    m_ctrlUserP;
    CEdit    m_ctrlBit;
    CEdit    m_ctrlIP4;
    CEdit    m_ctrlIP3;
    CEdit    m_ctrlIP2;
    CEdit    m_ctrlIP1;
    CString    m_strIP1;
    CString    m_strIP2;
    CString    m_strIP3;
    CString    m_strIP4;
    CString    m_strBit;
    CString    m_strUserP;
    CString    m_strUserSR;
    CString    m_strMask1;
    CString    m_strMask2;
    CString    m_strMask3;
    CString    m_strMask4;
    CString    m_strMaxP;
    CString    m_strMaxSR;
    CString    m_strPas;
    CString    m_strPlage1;
    CString    m_strPlage2;
    CString    m_strNbBitsP;
    CString    m_strNbBitsSR;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIP_CalculatorDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CIP_CalculatorDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnReset();
    afx_msg void OnChangeIp1();
    afx_msg void OnChangeIp2();
    afx_msg void OnChangeIp3();
    afx_msg void OnChangeIp4();
    afx_msg void OnChangeNbbits();
    afx_msg void OnChangeUserp();
    afx_msg void OnChangeUsersr();
    afx_msg void OnCalculate();
    afx_msg void OnKillfocusNbbits();
    afx_msg void OnOptions();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IP_CALCULATORDLG_H__B3A11E06_CF34_4A88_A2F1_EC539B4B6910__INCLUDED_)
