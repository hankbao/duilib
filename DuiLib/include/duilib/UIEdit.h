#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

template <class CHART> class CRegexpT;

namespace DuiLib {
enum EMFilterType
{
    EFILTER_CHAR_UPPER      = 0x01,     // ��дӢ���ַ�
    EFILTER_CHAR_LOWER      = 0x02,     // СдӢ���ַ�
    EFILTER_CHAR_BLANK      = 0x04,     // Ӣ�Ŀո�
    EFILTER_SPECIAL_SYMBOL  = 0x08,     // �����ַ�
    EFILTER_CHAR_LINUX      = 0x10,     // Linux�ļ����������ַ� ������ģʽ
    EFILTER_NUMBER_BIN      = 0x20,     // ����������
    EFILTER_NUMBER_DEC      = 0x40,     // ʮ��������
    EFILTER_NUMBER_HEX      = 0x80,     // ʮ����������
};


class CEditWnd;

class DUILIB_API CEditUI : public CLabelUI
{
    friend class CEditWnd;
public:
    CEditUI();
    virtual ~CEditUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    HWND GetNativeWindow() const;

    void SetEnabled(bool bEnable = true);
    void SetText(LPCTSTR pstrText);
    void SetMaxChar(UINT uMax);
    UINT GetMaxChar();
    void SetReadOnly(bool bReadOnly);
    bool IsReadOnly() const;
    void SetPasswordMode(bool bPasswordMode);
    bool IsPasswordMode() const;
    void SetPasswordChar(TCHAR cPasswordChar);
    TCHAR GetPasswordChar() const;
    bool IsAutoSelAll();
    void SetAutoSelAll(bool bAutoSelAll);
    void SetNumberOnly(bool bNumberOnly);
    bool IsNumberOnly() const;
    int GetWindowStyls() const;
    HWND GetNativeEditHWND() const;

    LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage();
    void SetDisabledImage(LPCTSTR pStrImage);

    void SetSel(long nStartChar, long nEndChar);
    void SetSelAll();
    void SetReplaceSel(LPCTSTR lpszReplace);

    void SetPos(RECT rc, bool bNeedInvalidate = true);
    void Move(SIZE szOffset, bool bNeedInvalidate = true);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    SIZE EstimateSize(SIZE szAvailable);
    void DoEvent(TEventUI &event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void PaintStatusImage(HDC hDC);
    void PaintText(HDC hDC);

    //2017-02-25 zhuyadong �����ʾ���ּ�����ɫ
    void SetTipText(LPCTSTR pstrTip);
    LPCTSTR GetTipText();
    virtual void ReloadText(void);
    void SetTipColor(DWORD dwColor);
    DWORD GetTipColor();
    // 2017-07-21 zhuyadong ��� minmaxnumber ����
    void SetMinMaxNumber(int nMin, int nMax);
    int GetMinNumber();
    int GetMaxNumber();

    // 2018-01-28 zhuyadong ����ַ����˹���
    void SetCharFilter(bool bFilter);
    bool IsCharFilter(void);
    void SetFilterMode(bool bWiteList);
    bool GetFilterMode(void);
    CDuiString GetFilterCharSet(void);
    void SetFilterCharSet(CDuiString sCharSet);
    void SetFilterCharSet(int nFilterType);
    void AppendFilterCharSet(CDuiString sCharSet);

    // �������
    void SetRegExpFilter(bool bRegExp);
    bool IsRegExpFilter(void);
    void SetRegExpPattern(CDuiString sRegExp);

protected:
    DWORD GetNativeEditBkColor() const;
    bool IsValidChar(TCHAR ch);
#ifndef UNICODE
    bool IsValidChar(LPCTSTR pstr);
#endif // UNICODE
    bool IsRegExpMatch(TCHAR ch);
    bool IsRegExpMatch(LPCTSTR pstr);

protected:
    CEditWnd *m_pWindow;

    UINT m_uMaxChar;
    bool m_bReadOnly;
    bool m_bPasswordMode;
    bool m_bAutoSelAll;
    TCHAR m_cPasswordChar;
    UINT m_uButtonState;
    int m_iWindowStyls;

    TDrawInfo m_diNormal;
    TDrawInfo m_diHot;
    TDrawInfo m_diFocused;
    TDrawInfo m_diDisabled;

    //2017-02-25 zhuyadong �����ʾ���ּ�����ɫ
    CDuiString m_sTipText;
    CDuiString m_sTipTextOrig;
    DWORD m_dwTipColor;
    // 2017-07-21 zhuyadong ��� minmaxnumber ����
    int   m_nMinNumber;
    int   m_nMaxNumber;
    // 2018-01-28 zhuyadong ����ַ����˹���
    // �ֺڰ�����ģʽ������������������ַ������������豸��ֹ���ַ���
    // ������ʽƥ�����ַ����ˣ�ֻ����һ�����á�
    bool  m_bCharFilter;            // �����ַ�����
    bool  m_bWiteList;              // true ��ʾ��������false ��ʾ��������
    bool  m_bRegExp;                // true ��ʾ����������ʽƥ��
    CDuiString  m_sFilterCharSet;   // �ַ����˼�
    CDuiString  m_sRegExp;          // ������ʽ
    CRegexpT<TCHAR>    *m_pRegExp;  // ������ʽ����
};

}
#endif // __UIEDIT_H__