#include "UIlib.h"
#include <ShlObj.h>
#include <string>
#include "resource.h"
#include "XUnzip.h"

using namespace DuiLib;
using std::string;
using std::wstring;

#if defined(UNICODE) || defined(_UNICODE)
    #define tstring wstring
#else
    #define tstring string
#endif // UINCODE

enum EMDLGTYPE
{
    EDLG_GET_PATH,  // Ŀ¼ѡ��Ի���
    EDLG_GET_FILE,  // �ļ�ѡ��Ի���
};

tstring GetPathFile(HWND hWnd, int nFlag, tstring strDefFileName = tstring())
{
    BOOL bRet = FALSE;
    // Ĭ���ļ���
    TCHAR szFile[MAX_PATH] = { 0 };
    _tcscpy(szFile, strDefFileName.c_str());
    // ��ǰ����Ŀ¼����ֹ��ǰ����Ŀ¼���ı�
    TCHAR szCurPath[MAX_PATH] = { 0 };
    GetCurrentDirectory(MAX_PATH, szCurPath);
    //
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_ENABLESIZING | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
    ofn.nFilterIndex = 1L;

    if (EDLG_GET_FILE == nFlag)
    {
        // ��������
        ofn.Flags |= OFN_FILEMUSTEXIST;
        ofn.lpstrFilter = _T("Zip ��Դ�ļ�(*.zip)\0*.zip\0\0");
        ofn.lpstrTitle = _T("��ѡ�� DuiLib Ƥ���ļ�(Zip)");
        bRet = GetOpenFileName(&ofn);
    }

    if (EDLG_GET_PATH == nFlag)
    {
        // ����У��ϵ��
        BROWSEINFO bi = { 0 };
        bi.hwndOwner = hWnd;        //ӵ���ߴ��ھ��,ΪNULL��ʾ�Ի����Ƿ�ģ̬��,ʵ��Ӧ����һ�㶼Ҫ��������
        bi.pszDisplayName = szFile; //�����ļ��еĻ�����
        bi.lpszTitle = _T("��ѡ�� DuiLib Ƥ��Ŀ¼");
        bi.ulFlags = BIF_NEWDIALOGSTYLE;
        LPITEMIDLIST idl = SHBrowseForFolder(&bi);
        bRet = SHGetPathFromIDList(idl, szFile);
    }

    SetCurrentDirectory(szCurPath);
    return bRet ? tstring(szFile) : tstring();
}

class CSkinView;
CSkinView      *s_pSkin = NULL;

class CSkinView : public CWndImplBase
{
public:
    void SetZipFile(LPCTSTR lpstrZip) { m_strZip = lpstrZip; }
    void SetSkinFile(LPCTSTR lpstrFile) { m_strFile = lpstrFile; }
    void SetResourceType(int nRType) { m_nResourceType = nRType; }
protected:
    virtual LPCTSTR GetWindowClassName(void) const { return _T("Skin"); }
    virtual UILIB_RESOURCETYPE GetResourceType() const { return (UILIB_RESOURCETYPE)m_nResourceType; }
    virtual CDuiString GetSkinFolder() { return _T(""); }
    virtual CDuiString GetSkinFile() { return m_strFile; }
    virtual CDuiString GetZIPFileName() const { return m_strZip; }
    virtual void OnFinalMessage(HWND hWnd)
    {
        CWndImplBase::OnFinalMessage(hWnd);
        delete this;
        s_pSkin = NULL;
    }

private:
    CDuiString m_strZip;
    CDuiString  m_strFile;
    int         m_nResourceType;
};

class CFrameWnd : public CWndImplBase
{
public:

    virtual void Notify(TNotifyUI &msg);

protected:
    virtual LPCTSTR GetWindowClassName(void) const { return _T("SkinViewer"); }
    virtual CDuiString GetSkinFolder() { return _T(""); }
    virtual CDuiString GetSkinFile() { return _T("102"); }
    virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_RESOURCE; }
    virtual LPCTSTR GetResourceID() const { return _T("xml"); }
    virtual void OnFinalMessage(HWND hWnd)
    {
        CWndImplBase::OnFinalMessage(hWnd);
        PostQuitMessage(0);
    }
    virtual void OnInitWindow(void);


    void OnPreview(void);
    void AddXmlFile(CDuiString sZipFile);
    void SwitchPage(int nType);

