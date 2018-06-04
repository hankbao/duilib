#include "stdafx.h"

namespace DuiLib {
LPCTSTR CCheckBoxUI::GetClass() const
{
    return DUI_CTR_CHECKBOX;
}

LPVOID CCheckBoxUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_CHECKBOX) == 0) { return static_cast<CCheckBoxUI *>(this); }

    return COptionUI::GetInterface(pstrName);
}

void CCheckBoxUI::SetCheck(bool bCheck, bool bTriggerEvent)
{
    Selected(bCheck, bTriggerEvent);
}

bool  CCheckBoxUI::GetCheck() const
{
    return IsSelected();
}

void CCheckBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    // ��ѡ��group������Ч
    if (_tcscmp(pstrName, _T("group")) == 0) { DUITRACE(_T("��֧������:group")); }
    else { COptionUI::SetAttribute(pstrName, pstrValue); }
}

}
