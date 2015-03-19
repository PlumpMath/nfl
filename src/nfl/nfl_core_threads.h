/** 
 * @file	nfl_core_threads.h
 * @brief	
 *
 * �����ܺ����̲߳���
 *
 * detail...
 *		�ڵ��߳�ģʽ�£�accept_handler��event_handler����
 *	��ͬһ��libevent_reactor�£����߳�ģʽ�£�accept_handler��
 * ��һ��libevent_reactor�У���event_dispatcher����event_handler
 * ���ɵ�������libevent_reactor�С�
 *
 * @author	mklong
 * @version	1.0
 * @date	2014/6/11
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2014/6/11	<td>mklong	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __NFL_CORE_THREADS_H__
#define __NFL_CORE_THREADS_H__


#include "nfl_base.h"
#include "cq_base.h"
#include "libevent_reactor.h"
#include <vector>

namespace nfl
{

	class sock_thread_pool :public nfl_nocopy
	{

	public:
		
		sock_thread_pool();
		
		int open(size_t thread_num = 0);
		
		int close();

		libevent_reactor* get_accept_reactor()const;

		static ev_dispatcher_fn get_dispatcher();

		//called only by accept_handler
		static  int event_dispatcher(event_handler*eh,void *ptr);

		void non_accept_mode();
	protected:
		
		int dispatch(event_handler*eh);

	private:
		size_t  ref_;
		size_t n_threads_;
		size_t cur_pos_;

		bool non_accept_mode_;

		std::vector<libevent_reactor*> work_threads_;
		ACE_Thread_Mutex lock_;
	};




	/************************************************************************/
	/*        inline func                                                               */
	/************************************************************************/

	ACE_INLINE int sock_thread_pool::dispatch( event_handler*eh )
	{
		if (n_threads_ == 1){
			//single thread
			if (work_threads_[0]->add_eh(eh) != 0){
				return -1;
			}

			return 0;
		}

		if (non_accept_mode_){
			//work_threads_[0] dispath to other work_threads_
			cur_pos_ %= n_threads_ ;
			if (work_threads_[cur_pos_]->add_eh(eh) != 0){
				return -1;
			}

			cur_pos_++;
			return 0;
		}else{
			//work_threads_[0] dispath to other work_threads_
			cur_pos_ %= n_threads_ - 1;
			if (work_threads_[cur_pos_+1]->add_eh(eh) != 0){
				return -1;
			}

			cur_pos_++;
			return 0;
		}
	}

}

#endif /* __NFL_CORE_THREADS_H__ */
