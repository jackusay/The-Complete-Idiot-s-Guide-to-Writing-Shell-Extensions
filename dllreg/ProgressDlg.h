// ProgressDlg.h : Declaration of the CProgressDlg

#ifndef __PROGRESSDLG_H_
#define __PROGRESSDLG_H_

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg

class CProgressDlg : public CDialogImpl<CProgressDlg>
{
public:
    // Construction
    enum { IDD = IDD_PROGRESSDLG };

    CProgressDlg ( const string_list* pFileList, const CMINVOKECOMMANDINFO* pCmdInfo ) :
         m_bStopSign(false), m_hwndList(NULL), m_pFileList(pFileList),
         m_pCmdInfo(pCmdInfo)
     { }

    // Maps
    BEGIN_MSG_MAP(CProgressDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        NOTIFY_HANDLER(IDC_LIST, LVN_ITEMCHANGED, OnItemchangedList)
        COMMAND_ID_HANDLER(IDC_STOP, OnStop)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

    // Message handlers
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnItemchangedList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        m_bStopSign = true;
        return 0;
    }

    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        EndDialog(wID);
        return 0;
    }

protected:
    bool         m_bStopSign;
    HWND         m_hwndList;
    const string_list* m_pFileList;
    string_list  m_lsStatusMessages;    // list of status messages for the dialog
    const CMINVOKECOMMANDINFO* m_pCmdInfo;

    void DoWork();
};

#endif //__PROGRESSDLG_H_
