// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__13228CAC_FCD2_4FC1_80C9_1979B40FDB32__INCLUDED_)
#define AFX_STDAFX_H__13228CAC_FCD2_4FC1_80C9_1979B40FDB32__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>        // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>            // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>        // MFC socket extensions

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "IP_Errors.h"
#include "IP_Algorithms.h"

#endif // !defined(AFX_STDAFX_H__13228CAC_FCD2_4FC1_80C9_1979B40FDB32__INCLUDED_)
