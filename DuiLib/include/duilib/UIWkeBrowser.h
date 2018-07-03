#ifndef __WKE_BROWSER_H
#define __WKE_BROWSER_H
#pragma once


namespace DuiLib {

class DUILIB_API CWkeBrowserUI : public CControlUI
{
public:
    enum
    {
        ENTY_TITLE      = 1,    // ��ҳ����ı�
        ENTY_URL,               // ��ҳURL �ı�
        ENTY_ALERTBOX,          // ��ҳ���� ����
        ENTY_CONFIRMBOX,        // ��ҳ���� ȷ��
        ENTY_PROMPTBOX,         // ��ҳ���� ��ʾ
    };
public:
    CWkeBrowserUI(void);
    virtual ~CWkeBrowserUI(void);

    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual void DoEvent(TEventUI &event);
    virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
    virtual bool DoPaint(HDC hDC, const RECT &rcPaint, CControlUI *pStopControl);
    virtual void DoInit();


    void LoadUrl(LPCTSTR szUrl);
    void LoadFile(LPCTSTR szFile);
    void Reload(void);

    CDuiString RunJS(LPCTSTR strValue);
    void GoBack();
    void GoForward();

    const CDuiString &GetTitle(void) { return m_sTitle; }
    const CDuiString &GetURL(void) { return m_sURL; }

    //ENTY_ALERTBOX ENTY_CONFIRMBOX ENTY_PROMPTBOX
    const CDuiString &GetMsg(void) { return m_sMsg; }
    // ENTY_PROMPTBOX
    const CDuiString &GetDefRet(void) { return m_sDefRet; }
    const CDuiString &GetRet(void) { return m_sRet; }


    bool SendNotify(void *pWebView, int nFlag, LPCTSTR sMsg, LPCTSTR sDefRet = NULL, LPCTSTR sRet = NULL);
protected:
    void InitBrowser(void);
    void PaintWebContent(HDC hDC, const RECT &rcPaint);

protected:
    enum
    {
        DEFAULT_TIMERID = 10,
    };

    void       *m_pWeb;

    // �ص�֪ͨ����
    void       *m_pView;    //
    CDuiString  m_sTitle;   // ��ǰ��ҳ����
    CDuiString  m_sURL;     // ��ǰ��ҳ����
    CDuiString  m_sMsg;
    CDuiString  m_sDefRet;
    CDuiString  m_sRet;

};

}

#endif // __WKE_BROWSER_H

