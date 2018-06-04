/// \copyright Copyright(c) 2018, SuZhou Keda Technology Co., All rights reserved.
/// \file droptarget.h
/// \brief �Ϸ�Ŀ��
///
///
/// \author zhuyadong
/// Contact: zhuyadong@kedacom.com
/// \date 2018-04-26
/// \note
/// -----------------------------------------------------------------------------
/// �޸ļ�¼��
/// ��  ��        �汾        �޸���        �߶���    �޸�����
///
#pragma once
#include <OleIdl.h>
#include <Shobjidl.h>

namespace DuiLib {

class CDropTarget : public IDropTarget
{
public:
    CDropTarget(void);

    bool DragDropRegister(IDuiDropTarget *pDuiDropTarget, HWND hWnd, DWORD AcceptKeyState = MK_LBUTTON);
    bool DragDropRevoke(HWND hWnd);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    //����
    HRESULT STDMETHODCALLTYPE DragEnter(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt,
                                        __RPC__inout DWORD *pdwEffect);
    //�ƶ�
    HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);
    //�뿪
    HRESULT STDMETHODCALLTYPE DragLeave();
    //�ͷ�
    HRESULT STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt,
                                   __RPC__inout DWORD *pdwEffect);

private:
    ~CDropTarget(void);

    HWND                m_hWnd;
    IDropTargetHelper  *m_piDropHelper;
    bool                m_bUseDnDHelper;
    DWORD               m_dAcceptKeyState;
    ULONG               m_lRefCount;
    IDuiDropTarget     *m_pDuiDropTarget;
};

}