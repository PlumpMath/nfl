#ifndef __NFL_ACCEPT_HANDLER_H__
#define __NFL_ACCEPT_HANDLER_H__


#include "libevent_handler.h"
#include "ace/INET_Addr.h"

namespace nfl
{

	struct accept_cfg
	{
		enum{
			UNSET_VALUE = -1,
		};

		accept_cfg():listen_fd(ACE_INVALID_HANDLE),
			deferred_accept(false),
			deferred_timeout(60),
			reuse_addr(true),
			backlog(511),
			rcvbuf(UNSET_VALUE),
			sndbuf(UNSET_VALUE),
			keepalive(false),
			keepidle(UNSET_VALUE),
			keepintvl(UNSET_VALUE),
			keepcnt(UNSET_VALUE)
		{
		}

		//listen socket opt
		ACE_HANDLE listen_fd;
		ACE_INET_Addr addr;

		//linux opt ,see TCP_DEFER_ACCEPT
		bool deferred_accept;
		int deferred_timeout;

		bool reuse_addr;
		int backlog;

		bool keepalive;

		//keepalive reserved
		int keepidle;
		int keepintvl;
		int keepcnt;

		//socket opt 
		int rcvbuf;
		int sndbuf;

	};

	template<class EVENT_HANDLER>
	class accept_handler : public event_handler
	{
	public:

		accept_handler();

/** 
 * @brief 	open
 * 
 * 		ͨ���������ʼ��listen fd��������ԣ��ú��������Ὣ
 *	��event_handler���뵽reactor�У���Ҫ���û����open(),����
 *	��open()������ʵ��ע����¼����ڴ�֮ǰ�ⲿ��Ҫ���ú�reactor()
 *  ���ҵ���set_dispatcher()��
 *	
 * @author	mklong
 * @date	2014/6/16
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	accept_cfg *cfg	- [in] 
 * @see		
 */
		virtual	int open(accept_cfg *cfg);

/** 
 * @brief 	close
 * 
 * 	��reactor���Ƴ��������ʼ��������ݽṹ
 * 
 * @author	mklong
 * @date	2014/6/16
 * @return	virtual int	- Return 0 in success, otherwise return error code.
 * @see		
 */
		virtual int close();

/** 
 * @brief 	set_dispatcher
 * 
 * 	��handle_input�е���accept������event_handler����ͨ��func����event_handler����
 * 
 * @author	mklong
 * @date	2014/6/16
 * @return	No return value.
 * @param	ev_dispatcher_fn func	- [in] 
 * @param	void * arg	- [in] 
 * @see		
 */
		void set_dispatcher(ev_dispatcher_fn func,void * arg);


		size_t accept_count()const;
	protected:

		virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

		virtual int handle_close (ACE_HANDLE fd,short close_mask);

	protected:
		//dispatcher
		ev_dispatcher_fn dispatcher_;
		void * arg_;

		//socket opt 
		int rcvbuf_;
		int sndbuf_;
		bool keepalive_;

		size_t accept_count_;

		//	size_t max_fds_;
		cq_base<EVENT_HANDLER,ACE_SYNCH> cq_eh_;

	};

}

//template need include cpp 
#include "nfl_accept_handler.cpp"

#endif /* __NFL_ACCEPT_HANDLER_H__ */
