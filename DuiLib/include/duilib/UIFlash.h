/*
    �������ڣ� 2012/11/05 15:09:48
    ���ߣ�     daviyang35@gmail.com
    ������     FlashUI
*/
#ifndef __UIFLASH_H__
#define __UIFLASH_H__
#pragma once


namespace ShockwaveFlashObjects {
struct IShockwaveFlash;
}

namespace DuiLib {
class CFlashEvents;

class DUILIB_API CFlashUI : public CActiveXUI, public ITranslateAccelerator
{
public:
    CFlashUI(void);
    ~CFlashUI(void);

    ShockwaveFlashObjects::IShockwaveFlash *GetShockwaveFlash(void);

    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    // ����
    CDuiString GetAlign(void);
    void SetAlign(CDuiString sAlign);
    CDuiString GetWMode(void);
    void SetWMode(CDuiString sWMode);
    CDuiString GetMovie(void);
    void SetMovie(CDuiString sMovie);
    CDuiString GetBase(void);
    void SetBase(CDuiString sBase);
    CDuiString GetScale(void);
    void SetScale(CDuiString sScale);

    bool IsPlaying(void);


    // ��Ϣ֪ͨ����
    int GetState(void)    { return m_nState; }          // ���ص�ǰ״̬
    int GetProgress(void) { return m_nProgress; }       // ���ص�ǰ����
    CDuiString GetCommand(void)  { return m_sCmd; }     // ��������
    CDuiString GetArgument(void) { return m_sArg; }     // ���ز���

protected:
    virtual bool DoCreateControl();

private:
    // ӰƬ״̬�仯֪ͨ�����ܵ�ֵ��
    // 0 ������������
    // 1 ����δ��ʼ��
    // 2 ����������
    // 3 �������ڽ���
    // 4 ������� ����
    HRESULT OnReadyStateChange(long newState);
    // ���Ž���
    HRESULT OnProgress(long percentDone);
    // fscommand ����
    // ȫ������          "fullscreen","true"
    // �����Ҽ��˵�       "showmenu","false"
    // ��ֹӰƬ����       "allowscale","false"
    // ʹ���̳�����Ч     "trapallkeys","true"
    // ���ÿ�ִ���ļ�     "exec","��Ҫ�򿪵��ļ�·��"
    // �رղ�����        "quit"
    // ����������ı��ļ�  "save","arg"
    HRESULT FSCommand(CDuiString &command, CDuiString &args);
    // FSCommand �¼���࣬�� FSCommand��ͬ���ǣ�����¼������з���ֵ��������һ��XML��ʽ���ַ�������ʽ����
    // "<invoke name='%s'returntype='xml'><arguments><string>%s</string></arguments></invoke>"
    HRESULT FlashCall(CDuiString &request);

    virtual void ReleaseControl();
    HRESULT RegisterEventHandler(BOOL inAdvise);

    // ITranslateAccelerator
    // Duilib��Ϣ�ַ���Flash
    virtual LRESULT TranslateAccelerator(MSG *pMsg);

private:
    friend class CFlashEvents;
    DWORD m_dwCookie;
    ShockwaveFlashObjects::IShockwaveFlash *m_pFlash;
    CFlashEvents       *m_pFlashEvents;

    // ����
    CDuiString  m_sAlign;       // �������ԣ�L=Left T=Top R=Right B=Bottom ���ַ������
    CDuiString  m_sWMode;       // �ؼ��Ĵ���ģʽ
    CDuiString  m_sMovie;       // ӰƬ·����URL / �����ԴĿ¼��·����
    CDuiString  m_sBase;        // ָ������ӰƬ���·���Ļ���ַ����ӰƬ��������Ҫ�������ļ�����ͬһĿ¼ʱ�ر����á�Ĭ��Ϊ��ǰӰƬ����·����
    CDuiString  m_sScale;       // ӰƬ����ģʽ

    int         m_nProgress;
    int         m_nState;
    CDuiString  m_sCmd;
    CDuiString  m_sArg;
};
}

#endif // __UIFLASH_H__
