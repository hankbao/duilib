#ifndef __UITILELAYOUT_H__
#define __UITILELAYOUT_H__

#pragma once

namespace DuiLib {
class DUILIB_API CTileLayoutUI : public CContainerUI
{
public:
    CTileLayoutUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void SetPos(RECT rc, bool bNeedInvalidate = true);

    int GetFixedColumns() const;
    void SetFixedColumns(int iColums);
    int GetFixedRows() const;
    void SetFixedRows(int iRows);
    int GetChildVPadding() const;
    void SetChildVPadding(int iPadding);
    bool GetChildRounded() const;
    void SetChildRounded(bool bRound);

    SIZE GetItemSize() const;
    void SetItemSize(SIZE szSize);
    int GetColumns() const;
    int GetRows() const;
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

private:
    void ResetInternVisible(void);  // �����ӿؼ��ڲ���ʾ����
protected:
    SIZE m_szItem;                  // ����|��ǰ���ӿؼ���С��Ĭ��80
    int m_nColumns;                 // ��ǰ�ӿؼ�����
    int m_nRows;                    // ��ǰ�ӿؼ�����

    int m_nColumnsFixed;            // ���ԣ��ӿؼ�����
    int m_nRowsFixed;               // ���ԣ��ӿؼ�����
    int m_iChildVPadding;           // ���ԣ��ӿؼ���ֱ������
    bool m_bChildRounded;           // ���ԣ��ӿؼ�ˮƽ����ֱ�����������ӿؼ���С
};
}
#endif // __UITILELAYOUT_H__
