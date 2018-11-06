#ifndef _NGX_CHAIN_ITERATOR_HPP
#define	_NGX_CHAIN_ITERATOR_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

class NgxChainIterator final : 
		public boost::iterator_facade<
					NgxChainIterator, NgxChainNode,
					boost::single_pass_traversal_tag
					>
{
public:
	typedef boost:iterator_facade<...>	super_type;
	typedef typename super_type::reference	reference;
	
public:
	NgxChainIterator(ngx_chain_t * c) : m_p(c) {}		// 获取节点指针
	
	NgxChainIterator() = default;
	
	~NgxChainIterator() = default;
	
private:
	ngx_chain_t * m_p = nullptr;				// 节点指针
	mutable NgxChainNode m_proxy{m_p};			// 代理对象
	
public:
	friend class boost::iterator_core_access;		// 必须的友元声明
	
	reference dereference() const				// 解引用操作，返回代理对象
	{
		m_proxy = m_p;
		return m_proxy;
	}
	
	void increment()					// 前进迭代器
	{
		if(!m_p)
		{
			return ;
		}
		
		m_p = m_p->next;
	}
	
	bool equal(NgxChainIterator const & o) const		// 比较迭代器
	{
		return m_p == o.m_p;
	}
};

#endif
