#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

namespace DuiLib {
class DUILIB_API COptionUI : public CButtonUI
{
public:
    COptionUI();
    ~COptionUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetManager(CPaintManagerUI *pManager, CControlUI *pParent, bool bInit = true);

    bool Activate();
    void SetEnabled(bool bEnable = true);

    void SetSelTextColor(DWORD dwTextColor);
    DWORD GetSelTextColor();

    void SetSelBkColor(DWORD dwBkColor);
    DWORD GetSelBkColor();

    void SetSelHotBkColor(DWORD dwHotBkColor);
    DWORD GetSelHotBkColor();

    // ѡ��
    LPCTSTR GetSelNormalImg();
    void SetSelNormalImg(LPCTSTR pStrImage);
    LPCTSTR GetSelHotImg();
    void SetSelHotImg(LPCTSTR pStrImage);
    LPCTSTR GetSelFocusedImg();
    void SetSelFocusedImg(LPCTSTR pStrImage);
    LPCTSTR GetSelPushedImg();
    void SetSelPushedImg(LPCTSTR pStrImage);
    LPCTSTR GetSelDisabledImg();
    void SetSelDisabledImg(LPCTSTR pStrImage);
    // δѡ��
    LPCTSTR GetUnselNormalImg();
    void SetUnselNormalImg(LPCTSTR pStrImage);
    LPCTSTR GetUnselHotImg();
    void SetUnselHotImg(LPCTSTR pStrImage);
    LPCTSTR GetUnselFocusedImg();
    void SetUnselFocusedImg(LPCTSTR pStrImage);
    LPCTSTR GetUnselPushedImg();
    void SetUnselPushedImg(LPCTSTR pStrImage);
    LPCTSTR GetUnselDisabledImg();
    void SetUnselDisabledImg(LPCTSTR pStrImage);

    LPCTSTR GetForeImage();
    void SetForeImage(LPCTSTR pStrImage);

    LPCTSTR GetGroup() const;
    void SetGroup(LPCTSTR pStrGroupName = NULL);
    bool IsSelected() const;
    virtual void Selected(bool bSelected, bool bTriggerEvent = true);

    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    virtual void PaintBkColor(HDC hDC);
    virtual void PaintStatusImage(HDC hDC);
    void PaintText(HDC hDC);

protected:
    void SwitchTabLayoutPage(void);
protected:
    bool            m_bSelected;
    CDuiString      m_sGroupName;

    // 5̬��ɫ ѡ��ʱ����������������ɫ��Ч
    // Button::m_dwPushedTextColor      ����/ѡ�� ǰ��ɫ ������ɫ��Buttton
    DWORD           m_dwSelBkColor;     // ����/ѡ�� ����ɫ
    DWORD           m_dwSelHotBkColor;  // ����/ѡ�� ����ɫ

    TDrawInfo       m_diSelNormal;      // ѡ��״̬ ����
    TDrawInfo       m_diSelHot;         // ѡ��״̬ �������
    TDrawInfo       m_diSelFocused;     // ѡ��״̬ ����״̬
    TDrawInfo       m_diSelPushed;      // ѡ��״̬ ����
    TDrawInfo       m_diSelDisabled;    // ѡ��״̬ ����

    TDrawInfo       m_diFore;

    int             m_nBindTabIndex;    // �����󴥷��� TabLayout ҳ��������
    CDuiString      m_sBindTabLayout;   // �󶨵� TabLayout �ؼ���
};

class CButtonGroupImpl;
class DUILIB_API CButtonGroup
{
public:
    CButtonGroup(void);
    virtual ~CButtonGroup(void);

    void AddButton(COptionUI *pBtn, int nId = -1);
    void DelButton(COptionUI *pBtn);

    void SetButtonId(COptionUI *pBtn, int nId);
    int  GetButtonId(COptionUI *pBtn);
    COptionUI *GetButton(int nId);

    COptionUI *GetSelectedButton(void);
    int        GetSelectedId(void);
private:
    CButtonGroupImpl *m_pBtnGroupImpl;
};

} // namespace DuiLib

#endif // __UIOPTION_H__