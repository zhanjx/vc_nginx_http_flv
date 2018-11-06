#ifndef _NGX_CHAIN_HPP
#define	_NGX_CHAIN_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"
#include "NgxChainIterator.hpp"

// 封装了 ngx_chain_t 链表
class NgxChain final : public NgxWrapper<ngx_chain_t>
{
public:
	typedef NgxWrapper<ngx_chain_t> 	super_type;
	typedef NgxChain			this_type;
	
public:
	NgxChain(ngx_chain_t * c) : super_type(c) {}		// 代理链表节点
	
	~NgxChain() = default;
	
public:
	typedef NgxChainIterator	iterator;		// 迭代器类型定义
	typedef const iterator		const_iterator;
	
	iterator begin() const					// 产生迭代器起点
	{
		return iterator(get());
	}
	
	iterator end() const					// 产生迭代器终点
	{
		return iterator();
	}
	
	std::size_t size() const				// 遍历链表，计算总长度
	{
		std::size_t len = 0;
		
		for(auto & c : *this)
		{
			len += c.data().size();			// 获取节点的 buf 长度
		}
		
		return len;
	}
	
public:
	ngx_chain_t * tail() const				// 查找末尾节点
	{
		auto p = get();
		
		for(; p->next; p = p->next);
		
		return p;
	}
	
	void append(ngx_chain_t * ch) const			// 向末尾追加节点
	{
		tail()->next = ch;
	}
};

#endif
