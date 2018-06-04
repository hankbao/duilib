//2017-02-25 zhuyadong ��Ӳ˵��ؼ�
// �˵���ѡ����Ϣ��
// 1. ������Ӧ�Զ��崰����Ϣ��WM_MENUITEM_CLICK
// 2. ������Ӧ Notify ֪ͨ��Ϣ��DUI_MSGTYPE_MENUITEM_CLICK
#ifndef __UIMENU_H__
#define __UIMENU_H__

#pragma once

namespace DuiLib {

enum EMMenuAlign
{
    EMENU_ALIGN_LEFT    = 1 << 1,
    EMENU_ALIGN_TOP     = 1 << 2,
    EMENU_ALIGN_RIGHT   = 1 << 3,
    EMENU_ALIGN_BOTTOM  = 1 << 4,
};

/////////////////////////////////////////////////////////////////////////////////////
//
class CMenuUI;
class CMenuElementUI;
class DUILIB_API CMenuWnd : public CWindowWnd, public INotifyUI, public IDialogBuilderCallback,
    public IObserver
{
    friend class CMenuElementUI;
public:
    /*
    *   @pOwner     һ���˵���Ҫָ��������������ǲ˵��ڲ�ʹ�õ�
    *   @xml        �˵��Ĳ����ļ�
    *   @pSkinType  �˵���Դ���ͣ�������Դ���ز˵�ʱ��Ч
    *   @point      �˵������Ͻ�����
    *   @pParent    �˵��ĸ����������ָ��
    *   @dwAlign    �˵��ĳ���λ�ã�Ĭ�ϳ������������²ࡣ
    */
    static CMenuWnd *CreateMenu(CMenuElementUI *pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT pt,
                                CPaintManagerUI *pParent, DWORD dwAlign = EMENU_ALIGN_LEFT | EMENU_ALIGN_TOP);

    static CDuiString       s_strName;      // �������˵���ĵ� ����
    static CDuiString       s_strUserData;  // �������˵���ĵ� �û�����
    static UINT_PTR         s_ptrTag;       // �������˵���ĵ� Tag
public:
    CMenuWnd(void);

    //�̳����Ľӿ�
    virtual void Notify(TNotifyUI &msg) { }
    virtual CControlUI *CreateControl(LPCTSTR pstrClassName);
    virtual void OnSubjectUpdate(WPARAM p1, WPARAM p2 = NULL, LPARAM p3 = NULL, CSubjectBase *pSub = NULL);

    // ��ȡ���˵��ؼ������ڶ�̬����Ӳ˵�
    CMenuUI *GetMenuUI(void);
    // ���µ����˵��Ĵ�С
    void ResizeMenu(void);
    // ���µ����Ӳ˵��Ĵ�С
    void ResizeSubMenu(void);

protected:
    CPaintManagerUI *GetManager(void);
    virtual LPCTSTR GetWindowClassName(void) const;
    virtual void OnFinalMessage(HWND hWnd);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
    void Init(CMenuElementUI *pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT ptClient,
              CPaintManagerUI *pParent, DWORD dwAlign = EMENU_ALIGN_LEFT | EMENU_ALIGN_TOP);

public:
    POINT               m_ptBase;
    STRINGorID          m_xml;
    CDuiString          m_sSkinType;
    CPaintManagerUI     m_pm;
    CMenuElementUI     *m_pOwner;
    DWORD               m_dwAlign;          // �˵����뷽ʽ
};

/////////////////////////////////////////////////////////////////////////////////////
//
class DUILIB_API CMenuUI : public CListUI
{
public:
    CMenuUI(void);
    virtual ~CMenuUI(void);

    // ����һ���µĲ˵�������Զ����Ĭ�����ԡ�
    // ���ڽ�� DuiLib ʹ�� /MT ����ѡ��ʱ�Ķ��Դ���_pFirstBlock == pHead
    CMenuElementUI *NewMenuItem(void);
    // ���/ɾ�� �˵���
    virtual bool Add(CControlUI *pControl);
    virtual bool AddAt(CControlUI *pControl, int iIndex);
    virtual bool Remove(CControlUI *pControl, bool bDoNotDestroy = false);

