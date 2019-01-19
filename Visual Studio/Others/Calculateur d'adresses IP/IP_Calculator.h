// **************************************************************
// *                                                            *
// *     main header file for the IP_CALCULATOR application        *
// *                                                            *
// **************************************************************

#if !defined(AFX_IP_CALCULATOR_H__884CC6B4_1E22_426F_ABDE_634E66FD56E5__INCLUDED_)
#define AFX_IP_CALCULATOR_H__884CC6B4_1E22_426F_ABDE_634E66FD56E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIP_CalculatorApp:
// See IP_Calculator.cpp for the implementation of this class
//

class CIP_CalculatorApp : public CWinApp
{
public:
    CIP_CalculatorApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIP_CalculatorApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CIP_CalculatorApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IP_CALCULATOR_H__884CC6B4_1E22_426F_ABDE_634E66FD56E5__INCLUDED_)
