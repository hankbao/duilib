#ifndef __UIHWEIGHTLAYOUT_H__
#define __UIHWEIGHTLAYOUT_H__

#pragma once

namespace DuiLib {
class DUILIB_API CHWeightLayoutUI : public CHorizontalLayoutUI
{
public:
    CHWeightLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    void SetPos(RECT rc, bool bNeedInvalidate = true);

protected:
    void ResetWeightCtrlState(void);    // ��Ȩ���ұ����صĿؼ��������� InternVisible ����
    bool HideMinHeightCtrl(int nIndex); // ����ָ������λ�õĿؼ�

};
}
#endif // __UIHWEIGHTLAYOUT_H__
