#ifndef __NFL_BASE_H__
#define __NFL_BASE_H__
/** 
 * @file	nfl_base.h
 * @brief	
 *		
 * detail...
 *
 *		������Ŀ�Ĵ���˼·�ǲο�memcached��˼·��������
 *	����libevent���̰߳汾��eventloop�������������ǽ��C
 *	���븴�õ����⣬ͬʱ���ָ�Ч�������memcached�еĿ�ܲ�
 *  �ֽ����˳�������C�����ж��ڴ�Ĺ����Ҫ����ߣ��Գ�
 *  �������һ�����Ѷȣ����Կ����ڴ������ȥ����������ͨ��
 *	����������Ϊ�˲�������װ������������ģ����Ⲣδʹ��̫��
 *  c++����ʱ�߼����ԣ��ڲ���󲿷ֶ���ʹ�þ�̬������Э����
 *  
 * @author	mklong
 * @version	1.0
 * @date	2014/2/21
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2014/2/21	<td>mklong	<td>Create this file</tr>
 * </table>
 * 
 */

//libevent include
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"

//ace include
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

#include "nfl_os.h"


namespace nfl
{
	//class Declaration
 	class event_handler;
 	class libevent_reactor;

	//libevent event callback  

	typedef void (*ev_cb_fn)(evutil_socket_t, short, void *arg);

	//dispather
	typedef int (*ev_dispatcher_fn)(event_handler*eh,void *ptr);

	//typedef
	typedef struct event_base libevent_base;
	typedef struct event libevent_event;


//disable copy constructor
class nfl_nocopy
{
public:
	/// Default constructor
	nfl_nocopy (void){};

private:
	nfl_nocopy (const nfl_nocopy &);
	nfl_nocopy &operator= (const nfl_nocopy &);
};


//event handler mask
enum EH_MASK
{	
	//EV_TIMEOUT	0x00
	NULL_MASK = 0x00,
	//EV_TIMEOUT	0x01
	TIMER_MASK = 0x01,
	//EV_READ		0x02
	READ_MASK = 0x02,
	//EV_WRITE	0x04
	WRITE_MASK = 0x04,
	//EV_SIGNAL	0x08
	SIGNAL_MASK = 0x08,
	//EV_PERSIST	0x10
	PERSIST_MASK = 0x10,
	//EV_ET       0x20
	ET_MASK = 0x20,

	//nfl mask
	EXCEPT_MASK = 0x40,
	CLOSE_MASK = 0x80,
	OPEN_MASK = 0x100,

};

}




#endif /* __NFL_BASE_H__ */
