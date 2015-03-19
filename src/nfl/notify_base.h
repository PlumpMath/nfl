#ifndef __NOTIFY_BASE_H__
#define __NOTIFY_BASE_H__

#include "nfl_base.h"


namespace nfl
{
/** 
 * @class	nofity_base
 * 
 * @brief	
 *		���߳�֪ͨ
 * detail...
 *		���ô�����߳��������eventloop�̣߳������ݾ���֪ͨ���ݡ�
 *      ������϶��������ݸ����ӵ�֪ͨ��
 *    
 * @author	mklong
 * @date	2014/6/10
 * 
 * @see		
 * 
 * @par ��ע��
 * 
 */
	class nofity_base : public nfl_nocopy
	{
	public:

		nofity_base():cb_(NULL),
			  cb_arg_(NULL)
		{
			socket_pair_[0] = -1;
			socket_pair_[1] = -1;
		}

		int open(libevent_base *bs)
		{
			if (cb_ == NULL || cb_arg_ == NULL){
				return -1;
			}
			
			//notify pipe (socketpair) init
			if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair_)<0){
				return -1;
			}

			evutil_make_socket_nonblocking(socket_pair_[0]);
			evutil_make_socket_nonblocking(socket_pair_[1]);

			//��linux�������socketpair��Ҫ����SNDBUF��RCVBUF,��ֹд��������
			int sndbuf = 128*1024;
			ACE_OS::setsockopt((ACE_HANDLE)socket_pair_[1],SOL_SOCKET,SO_SNDBUF,(const char *)&sndbuf,sizeof(int));

			int rcvbuf = 128*1024;
			ACE_OS::setsockopt((ACE_HANDLE)socket_pair_[0],SOL_SOCKET,SO_RCVBUF,(const char *)&rcvbuf,sizeof(int));

			event_assign(&notify_event_,bs,socket_pair_[0],EV_READ | EV_PERSIST, notify_cb, this);
			event_add(&notify_event_,NULL);
			return 0;
		}

		int close()
		{
			//close socketpair
			if (socket_pair_[0] != -1){
				event_del(&notify_event_);
				EVUTIL_CLOSESOCKET(socket_pair_[0]);

				if (socket_pair_[1] != -1){
					EVUTIL_CLOSESOCKET(socket_pair_[1]);
				}
				socket_pair_[0] = -1;
				socket_pair_[1] = -1;
			}
			return 0;
		}

		void set_notify_cb(ev_cb_fn cb,void *arg)
		{
			cb_ = cb;
			cb_arg_ = arg;
		}

		int notify(const char* msg,size_t len)
		{
			if (socket_pair_[1] == -1){
				return -1;
			}
			
			/*
			*	��ѹ��д�����������д��SNDBUF,����ͨ��2���ֶι��
			*  1������SNDBUF
			*	2��д��whileѭ��д
			*/
			
			int ret = send(socket_pair_[1],msg,len,0);
			if (ret <0){
				if(errno == EAGAIN){
					return 1;
				}else{
					return -1;
				}
			}

			return 0;
		}

		static void notify_cb(evutil_socket_t s, short what, void *ptr)
		{
			nofity_base * nb = (nofity_base*) ptr;
			if (nb->cb_ == NULL){
				return;
			}else{
				nb->cb_(s,what,nb->cb_arg_);
			}
		}

	private:
		//cb
		ev_cb_fn cb_;
		void * cb_arg_;

		libevent_event notify_event_;
		evutil_socket_t socket_pair_[2];
	};

}

#endif /* __NOTIFY_BASE_H__ */
