// ProgressDlg.cpp : Implementation of CProgressDlg

#include "stdafx.h"
#include "resource.h"
#include "ProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg

LRESULT CProgressDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
LVCOLUMN rCol = { LVCF_FMT | LVCF_TEXT, LVCFMT_LEFT, 0, _T("Filename") };

    // List control initialization.
    m_hwndList = GetDlgItem ( IDC_LIST );

    ListView_InsertColumn ( m_hwndList, 0, &rCol );

    rCol.mask |= LVCF_SUBITEM;
    rCol.pszText = _T("Result");
    rCol.iSubItem = 1;

    ListView_InsertColumn ( m_hwndList, 1, &rCol );

    ListView_SetColumnWidth ( m_hwndList, 0, LVSCW_AUTOSIZE_USEHEADER );
    ListView_SetColumnWidth ( m_hwndList, 1, LVSCW_AUTOSIZE_USEHEADER );

    // Show the window.
    CenterWindow ( m_pCmdInfo->hwnd );
    ShowWindow ( SW_SHOW );

    // Process all the files in the string list passed in to our constructor.
    DoWork();

    // Enable the Close button, and disable the Stop button.
    ::EnableWindow ( GetDlgItem ( IDCANCEL ), TRUE );
    ::EnableWindow ( GetDlgItem ( IDC_STOP ), FALSE );

    return 1;  // Let the system set the focus
}

// work area - this needs to be outside DoWork() so I can use try/finally in that function.
std::basic_string<TCHAR> strMsg;

void CProgressDlg::DoWork()
{
USES_CONVERSION;
HRESULT (STDAPICALLTYPE* pfn)();
HINSTANCE hinst;
TCHAR     szMsg [512];
LPCSTR    pszFnName;
WORD      wCmd;
LVITEM    rItem;
int       nIndex = 0;
bool      bSuccess;

    wCmd = LOWORD(m_pCmdInfo->lpVerb);

    // We only support 2 commands, so check the value passed in lpVerb.
    if ( 0 != wCmd && 1 != wCmd )
        {
        ATLASSERT(0);   // should never get here
        return;
        }

    // Determine which function we'll be calling.  Note that these strings are
    // not enclosed in the _T macro, since GetProcAddress() only takes an
    // ANSI string for the function name.
    pszFnName = wCmd ? "DllUnregisterServer" : "DllRegisterServer";

    for ( string_list::const_iterator it = m_pFileList->begin();
          it != m_pFileList->end(); it++ )
        {
        bSuccess = false;
        hinst    = NULL;
        *szMsg   = '\0';

        // We will print a status message into szMsg, which will eventually
        // be stored in the LPARAM of a listview control item.
        __try
            {
            // Try to load the next file.
            hinst = LoadLibraryEx ( it->c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );

            // If it failed, construct a friendly error message.
            if ( NULL == hinst )
                {
                void* pvMsgBuf = NULL;
                DWORD dwLastErr = GetLastError();

                FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                  FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, dwLastErr, 
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPTSTR) &pvMsgBuf, 0, NULL );

                wsprintf ( szMsg, _T("LoadLibraryEx failed on this module.\nError 0x%08lX (%s)"),
                           dwLastErr, pvMsgBuf ? pvMsgBuf : _T("No description available") );

                LocalFree ( pvMsgBuf );
                continue;
                }

            // Get the address of the register/unregister function.
            (FARPROC&) pfn = GetProcAddress ( hinst, pszFnName );

            // If it wasn't found, construct an error message.
            if ( NULL == pfn )
                {
                wsprintf ( szMsg, _T("%hs not found in this module."), pszFnName );
                continue;
                }

            // Call the function!
            HRESULT hr = pfn();

            // Construct a message listing the result of the function call.
            if ( SUCCEEDED(hr) )
                {
                bSuccess = true;
                wsprintf ( szMsg, _T("%hs succeeded on %s"), pszFnName, it->c_str() );
                }
            else
                {
                void* pvMsgBuf = NULL;

                FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                  FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, hr, 
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPTSTR) &pvMsgBuf, 0, NULL );

                wsprintf ( szMsg, _T("%hs failed on %s\nError 0x%08lX: %s"),
                           pszFnName, it->c_str(), (DWORD) hr,
                           NULL != pvMsgBuf ? pvMsgBuf : _T("(No description available)") );
                
                LocalFree ( pvMsgBuf );
                }
            }   // end __try
        __finally
            {
            // Fill in the LVITEM struct.  The item text will be the filename,
            // and the LPARAM will point to a copy of the szMsg status message.
            strMsg = szMsg;
            m_lsStatusMessages.push_back ( strMsg );

            ZeroMemory ( &rItem, sizeof(LVITEM) );
            rItem.mask     = LVIF_PARAM | LVIF_TEXT;
            rItem.iItem    = nIndex;
            rItem.pszText  = const_cast<LPTSTR>( it->c_str() );
            rItem.lParam   = (LPARAM)(DWORD_PTR) m_lsStatusMessages.back().c_str();

            ListView_InsertItem ( m_hwndList, &rItem );

            // Set the text in column 2 to "succeeded" or "failed" depending 
            // on the outcome of the function call.
            ListView_SetItemText ( m_hwndList, nIndex++, 1,
                                   bSuccess ? _T("Succeeded") : _T("Failed") );

            if ( NULL != hinst )
                FreeLibrary ( hinst );
            }

        // Process messages in our queue - this is much easier than doing it
        // "the real way" with a worker thread. This is how we detect a click
        // on the Stop button.
        MSG  msg;

        while ( PeekMessage ( &msg, m_hWnd, 0, 0, PM_REMOVE ) )
            {
            TranslateMessage ( &msg );
            DispatchMessage ( &msg );
            }

        // Resize the list columns.
        ListView_SetColumnWidth ( m_hwndList, 0, LVSCW_AUTOSIZE_USEHEADER );
        ListView_SetColumnWidth ( m_hwndList, 1, LVSCW_AUTOSIZE_USEHEADER );
        }   // end for
}


LRESULT CProgressDlg::OnItemchangedList ( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
NMLISTVIEW* pNMLV = (NMLISTVIEW*) pnmh;
LVITEM      rItem = { 0 };

    // This handler is called when something about a list item changes.
    // We first check if there is a selection.  If not, ignore the message.
    if ( -1 == pNMLV->iItem )
        {
        bHandled = false;
        return 0;
        }

    // If the item changing is becoming selected, display its associated
    // status message in the dialog.
    if ( pNMLV->uNewState & LVIS_SELECTED )
        {
        rItem.mask  = LVIF_PARAM;
        rItem.iItem = pNMLV->iItem;

        ListView_GetItem ( m_hwndList, &rItem );
        SetDlgItemText ( IDC_STATUS, (LPCTSTR) rItem.lParam );
        }

    return 0;
}
