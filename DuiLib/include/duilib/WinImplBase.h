#ifndef WIN_IMPL_BASE_HPP
#define WIN_IMPL_BASE_HPP

namespace DuiLib {

enum UILIB_RESOURCETYPE
{
    UILIB_FILE = 1,     // ���Դ����ļ�
    UILIB_ZIP,          // ���Դ���zipѹ����
    UILIB_RESOURCE,     // ������Դ
    UILIB_ZIPRESOURCE,  // ������Դ��zipѹ����
};

class DUILIB_API CWndImplBase
    : public CWindowWnd
    , public CNotifyPump
    , public INotifyUI
    , public IMessageFilterUI
    , public IDialogBuilderCallback
{
public:
    CWndImplBase();
    virtual ~CWndImplBase() {};

    // IMessageFilterUI �ӿڣ�������Ϣ���͵����ڹ���ǰ�Ĺ��˴���
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool &bHandled);

    // INotifyUI �ӿڣ�duilib �ؼ�֪ͨ��Ϣ
    virtual void Notify(TNotifyUI &msg);

    void ShowWindow(bool bShow = true, bool bTakeFocus = true);

protected:
    DUI_DECLARE_MESSAGE_MAP()
    virtual void OnClick(TNotifyUI &msg);

protected:
    // ��������ؽӿ�
    virtual LPCTSTR GetWindowClassName(void) const = 0;
    // virtual LPCTSTR GetSuperClassName() const;
    virtual UINT GetClassStyle() const { return CS_DBLCLKS; }

    // ������Դ��ؽӿ�
    virtual CDuiString GetSkinFolder() = 0;
    virtual CDuiString GetSkinFile() = 0;
    virtual UILIB_RESOURCETYPE GetResourceType() const { return UILIB_FILE; }
    virtual CDuiString GetZIPFileName() const { return _T(""); }
    virtual LPCTSTR GetResourceID() const { return _T(""); }

    // IDialogBuilderCallback �ӿڣ������Զ���ؼ�
    virtual CControlUI *CreateControl(LPCTSTR pstrClass) { return NULL; }

    // �������������У���һ�κ����һ�α����õĽӿڣ��ҽ�������һ�Ρ�
    // .���崴���󣬰����ݡ�����ؼ�������İ󶨡��ؼ��������õ�ֻ��Ҫ����һ�εĲ�����
    virtual void OnInitWindow(void) { }
    // .�������ٺ����������ǰ����󱻵��õĽӿڡ�new �Ĵ��壬�ڸýӿ��е��� delete ���ٶ���
    virtual void OnFinalMessage(HWND hWnd);
    // ����������ʾ���ݵĳ�ʼ��������
    // .���� ����|��ʾ ֮ǰ ��ʼ�� ���ݡ��� OnInitWindow �� OnFinalMesssage ֮�����
    virtual void OnDataInit(void) { }
    // .���� ����|���� ֮ǰ  ����  ���ݡ��� OnInitWindow �� OnFinalMesssage ֮�����
    virtual void OnDataSave(void) { }
    // ���岼�ֵ�һ����ȷ��ʼ������õĽӿڣ���ʱ���ڽ��ύˢ��/�������ڴ�����ʾǰ����
    virtual void OnPrepare(void) { }

    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    //2017-02-25 zhuyadong ���ƶ������л�
    virtual LRESULT OnLanguageUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // �Զ�����Ϣ�������͵����ڵ���Ϣ���ڽ��� CPaintManagerUI ����֮ǰ�ػ�
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

#if defined(WIN32) && !defined(UNDER_CE)
    virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
#endif
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    // ���ڴ�����̣�ͨ������Ҫ���ء����з��͵����ڵ���Ϣ���������ڴ˽ػ�
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    LRESULT OnWndDataUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    CPaintManagerUI m_pm;
    static LPBYTE m_lpResourceZIPBuffer;

private:
    int     m_nWndState;            // �����ж��Ƿ��ʹ������ݳ�ʼ��/������Ϣ
};

}

#endif // WIN_IMPL_BASE_HPP