private:
    CButtonUI      *m_pBtnBrowse1;  // �ļ�Ƥ�����
    CButtonUI      *m_pBtnBrowse2;  // ZipƤ�����
    CButtonUI      *m_pBtnView;     // Ԥ��
    CEditUI        *m_pEdtPath;     // �ļ�Ƥ�� ·��
    CEditUI        *m_pEdtFile;     // �ļ�Ƥ�� xml�ļ���
    CEditUI        *m_pEdtZip;      // Zip Ƥ�� �ļ���
    CComboUI       *m_pCmbXmlFile;  // Zip Ƥ�� xml�ļ����б�

    int             m_nType;        // Ƥ������
};


void CFrameWnd::Notify(TNotifyUI &msg)
{
    bool bHandled = true;

    if (msg.sType == DUI_MSGTYPE_CLICK)
    {
        if (msg.pSender->GetName() == _T("btnBrowse1"))
        {
            tstring strPath = GetPathFile(GetHWND(), EDLG_GET_PATH);

            if (NULL != m_pEdtPath) { m_pEdtPath->SetText(strPath.c_str()); }
        }
        else if (msg.pSender->GetName() == _T("btnBrowse2"))
        {
            tstring strFile = GetPathFile(GetHWND(), EDLG_GET_FILE);

            if (NULL != m_pEdtZip) { m_pEdtZip->SetText(strFile.c_str()); }
        }
        else if (msg.pSender->GetName() == _T("btnView"))
        {
            OnPreview();
        }
        else if (msg.pSender->GetName() == _T("btnStop"))
        {
            if (NULL != s_pSkin) { s_pSkin->Close(); s_pSkin = NULL; }
        }
        else { bHandled = false; }
    }
    else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
    {
        m_nType = (msg.pSender->GetName() == _T("optFile")) ? UILIB_FILE : UILIB_ZIP;
    }
    else if (msg.sType == DUI_MSGTYPE_DRAGENTER || msg.sType == DUI_MSGTYPE_DRAGOVER)
    {
        if (NULL == msg.wParam) { return; }

        COleDataHelper *pHelper = (COleDataHelper *)msg.wParam;
        pHelper->SetEffect(pHelper->HasFileList() ? DROPEFFECT_COPY : DROPEFFECT_NONE);
    }
    else if (msg.sType == DUI_MSGTYPE_DRAGDROP)
    {
        if (NULL == msg.wParam) { return; }

        COleDataHelper *pHelper = (COleDataHelper *)msg.wParam;
        CDuiPtrArray ptrArray = pHelper->GetFileList();

        if (ptrArray.IsEmpty()) { return; }

        // �����ļ�����ֻ���ܵ�һ���ļ�
        CDuiString sDropFile((LPCTSTR)ptrArray.GetAt(0));

        for (int i = 0; i < ptrArray.GetSize(); ++i)
        {
            LPCTSTR szFile = (LPCTSTR)ptrArray.GetAt(i);
            DUITRACE(szFile);
            free((void *)szFile);
        }

        if (sDropFile.GetLength() <= 4) { return; }

        CDuiString sExt(sDropFile.Right(4));

        if (sExt.CompareNoCase(_T(".xml")) == 0)
        {
            int nPos = sDropFile.ReverseFind(_T('\\'));
            CDuiString sPath = sDropFile.Left(nPos);
            CDuiString sName = sDropFile.Right(sDropFile.GetLength() - nPos - 1);
            m_pEdtPath->SetText(sPath);
            m_pEdtFile->SetText(sName);
            SwitchPage(UILIB_FILE);
        }
        else
        {
            m_pEdtZip->SetText(sDropFile);
            AddXmlFile(sDropFile);
            SwitchPage(UILIB_ZIP);
        }
    }
    else { bHandled = false; }

    CWndImplBase::Notify(msg);
}


void CFrameWnd::OnInitWindow(void)
{
    m_pBtnBrowse1 = dynamic_cast<CButtonUI *>(m_pm.FindControl(_T("btnBrowse1")));
    m_pBtnBrowse2 = dynamic_cast<CButtonUI *>(m_pm.FindControl(_T("btnBrowse2")));
    m_pBtnView = dynamic_cast<CButtonUI *>(m_pm.FindControl(_T("btnView")));
    m_pEdtPath = dynamic_cast<CEditUI *>(m_pm.FindControl(_T("edtPath")));
    m_pEdtFile = dynamic_cast<CEditUI *>(m_pm.FindControl(_T("edtFile")));
    m_pEdtZip = dynamic_cast<CEditUI *>(m_pm.FindControl(_T("edtZipFile")));
    m_pCmbXmlFile = dynamic_cast<CComboUI *>(m_pm.FindControl(_T("cmbZipName")));
    s_pSkin = NULL;

    COptionUI *pOpt = dynamic_cast<COptionUI *>(m_pm.FindControl(_T("optFile")));
    m_nType = pOpt->IsSelected() ? UILIB_FILE : UILIB_ZIP;
}

