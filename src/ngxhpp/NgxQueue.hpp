#ifndef _NGX_QUEUE_HPP
#define	_NGX_QUEUE_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

// 封装了 ngx_queue_t 的队列操作，代理了队列的头结点
template<typename T,
		ngx_queue_t T::* q = &T::queue,
		std::size_t offset = 
				(std::size_t)&(((T*)0)->*q)
				>
class NgxQueue final : public NgxWrapper<ngx_queue_t>
{
public:
	typedef NgxWrapper<ngx_queue_t>		super_type;
	typedef NgxQueueNode<T, q, offset>	node_type;	// 节点类型定义
	typedef NgxQueue			this_type;
	
public:
	NgxQueue(ngx_queue_t & v) : super_type(&v) {}		// 引用构造
	
	NgxQueue(ngx_queue_t * ptr) : super_type(ptr) {}	// 指针构造
	
	NgxQueue(T & x) : super_type(x.*q) {}			// 获取成员变量地址
	
	NgxQueue(T * x) : super_type(x->*q) {}			// 获取成员变量地址
	
	~NgxQueue() = default;
	
public:
	void init() const					// 初始化队列
	{
		ngx_queue_init(get());
	}
	
	bool empty() const					// 队列是否为空
	{
		return ngx_queue_empty(get());
	}
	
	node_type front() const					// 获得队首节点
	{
		return ngx_queue_head(get());
	}
	
	node_type back() const					// 获得队尾节点
	{
		return ngx_queue_last(get());
	}

public:
	// 迭代器和逆向迭代器类型定义
	typedef NgxQueueIterator<node_type>		iterator;
	typedef boost::reverse_iterator<iterator>	reverse_iterator;
	
	typedef const iterator				const_iterator;
	typedef const reverse_iterator			const_reverse_iterator;
	
	iterator begin() const					// 正向迭代器起点
	{
		return iterator(ngx_queue_head(get()));
	}
	
	iterator end() const					// 正向迭代器终点
	{
		return iterator(ngx_queue_sentinel(get()));	// 终点即头结点
	}
	
	reverse_iterator rbegin() const				// 逆向迭代器起点
	{
		return reverse_iterator(end());			// 适配正向迭代器
	}
	
	reverse_iterator rend() const				// 逆向迭代器终点
	{
		return reverse_iterator(begin());		// 适配正向迭代器
	}
	
public:
	void insert(T & v) const				// 插入队首
	{
		ngx_queue_insert_head(get(), &(v.*q));
	}
	
	void append(T & v) const				// 插入队尾
	{
		ngx_queue_insert_tail(get(), &(v.*q));
	}
	
	void insert(const iterator & iter, T & v) const		// 插入迭代器位置之后
	{
		iter->append(v);
	}
};

#endif
