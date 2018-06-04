#ifndef __GIFINFO_H__
#define __GIFINFO_H__

#pragma once

namespace DuiLib {

class CGifInfo
{
public:
    explicit CGifInfo(int nFrameCnt);
    ~CGifInfo(void);

    int GetFrameCount(void);                // ������֡��
    void AddFrame(TImageInfo *ptFrame);     // ���һ֡ͼ��
    void SetCurFrame(int nCurFrame);        // ���õ�ǰ֡
    TImageInfo *GetCurFrame(void);          // ���ص�ǰ֡
    TImageInfo *GetNextFrame(void);         // ������һ֡
    TImageInfo *GetFrame(int nIdx);         // ����ָ��֡

private:
    CDuiPtrArray    vGifFrame;              // �洢 GIF ����֡��Ϣ��ָ��
    int             nCurFrame;              // ��ǰ���ŵ��ڼ�֡
    bool            bIsDeleting;
};
}

#endif // __GIFINFO_H__
