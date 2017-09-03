// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CABCF246_9BB1_4A11_AB1F_E2DF376A4C50__INCLUDED_)
#define AFX_STDAFX_H__CABCF246_9BB1_4A11_AB1F_E2DF376A4C50__INCLUDED_

#if defined _WIN64

#ifndef WINVER
#define WINVER 0x0600
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0700
#endif

#endif //#if defined _WIN64

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <Winsock2.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#define SDK_V15

#pragma warning (disable: 4996)
#endif // !defined(AFX_STDAFX_H__CABCF246_9BB1_4A11_AB1F_E2DF376A4C50__INCLUDED_)
