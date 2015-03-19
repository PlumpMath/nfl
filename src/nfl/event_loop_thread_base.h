#ifndef __EVENT_LOOP_THREAD_BASE_H__
#define __EVENT_LOOP_THREAD_BASE_H__


#include "nfl_base.h"
#include "notify_base.h"
#include "libevent_handler.h"

namespace nfl
{

/** 
 * @class	event_loop_thread_base
 * 
 * @brief	
 *		event loop�̻߳���
 * detail...
 *		  ��libevent��event_baseΪ��������װ��һ���߳�	һ��event_base��ģ�ͣ�
 * ���ڽ�event_base��ʼ��ΪEVENT_BASE_FLAG_NOLOCK ������ģʽ�����Զ���
 * ��ʹ��libevent api�������̰߳�ȫ��.
 *		Ϊ������Ч�ʵ� ��ͬʱ������ʧ�����ԣ���ÿ��event loop�߳�������һ���¼�
 * ֪ͨ��nofity_base��ʹ�ø�������������Ĵ��event loop�������¼�֪ͨ��������
 * �̰�ȫ���⡣libevnet����Ҳ��ʹ�����ֻ����������źŵĴ�ϣ���ϧ��������ϲ�û�н�
 * ���õ�����֪ͨ��
 *
 *
 * @author	mklong
 * @date	2014/6/11
 * 
 * @see		
 * 
 * @par ��ע��
 * 
 */

class event_loop_thread_base :public ACE_Task_Base
{
public:
	enum NOTIFY_FLAG{
		NOTIFY_EVENT = 'n',
		END_EVENT_LOOP = 'e',
	};

	event_loop_thread_base();


/** 
 * @brief 	open
 * 
 * 		��ʼ��event_base��notify
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @see		
 */
	virtual int open();

/** 
 * @brief 	close
 * 
 * 	���ʼ��event_base��notify
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @see		
 */
	virtual int close();


/** 
 * @brief 	end_event_loop
 * 
 *	���߳��ж�event loop��ͨ��notifyд���˳��¼�
 * event loop�߳��յ�����Ҫ����end_event_loop_cb
 * ���˳���
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
	int end_event_loop();


/** 
 * @brief 	end_event_loop_cb
 * 
 * 	����event loop�߳��ڲ���ֹѭ����ʹ��event loop �߳��˳�
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
	int end_event_loop_cb();


/** 
 * @brief 	notify_cb
 * 
 * 	���ݸ�notify��ʹ�ã���Ϊ֪ͨ�ص������������Ǿ�̬���������Իᴫ��ָ����Ϊ����
 *  ��notify�����ú���ʱ����������process_notify����������ݾ��������д
 *	 process_notify
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	No return value.
 * @param	evutil_socket_t s	- [in] 
 * @param	short what	- [in] 
 * @param	void *ptr	- [in] 
 * @see		
 */
	static void notify_cb(evutil_socket_t s, short what, void *ptr);


/** 
 * @brief 	notify_event
 * 
 * 	���߳�֪ͨ�¼������鴫�̶������ֽڣ�
 *	�Ա����ڴ���ص���ʱ��ÿ����ȡ�̶��ĳ��ȣ�
 *  NOTIFY_FLAG�ǽ���ļ����¼�����
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	const char* msg	- [in] 
 * @param	size_t len	- [in] 
 * @see		
 */
	int notify_event(const char* msg,size_t len =1);

/** 
 * @brief 	base
 * 
 * 	
 * 
 * @author	mklong
 * @date	2014/6/12
 * @return	libevent_base * 
 * @see		
 */
	libevent_base * base()const;

protected:

/** 
 * @brief 	process_notify
 * 
 * 		����֪ͨ�ص�����������ʵ��
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @param	ACE_HANDLE s	- [in] 
 * @see		
 */
	virtual int process_notify(ACE_HANDLE s);

/** 
 * @brief 	svc
 * 
 * 	event loop �̺߳�����Ĭ��ֱ��ִ��event_loop
 * 
 * @author	mklong
 * @date	2014/6/11
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @see		
 */
	virtual int svc();

protected:

	libevent_base * base_;
	nofity_base notify_;
};




/************************************************************************/
/* Class Implementation                                                                     */
/************************************************************************/

ACE_INLINE int event_loop_thread_base::end_event_loop_cb()
{
	//exit 
	return event_base_loopexit(base_,NULL);
}

ACE_INLINE libevent_base * event_loop_thread_base::base() const
{
	return this->base_;
}

ACE_INLINE int event_loop_thread_base::notify_event(const char* msg,size_t len)
{
	return this->notify_.notify(msg,len);
}

}

#endif /* __EVENT_LOOP_THREAD_BASE_H__ */