    // ��ȡ/���� �˵���λ������
    virtual int GetItemIndex(CControlUI *pControl) const;
    virtual bool SetItemIndex(CControlUI *pControl, int iIndex);

    // ���Ҳ˵��
    // ������صĲ˵�������Ӳ˵�������ֱ������Ӳ˵���
    // ������Ҫ��ӿ�չ������
    CMenuElementUI *FindMenuItem(LPCTSTR pstrName);

    //////////////////////////////////////////////////////////////////////////
    virtual LPCTSTR GetClass(void) const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual void DoEvent(TEventUI &event);
    virtual SIZE EstimateSize(SIZE szAvailable) override;
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

/////////////////////////////////////////////////////////////////////////////////////
//
class DUILIB_API CMenuElementUI : public CListContainerElementUI
{
    friend CMenuWnd;
public:
    CMenuElementUI(void);
    ~CMenuElementUI(void);

    // ����һ���µĲ˵�������Զ����Ĭ�����ԡ�
    // ���ڽ�� DuiLib ʹ�� /MT ����ѡ��ʱ�Ķ��Դ���_pFirstBlock == pHead
    CMenuElementUI *NewMenuItem(void);
    // ���/ɾ�� �˵��ֱ�ӵ��� Addd/AddAt/Remove

    //����
    void SetIconWidth(WORD wWidth);
    WORD GetIconWidth(void) const;
    void SetIconNormal(LPCTSTR pstrIcon);
    void SetIconChecked(LPCTSTR pstrIcon);
    void SetCheckItem(bool bCheckItem = false);
    bool IsCheckItem(void) const;
    void SetCheck(bool bCheck = true);
    bool IsChecked(void) const;

    void SetExpandWidth(WORD wWidth);
    WORD GetExpandWidth(void) const;
    void SetExpandIcon(LPCTSTR pstrIcon);

    void SetLine(void);
    bool IsLine(void) const;
    void SetLineColor(DWORD color);
    DWORD GetLineColor(void) const;

    //////////////////////////////////////////////////////////////////////////
    virtual LPCTSTR GetClass(void) const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual bool DoPaint(HDC hDC, const RECT &rcPaint, CControlUI *pStopControl) override;
    virtual SIZE EstimateSize(SIZE szAvailable);
    virtual void DoEvent(TEventUI &event);
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    virtual void SetAttributeList(LPCTSTR pstrList);

private:
    void DrawItemIcon(HDC hDC, const RECT &rcItem);
    void DrawItemText(HDC hDC, const RECT &rcItem) override;
    void DrawItemExpand(HDC hDC, const RECT &rcItem);

    void OnTimer(TEventUI &event);
    void OnMouseEnter(TEventUI &event);
    void OnMouseLeave(TEventUI &event);
    void OnLButtonUp(TEventUI &event);
    void OnKeyDown(TEventUI &event);

private:
    enum { TIMERID_MOUSEENTER = 30, TIMERID_MOUSELEAVE = 31 };

    CMenuWnd   *m_pSubMenuWnd;

    //�˵���ͼ��
    TDrawInfo   m_diIconNormal;     //����״̬ͼ��
    TDrawInfo   m_diIconChecked;    //ѡ��״̬ͼ�꣬����֧�ָ�ѡʱ��Ч
    bool        m_bCheckItem;       //�Ƿ�֧�ָ�ѡ
    bool        m_bChecked;         //��ǰ�Ƿ�ѡ��
    WORD        m_wIconWidth;       //ռ�ÿ��

    //��ʶ�����¼��˵���ͼ��
    //IsExpandable()
    TDrawInfo   m_diExpandIcon;     //
    WORD        m_wExpandWidth;     //ռ�ÿ��

    //�ָ���
    bool        m_bLine;            //���ָ���
    DWORD       m_dwLineColor;      //�ָ�����ɫ
};

} // namespace DuiLib

#endif // __UIMENU_H__
