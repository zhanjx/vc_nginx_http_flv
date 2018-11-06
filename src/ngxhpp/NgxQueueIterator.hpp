#ifndef _NGX_QUEUE_ITERATOR_HPP
#define	_NGX_QUEUE_ITERATOR_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

template<typename T>							// T是 NgxQueueNode
class NgxQueueIterator final : 
		public boost::iterator_facade<
				NgxQueueIterator<T>, T,
				boost::bidirectional_traversal_tag>	// 双向迭代器
{
public:
	typedef boost::iterator_facade<...> 	super_type;
	typedef NgxQueueIterator		this_type;
	
	typedef typename super_type::reference 	reference;
	
public:
	NgxQueueIterator(ngx_queue_t * p) : m_cur(p) {}		// 初始化指针
	
	NgxQueueIterator()  = default;
	
	~NgxQueueIterator() = default;
	
private:
	ngx_queue_t *	m_cur = nullptr;			// 迭代用的指针
	mutable T		m_proxy{m_cur}; 		// 代理节点对象
	
private:
	friend class boost::iterator_core_access;		// 必需的友元声明
	
	reference dereference() const				// 解引用操作，返回代理对象
	{
		m_proxy = m_cur;				// 拷贝赋值数据节点
		return m_proxy;					// 返回节点对象的引用
	}
	
	void increment()					// 前进迭代器
	{
		m_cur = ngx_queue_next(m_cur);
	}
	
	void decrement()					// 后退迭代器
	{
		m_cur = ngx_queue_prev(m_cur);
	}
	
	bool equal(this_type const & o) const			// 比较两个迭代器
	{
		return m_cur == o.m_cur;
	}
};

#endif
