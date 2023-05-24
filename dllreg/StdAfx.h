// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__8AB81E68_CB2F_11D3_8D3B_AC2F34F1FA3C__INCLUDED_)
#define AFX_STDAFX_H__8AB81E68_CB2F_11D3_8D3B_AC2F34F1FA3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0400
#define _WIN32_WINNT 0x0400
#define _WIN32_IE 0x0400
#define _ATL_APARTMENT_THREADED

// Define this symbol to get XP themes. See:
// http://msdn.microsoft.com/library/en-us/dnwxp/html/xptheming.asp
// for more info. Note that as of May 2006, the page says the symbols should
// be called "SIDEBYSIDE_COMMONCONTROLS" but the headers in my SDKs in VC 6 & 7
// don't reference that symbol. If ISOLATION_AWARE_ENABLED doesn't work for you,
// try changing it to SIDEBYSIDE_COMMONCONTROLS
#define ISOLATION_AWARE_ENABLED 1

// ATL
#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>
#include <atlcom.h>

// STL
#include <string>
#include <list>
typedef std::list< std::basic_string<TCHAR> > string_list;

// Win32
#include <commctrl.h>
#include <comdef.h>
#include <shlobj.h>
#include <shlguid.h>

// Utility macros
#define countof(x) (sizeof(x)/sizeof((x)[0]))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8AB81E68_CB2F_11D3_8D3B_AC2F34F1FA3C__INCLUDED)
