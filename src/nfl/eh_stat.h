#ifndef __EH_STAT_H__
#define __EH_STAT_H__
/** 
 * @file	eh_stat.h
 * @brief	
 *
 *
 * detail...
 * 
 * nfl::event_handler ���и�id����id����Ψһ�ԣ�����ͨ����id�������������event_handler��
 * ͨ�������������ǻ���map�����ݣ����ǲ���������O(lgn)�����ļ��е��࣬����ʱ�临�ӶȴﵽO(1)��
 * �������ڴ�����ϣ�����һ�ε�λ���������Ƶ�������ͷ�С���ڴ�������
 * 
 *
 * @author	mklong
 * @version	1.0
 * @date	2014/12/10
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2014/6/13	<td>mklong	<td>Create this file</tr>
 * </table>
 * 
 */

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Guard_T.h"


template<typename _INST>
struct  eh_state_node
{
	eh_state_node():
		active_(false),
		eh_(NULL)
	{}

	bool active_;
	_INST *eh_;

};

template<typename _INST,class _ACE_SYNCH = ACE_MT_SYNCH >
class eh_state_mgr
{
public:
	typedef eh_state_node<_INST> Node;

	eh_state_mgr():max_size_(0),
		active_size_(0),
		data_(NULL)
	{
	}
	
	int open(size_t max_size)
	{
		//�ⲿ�����±겢��ȷ���Ǵ�0��ʼ���������һ����Ԫ
		Node * new_ptr = new Node[max_size+1];
		if (new_ptr == NULL){
			//bad alloc
			return -1;
		}
		data_ = new_ptr;
		max_size_ = max_size+1;

		return 0;
	}

	int close()
	{
		if (data_){
			delete[] data_;
			data_ = NULL;
		}

		max_size_ = 0;
		return 0;
	}

	int add(_INST* eh)
	{
		size_t id = eh->id();

		if (id >= max_size_){
			//invalid id
			return -1;
		}

		ACE_GUARD_RETURN (_ACE_SYNCH::MUTEX, ace_mon, this->mutex_, -1);
		if (data_[id].active_){
			//already add
			return 1;
		}

		data_[id].active_ = true;

		//У������
		if (data_[id].eh_ != NULL && data_[id].eh_ != eh){
			return -1;
		}else{
			data_[id].eh_ = eh;
		}
		
		active_size_++;
		return 0;
	}

	int remove(_INST* eh)
	{
		size_t id = eh->id();
		if (id >= max_size_){
			//invalid id
			return -1;
		}

		ACE_GUARD_RETURN (_ACE_SYNCH::MUTEX, ace_mon, this->mutex_, -1);
		if (!data_[id].active_){
			//already remove
			return 1;
		}

		if (data_[id].eh_ != eh){
			//fatal error
			return -1;
		}

		data_[id].active_ = false;
		active_size_--;
		return 0;
	}

	_INST* find_eh(size_t id)
	{
		if (id >= max_size_){
			//invalid id
			return NULL;
		}

		ACE_GUARD_RETURN (_ACE_SYNCH::MUTEX, ace_mon, this->mutex_, NULL);
		if (!data_[id].active_){
			//not active
			return NULL;
		}

		if (data_[id].eh_ == NULL){
			//fatal error
			return NULL;
		}
	
		return data_[id].eh_;
	}

	size_t active_size()
	{
		return active_size_;
	}

	size_t max_size()
	{
		return max_size_;
	}

private:

	typename _ACE_SYNCH::MUTEX mutex_;
		
	Node * data_;
	size_t max_size_;
	size_t active_size_;
};


#endif /* __EH_STAT_H__ */
