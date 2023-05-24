// DLLRegShlExt.h : Declaration of the CDLLRegShlExt

#ifndef __DLLREGSHLEXT_H_
#define __DLLREGSHLEXT_H_

/////////////////////////////////////////////////////////////////////////////
// CDLLRegShlExt

class ATL_NO_VTABLE CDLLRegShlExt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDLLRegShlExt, &CLSID_DLLRegShlExt>,
    public IShellExtInit,
    public IContextMenu
{
public:
    CDLLRegShlExt();
    ~CDLLRegShlExt();

    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

    DECLARE_REGISTRY_RESOURCEID(IDR_DLLREGSHLEXT)
    DECLARE_NOT_AGGREGATABLE(CDLLRegShlExt)
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CDLLRegShlExt)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

protected:
    HBITMAP     m_hRegBmp;
    HBITMAP     m_hUnregBmp;
    string_list m_lsFiles;
};

#endif //__DLLREGSHLEXT_H_
