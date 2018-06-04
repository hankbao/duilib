/// \copyright Copyright(c) 2018, SuZhou Keda Technology Co., All rights reserved.
/// \file dataobjecthelper.h
/// \brief Ole���ݶ�������
///
/// RegisterClipboardFormat ����ע���Լ������ݸ�ʽ
/// \author zhuyadong
/// Contact: zhuyadong@kedacom.com
/// \date 2018-04-28
/// \note
/// -----------------------------------------------------------------------------
/// �޸ļ�¼��
/// ��  ��        �汾        �޸���        �߶���    �޸�����
///
#pragma once

namespace DuiLib {

// ֧�ֵ���������
// 1. ��׼��������������
#define ECF_TEXT             1          // ANSI �ı�
#define ECF_BITMAP           2          // λͼ
#define ECF_METAFILEPICT     3
#define ECF_SYLK             4
#define ECF_DIF              5
#define ECF_TIFF             6
#define ECF_OEMTEXT          7
#define ECF_DIB              8
#define ECF_PALETTE          9
#define ECF_PENDATA          10
#define ECF_RIFF             11
#define ECF_WAVE             12
#define ECF_UNICODETEXT      13         // UNICODE �ı�
#define ECF_ENHMETAFILE      14

#if(WINVER >= 0x0400)
    #define ECF_HDROP            15     // ��Դ�������ļ���ק��ͨ�������壩
    #define ECF_LOCALE           16
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
    #define ECF_DIBV5            17
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0500)
    #define ECF_MAX              18
    #elif(WINVER >= 0x0400)
    #define ECF_MAX              17
#else
    #define ECF_MAX              15
#endif

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E

// 2. "Private" formats don't get GlobalFree()'d
#define ECF_PRIVATEFIRST     0x0200
#define ECF_STRUCTDATA       0x0201
#define ECF_PRIVATELAST      0x02FF

// 3. "GDIOBJ" formats do get DeleteObject()'d
#define ECF_GDIOBJFIRST      0x0300
#define ECF_GDIOBJLAST       0x03FF

enum TDVASPECT
{
    EDVASPECT_CONTENT = 1,
    EDVASPECT_THUMBNAIL = 2,
    EDVASPECT_ICON = 4,
    EDVASPECT_DOCPRINT = 8
};


// �洢ý�����ͣ��� TYMED �Ķ���һ��
enum EMTypeMedium
{
    ETYMED_NULL = 0,
    ETYMED_HGLOBAL = 1,
    ETYMED_FILE = 2,
    ETYMED_ISTREAM = 4,
    ETYMED_ISTORAGE = 8,
    ETYMED_GDI = 16,
    ETYMED_MFPICT = 32,
    ETYMED_ENHMF = 64,
};

struct DUILIB_API TDragData
{
    WORD    m_wClipFormat;  // ֧�ֵ���������
    WORD    m_wAspect;      // ��� TDVASPECT(DVASPECT)
    int     m_eTypeMedium;  // �洢ý�����ͣ���� EMTypeMedium(TYMED)
    void   *m_pData;        //
};

// �ַ����洢�� HGLOBAL
HGLOBAL DUILIB_API StringToHandle(LPCWSTR lpcstrText, int nTextLen = -1);
HBITMAP DUILIB_API CopyHBitmap(HDC hDC, HBITMAP hBmpSrc);

class DUILIB_API COleDataHelper
{
public:
    COleDataHelper(void);
    ~COleDataHelper(void);

    // ������ݡ�
    bool SetText(CDuiString sText);                         // ���� �ı�
    bool SetBitmap(HBITMAP hBmp);                           // ���� HBITMAP λͼ
    bool SetCustomData(void *pData, DWORD dwLen, WORD wCF); // ���� �Զ�������
    bool SetCustomGDI(HGDIOBJ hGDI, WORD wCF);              // ���� HBITMAP ����� GDI ����

    // ��ȡ����
    CDuiString GetText(void);                       // ���� �ı�
    HBITMAP GetBitmap(void);                        // ���� HBITMAP λͼ
    CDuiPtrArray GetFileList(void);                 // ���� �ļ��б�(TCHAR*)����Ҫ free �ͷ��ڴ档������Դ��������ק���ļ��б�
    void *GetCustomData(WORD wCF, DWORD &dwLen);    // ���� �Զ������ݡ���Ҫ free �ͷ��ڴ�
    HGDIOBJ GetCustomGDI(WORD wCF);                 // ���� HBITMAP ����� GDI ����

    // bRelease : TRUE ������ɺ��û������ͷ���Դ��FALSE ��ʾ�����ڲ��Ḵ��һ����Դ��pData���û��ͷ�
    bool SetData(void *pData, int nTM, WORD wCF, WORD wAspect = EDVASPECT_CONTENT, BOOL bRelease = TRUE);
    void *GetData(WORD wCF, int nTM, WORD wAspect = EDVASPECT_CONTENT);

    // IDataObject ����ö��
    bool GetNext(WORD &wCF, WORD &wAspect, int &nTM);   // ��0��ʼö����������
    bool Reset(void);                                   // ��������Ϊ0�����¿�ʼö��

    bool HasText(void);                                 // ���� �ı��Ƿ����
    bool HasBitmap(void);                               // ���� λͼ�Ƿ����
    bool HasFileList(void);                             // ���� �ļ��б��Ƿ����
    bool HasCustomData(void);                           // ���� �Զ��������Ƿ����
    bool HasCustomGDI(void);                            // ���� HBITMAP ����� GDI �����Ƿ����

    // ��קʱ���ص�ǰ��״̬��DROPEFFECT_NONE | DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK | DROPEFFECT_SCROLL
    DWORD GetEffect(void) { return m_dwEffect; }
    void SetEffect(DWORD dwEffect) { m_dwEffect = dwEffect; }
    // ��קʱ����״̬��MK_CONTROL ��
    DWORD GetKeyState(void) { return m_dwKeyState; }

protected:
    friend class CPaintManagerUI;
    friend class CControlUI;

    IDataObject *CreateDataObject(void);
    bool CreateEnumFormatEtc(void);                     // ������������ö�ٽӿ�
private:
    IDataObject    *m_pDataObj;
    bool            m_bAutoRelease;

    IEnumFORMATETC *m_pEnumFmt;

    DWORD           m_dwKeyState;
    DWORD           m_dwEffect;
};

}