void CFrameWnd::OnPreview(void)
{
    if (NULL != s_pSkin) { return; }

    if (UILIB_FILE == m_nType)
    {
        if (m_pEdtPath->GetText().IsEmpty() || m_pEdtFile->GetText().IsEmpty())
        {
            MessageBox(GetHWND(), _T("Ƥ��·��/�ļ�������ȷ��"), _T("��ʾ"), MB_OK);
            return;
        }

        CPaintManagerUI::SetResourcePath(m_pEdtPath->GetText());
        s_pSkin = new CSkinView;
        s_pSkin->SetResourceType(m_nType);
        s_pSkin->SetSkinFile(m_pEdtFile->GetText());
        s_pSkin->Create(NULL, _T("Skin"), UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE);
    }

    if (UILIB_ZIP == m_nType)
    {
        if (m_pEdtZip->GetText().IsEmpty() || m_pCmbXmlFile->GetCurSel() < 0)
        {
            MessageBox(GetHWND(), _T("Ƥ��ѹ����/�ļ�������ȷ��"), _T("��ʾ"), MB_OK);
            return;
        }

        CDuiString strText = m_pEdtZip->GetText();
        int nPos = strText.ReverseFind(_T('\\'));
        CDuiString strPath = strText.Left(nPos);
        CDuiString strZip = strText.Right(strText.GetLength() - nPos - 1);
        CPaintManagerUI::SetResourcePath(strPath);
        s_pSkin = new CSkinView;
        s_pSkin->SetZipFile(strZip);
        s_pSkin->SetResourceType(m_nType);
        s_pSkin->SetSkinFile(m_pCmbXmlFile->GetText());
        s_pSkin->Create(NULL, _T("Skin"), UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE);
    }
}

void CFrameWnd::AddXmlFile(CDuiString sZipFile)
{
    m_pCmbXmlFile->RemoveAll();
    HZIP hz = OpenZip((void *)sZipFile.GetData(), 0, ZIP_FILENAME);

    if (NULL == hz) { return; }

    ZIPENTRY ze;
    GetZipItem(hz, -1, &ze);
    int nFileCnt = ze.index;

    for (int i = 0; i < nFileCnt; ++i)
    {
        memset(&ze, 0, sizeof(ze));
        int index = -1;
        ZRESULT zr = 0;
        GetZipItem(hz, i, &ze);
        LPCTSTR pszExt = ze.name + _tcslen(ze.name) - 4;

        if (0 == _tcsicmp(pszExt, _T(".xml")))
        {
            CListLabelElementUI *pCtrl = new CListLabelElementUI;
            m_pCmbXmlFile->Add(pCtrl);
            pCtrl->SetText(ze.name);
        }
    }

    if (m_pCmbXmlFile->GetCount() > 0) { m_pCmbXmlFile->SelectItem(0); }
}

void CFrameWnd::SwitchPage(int nType)
{
    m_nType = nType;
    CTabLayoutUI *pLayout = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("tbLay")));

    if (NULL != pLayout)
    {
        pLayout->SelectItem(UILIB_FILE == m_nType ? 0 : 1);
    }

    if (UILIB_FILE == m_nType)
    {
        COptionUI *pOpt = static_cast<COptionUI *>(m_pm.FindControl(_T("optFile")));

        if (NULL != pOpt) { pOpt->Selected(true); }
    }
    else if (UILIB_ZIP == m_nType)
    {
        COptionUI *pOpt = static_cast<COptionUI *>(m_pm.FindControl(_T("optZipFile")));

        if (NULL != pOpt) { pOpt->Selected(true); }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);

    CFrameWnd *pFrame = new CFrameWnd();

    if (pFrame == NULL) { return 0; }

    pFrame->Create(NULL, _T("DuiLib Ƥ��Ԥ��"), UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE);
    pFrame->SetIcon(IDI_LOGO);
    pFrame->CenterWindow();
    CPaintManagerUI::MessageLoop();
    CPaintManagerUI::Term();
    return 0;
}
