#ifndef __OBSERVER_PATTERN_H__
#define __OBSERVER_PATTERN_H__
#pragma once

namespace DuiLib {

//�۲���
class CSubjectBase;
class DUILIB_API IObserver
{
public:
    /** \brief
     *
     * \param p1 WPARAM                     ����1
     * \param NULL WPARAM p2=               ����2
     * \param NULL LPARAM p3=               ����3
     * \param NULL CSubjectBase *pSub=      �������ָ��
     * \return virtual void
     *
     */
    virtual void OnSubjectUpdate(WPARAM p1, WPARAM p2 = NULL, LPARAM p3 = NULL, CSubjectBase *pSub = NULL) = 0;
};

//����
namespace sub {
class CSubjectImpl;
}

class DUILIB_API CSubjectBase
{
public:
    CSubjectBase(void);
    virtual ~CSubjectBase(void);
    void AddObserver(IObserver *o);
    void RemoveObserver(IObserver *o);
    // ����˵����
    // p1/p2/p3  ֪ͨ���ݡ�ʹ��ָ�룬���Դ���32/64λ��������ṹ��/�����ָ�룬32/64λϵͳͨ��
    // o         �۲���ָ�룬���ڶ���֪ͨ
    void NotifyObserver(WPARAM p1, WPARAM p2 = NULL, LPARAM p3 = NULL, IObserver *o = NULL);

    // ֪ͨ���ͷ�������/���� �����۲���
    void SetNotifyDirection(bool bForward = true);
    // �����۲���
    IObserver *GetFirst(void);
    IObserver *GetNext(void);

private:
    sub::CSubjectImpl  *m_pSub;
};


}

#endif // __OBSERVER_PATTERN_H__
