#ifndef _NGX_QUEUE_NODE_HPP
#define	_NGX_QUEUE_NODE_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

template<typename T,
		ngx_queue_t T::* q = &T::queue,			// 成员变量指针类型
		std::size_t offset =				// 成员偏移量
				(std::size_t)&(((T*)0)->*q)	// 模仿 offsetof 宏
				>
class NgxQueueNode : public NgxWrapper<ngx_queue_t>
{
public:
	typedef NgxWrapper<ngx_queue_t>		super_type;
	typedef NgxQueueNode			this_type;
	
public:
	NgxQueueNode(ngx_queue_t * ptr) : super_type(ptr) {}	// 直接指针构造
	
	NgxQueueNode(T & x) : super_type(x.*q) {}		// 获取成员变量地址
	
	NgxQueueNode(T * x) : super_type(x->*q) {}		// 获取成员变量地址
	
	~NgxQueueNode() = default;
	
public:
	T & data() const					// 获取节点数据
	{
		return *(T*)((u_char*)(get()) - offset);	// 返回引用
	}
	
	NgxQueueNode next() const				// 下一个节点
	{
		return ngx_queue_next(get());
	}
	
	NgxQueueNode prev() const				// 前一个节点
	{
		return ngx_queue_prev(get());
	}
	
public:
	void append(T & v) const				// 插入后继节点
	{
		ngx_queue_insert_after(get(), &(v.*q));
	}
	
	void remove() const					// 删除当前节点
	{
		ngx_queue_remove(get());
	}
};

#endif
