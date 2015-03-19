#ifndef __LIBEVENT_REACTOR_H__
#define __LIBEVENT_REACTOR_H__

#include "nfl_base.h"
#include "cq_base.h"
#include "event_loop_thread_base.h"

namespace nfl
{

/** 
 * @class	libevent_reactor
 * 
 * @brief	
 *		����������event_base��notify��ϣ���������ʵ�ʵ��߳���
 *	����ʱ���̲߳�����notify֪ͨ���жϵ�eventloop�����process_notify
 *
 * detail...
 * 
 * @author	mklong
 * @date	2014/6/12
 * 
 * @see		
 * 
 * @par ��ע��
 * 
 */

	class libevent_reactor: public event_loop_thread_base
	{
	public:
		libevent_reactor();


/** 
 * @brief 	open
 * 
 * 	���û���open��ʼ��������ʼ�����ಿ��
 * 
 * @author	mklong
 * @date	2014/6/12
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
		virtual int open();

/** 
 * @brief 	close
 * 
 * 	���ʼ�����࣬�����û���close���ʼ��
 * 
 * @author	mklong
 * @date	2014/6/12
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
		virtual int close();

/** 
 * @brief 	notify_eh
 * 
 * 	���߳�֪ͨevent_handler,��ص�event_handler��handle_exception
 * mask�����õ�event_handler::notify_mask_,�ɴ˿��Դ��������¼��ص�.
 *	�����߱���ȷ��eh�ڵ�ǰevent_loop��
 *
 * @author	mklong
 * @date	2014/6/12
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	event_handler*eh	- [in] 
 * @param	short mask = EH_MASK::EXCEPT_MASK	- [in] 
 * @see		
 */
		int notify_eh(event_handler*eh,short mask = nfl::EXCEPT_MASK);


/** 
 * @brief 	add_eh
 * 
 * 	���߳�����¼�
 * 
 * @author	mklong
 * @date	2014/6/12
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	event_handler*eh	- [in] 
 * @see		
 */
		int add_eh(event_handler*eh);

	protected:

/** 
 * @brief 	process_notify
 * 
 * 	ʵ����֪ͨ�¼��Ĵ�����Ҫ������event_handler�������Ѵ��ڵ�event_handler��
 * ֪ͨevent_loop�߳��˳�
 * 
 * @author	mklong
 * @date	2014/6/12
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @param	ACE_HANDLE s	- [in] 
 * @see		
 */
		virtual int process_notify(ACE_HANDLE s);

	private:

		//�¼�֪ͨ����
		cq_base<event_handler,ACE_SYNCH> cq_eh_;

	};


	/************************************************************************/
	/*        inline func                                                               */
	/************************************************************************/


	ACE_INLINE int libevent_reactor::notify_eh( event_handler*eh,short mask /*= EH_MASK::EXCEPT_MASK*/ )
	{
		eh->notify_mask(mask);
		cq_eh_.cq_push(eh);

		char c = NOTIFY_EVENT;
		//����1��˵��д���ˣ�����֪ͨģʽֻ�𵽴��������ã�����
		//��ǰ֪ͨ�����ǿ����𵽴������õ�

		return this->notify_event(&c) >= 0?0:-1;
	}

	ACE_INLINE int libevent_reactor::add_eh( event_handler*eh )
	{
		return notify_eh(eh,OPEN_MASK);
	}

}

#endif /* __LIBEVENT_REACTOR_H__ */
