#include "stdafx.h"
#include <vector>

namespace DuiLib {

typedef struct tagCtrlInfo
{
    int         nIndex;     // ����
    int         nWeight;    // Ȩ��
    int         nSpace;     // �ؼ�ռ�õĿ�
    CControlUI *pCtrl;      // �ؼ�ָ��

    tagCtrlInfo(int nI, int nW, int nS, CControlUI *pC) : nIndex(nI), nWeight(nW), pCtrl(pC), nSpace(nS) { }
    tagCtrlInfo(CControlUI *pC) : nIndex(0), nWeight(0), nSpace(0), pCtrl(pC) { }
    bool operator==(const tagCtrlInfo &tCtrl) const { return this->pCtrl == tCtrl.pCtrl; }

} TCtrlInfo;

typedef std::vector<TCtrlInfo>      CVecCtrlInfo;


CHWeightLayoutUI::CHWeightLayoutUI()
{
}

LPCTSTR CHWeightLayoutUI::GetClass() const
{
    return DUI_CTR_HWEIGHTLAYOUT;
}

LPVOID CHWeightLayoutUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_HWEIGHTLAYOUT) == 0) { return static_cast<CHWeightLayoutUI *>(this); }

    return CContainerUI::GetInterface(pstrName);
}

UINT CHWeightLayoutUI::GetControlFlags() const
{
    if (IsEnabled() && m_iSepWidth != 0) { return UIFLAG_SETCURSOR; }
    else { return 0; }
}

void CHWeightLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
{
    CControlUI::SetPos(rc, bNeedInvalidate);
    rc = m_rcItem;

    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) { rc.right -= m_pVerticalScrollBar->GetFixedWidth(); }

    if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) { rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight(); }

    if (m_items.GetSize() == 0)
    {
        ProcessScrollBar(rc, 0, 0);
        return;
    }

    // Determine the minimum size ��������Բ��ֲ��ҿɼ��Ŀؼ�����С�ռ�
    SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

    if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
    { szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange(); }

    if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
    { szAvailable.cy += m_pVerticalScrollBar->GetScrollRange(); }

    int cyNeeded = 0;               // �����ӿؼ��У��߶ȵ����ֵ
    int nAdjustables = 0;           // �ɵ�����С�Ŀؼ��ĸ���
    int cxFixed = 0;                // �ؼ�ռ�õĿ��
    int nEstimateNum = 0;           // ��Ҫ�����С�Ŀؼ���
    SIZE szControlAvailable;        // �ؼ������ÿռ�
    int iControlMaxWidth = 0;       // ���ؼ��Ŀ�
    int iControlMaxHeight = 0;
    int cxExpand = 0;
    CVecCtrlInfo vecCtrls;          // ��ȡ�ӿؼ�Ȩ��
    CControlUI *pCtrl = NULL;

    ResetWeightCtrlState();         // �����ӿؼ��ڲ���ʾ����

    // ԭ���룬����ռ�
    for (int i = 0; i < m_items.GetSize(); i++)
    {
        pCtrl = static_cast<CControlUI *>(m_items[i]);

        if (!pCtrl->IsVisible()) { continue; }

        if (pCtrl->IsFloat()) { continue; }

        szControlAvailable = szAvailable;
        RECT rcPadding = pCtrl->GetPadding();
        szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
        iControlMaxWidth = pCtrl->GetFixedWidth();
        iControlMaxHeight = pCtrl->GetFixedHeight();

        if (iControlMaxWidth <= 0) { iControlMaxWidth = pCtrl->GetMaxWidth(); }

        if (iControlMaxHeight <= 0) { iControlMaxHeight = pCtrl->GetMaxHeight(); }

        if (szControlAvailable.cx > iControlMaxWidth) { szControlAvailable.cx = iControlMaxWidth; }

        if (szControlAvailable.cy > iControlMaxHeight) { szControlAvailable.cy = iControlMaxHeight; }

        SIZE sz = { 0 };

        if (pCtrl->GetFixedWidth() == 0)
        {
            nAdjustables++;
            sz.cy = pCtrl->GetFixedHeight();

            if (sz.cx < pCtrl->GetMinWidth()) { sz.cx = pCtrl->GetMinWidth(); }
        }
        else
        {
            sz = pCtrl->EstimateSize(szControlAvailable);

            if (sz.cx == 0)
            {
                nAdjustables++;
            }
            else
            {
                if (sz.cx < pCtrl->GetMinWidth()) { sz.cx = pCtrl->GetMinWidth(); }

                if (sz.cx > pCtrl->GetMaxWidth()) { sz.cx = pCtrl->GetMaxWidth(); }
            }
        }

        cxFixed += sz.cx + pCtrl->GetPadding().left + pCtrl->GetPadding().right;

        sz.cy = std::max<int>(sz.cy, 0);

        if (sz.cy < pCtrl->GetMinHeight()) { sz.cy = pCtrl->GetMinHeight(); }

        if (sz.cy > pCtrl->GetMaxHeight()) { sz.cy = pCtrl->GetMaxHeight(); }

        cyNeeded = std::max<int>(cyNeeded, sz.cy + rcPadding.top + rcPadding.bottom);
        nEstimateNum++;
        vecCtrls.push_back(TCtrlInfo(i, pCtrl->GetWeight(), sz.cx, pCtrl));
    }

    // ��Ȩ�ش�С��������
    std::sort(vecCtrls.begin(), vecCtrls.end(), [ ](TCtrlInfo lhs, TCtrlInfo rhs) -> bool
    {
        return lhs.nWeight < rhs.nWeight;
    });

    cxFixed += (nEstimateNum - 1) * m_iChildPadding;

    // ����ռ䲻�㣬����Ȩ�شӵ͵��������ؼ���С
    if (szAvailable.cx < cxFixed)
    {
        for (CVecCtrlInfo::iterator it(vecCtrls.begin()); it != vecCtrls.end(); ++it)
        {
            if (0 == it->nSpace) { continue; }

            pCtrl = it->pCtrl;
            int nSub = std::max<int>(0, it->nSpace - pCtrl->GetMinWidth());

            if (nSub > 0)
            {
                int nSubWidth = std::min<int>(abs(szAvailable.cx - cxFixed), nSub);
                it->nSpace -= nSubWidth;
                cxFixed -= nSubWidth;

                if (szAvailable.cx >= cxFixed) { break; }
            }
        }
    }

    if (szAvailable.cx <= cxFixed)
    {
        for (CVecCtrlInfo::iterator it(vecCtrls.begin());
             it != vecCtrls.end() && szAvailable.cx <= cxFixed;
             ++it)
        {
            HideMinHeightCtrl(it->nIndex);
            cxFixed -= it->nSpace;
            cxFixed -= m_iChildPadding;
            it->nSpace = 0;

            if (it->pCtrl->GetFixedWidth() == 0) { --nAdjustables; }
        }
    }

    // Place elements
    if (nAdjustables > 0) { cxExpand = std::max<int>(0, (szAvailable.cx - cxFixed) / nAdjustables); }

    // Position the elements
    SIZE szRemaining = szAvailable;
    int iPosX = rc.left;

    if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
    {
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
    }

    int iEstimate = 0;
    int iAdjustable = 0;
    int cxFixedRemaining = cxFixed;
    int cxNeeded = 0;

    for (int i = 0; i < m_items.GetSize(); i++)
    {
        pCtrl = static_cast<CControlUI *>(m_items[i]);

        if (!pCtrl->IsVisible()) { continue; }

        if (pCtrl->IsFloat())
        {
            SetFloatPos(i);
            continue;
        }

        iEstimate += 1;
        RECT rcPadding = pCtrl->GetPadding();
        szRemaining.cx -= rcPadding.left;

        szControlAvailable = szRemaining;
        szControlAvailable.cy -= rcPadding.top + rcPadding.bottom;
        iControlMaxWidth = pCtrl->GetFixedWidth();
        iControlMaxHeight = pCtrl->GetFixedHeight();

        if (iControlMaxWidth <= 0) { iControlMaxWidth = pCtrl->GetMaxWidth(); }

        if (iControlMaxHeight <= 0) { iControlMaxHeight = pCtrl->GetMaxHeight(); }

        if (szControlAvailable.cx > iControlMaxWidth) { szControlAvailable.cx = iControlMaxWidth; }

        if (szControlAvailable.cy > iControlMaxHeight) { szControlAvailable.cy = iControlMaxHeight; }

        cxFixedRemaining = cxFixedRemaining - (rcPadding.left + rcPadding.right);

        if (iEstimate > 1) { cxFixedRemaining = cxFixedRemaining - m_iChildPadding; }

        SIZE sz = pCtrl->EstimateSize(szControlAvailable);

        if (pCtrl->GetFixedWidth() == 0 || sz.cx == 0)
        {
            iAdjustable++;
            sz.cx = cxExpand;

            // Distribute remaining to last element (usually round-off left-overs)
            if (iAdjustable == nAdjustables)
            {
                sz.cx = std::max<int>(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
            }

            if (sz.cx < pCtrl->GetMinWidth()) { sz.cx = pCtrl->GetMinWidth(); }

            if (sz.cx > pCtrl->GetMaxWidth()) { sz.cx = pCtrl->GetMaxWidth(); }
        }
        else
        {
            if (sz.cx < pCtrl->GetMinWidth()) { sz.cx = pCtrl->GetMinWidth(); }

            if (sz.cx > pCtrl->GetMaxWidth()) { sz.cx = pCtrl->GetMaxWidth(); }

            cxFixedRemaining -= sz.cx;
        }

        sz.cy = pCtrl->GetMaxHeight();

        if (sz.cy == 0) { sz.cy = szAvailable.cy - rcPadding.top - rcPadding.bottom; }

        if (sz.cy < 0) { sz.cy = 0; }

        if (sz.cy > szControlAvailable.cy) { sz.cy = szControlAvailable.cy; }

        if (sz.cy < pCtrl->GetMinHeight()) { sz.cy = pCtrl->GetMinHeight(); }

        CVecCtrlInfo::iterator it = std::find(vecCtrls.begin(), vecCtrls.end(), TCtrlInfo(pCtrl));
        int n = (it != vecCtrls.end() && 0 < it->nSpace) ? it->nSpace : sz.cx;
        n = sz.cx - n;
        sz.cx -= n;
        cxFixedRemaining += n;

        UINT iChildAlign = GetChildVAlign();

        if (iChildAlign == DT_VCENTER)
        {
            int iPosY = (rc.bottom + rc.top) / 2;

            if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            {
                iPosY += m_pVerticalScrollBar->GetScrollRange() / 2;
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, iPosY - sz.cy / 2, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy - sz.cy / 2 };
            pCtrl->SetPos(rcCtrl, false);
        }
        else if (iChildAlign == DT_BOTTOM)
        {
            int iPosY = rc.bottom;

            if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            {
                iPosY += m_pVerticalScrollBar->GetScrollRange();
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, iPosY - rcPadding.bottom - sz.cy, iPosX + sz.cx + rcPadding.left, iPosY - rcPadding.bottom };
            pCtrl->SetPos(rcCtrl, false);
        }
        else
        {
            int iPosY = rc.top;

            if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
            {
                iPosY -= m_pVerticalScrollBar->GetScrollPos();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + sz.cx + rcPadding.left, iPosY + sz.cy + rcPadding.top };
            pCtrl->SetPos(rcCtrl, false);
        }

        iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
        cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
        szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
    }

    cxNeeded += (nEstimateNum - 1) * m_iChildPadding;

    // Process the scrollbar
    ProcessScrollBar(rc, cxNeeded, cyNeeded);
}

void CHWeightLayoutUI::ResetWeightCtrlState(void)
{
    for (int i = 0; i < m_items.GetSize(); ++i)
    {
        CControlUI *pCtrl = static_cast<CControlUI *>(m_items[i]);

        if (pCtrl->IsFloat()) { continue; }

        pCtrl->SetInternVisible(true);
    }
}

bool CHWeightLayoutUI::HideMinHeightCtrl(int nIndex)
{
    if (nIndex >= m_items.GetSize()) { return false; }

    CControlUI *pCtrl = static_cast<CControlUI *>(m_items[nIndex]);

    if (pCtrl && pCtrl->IsVisible())
    {
        pCtrl->SetInternVisible(false);
        return true;
    }

    return false;
}

}
