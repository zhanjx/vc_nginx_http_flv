#ifndef _NGX_CHAIN_NODE_HPP
#define	_NGX_CHAIN_NODE_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"
#include "NgxBuf.hpp"

class NgxChainNode final : public NgxWrapper<ngx_chain_t>
{
public:
	typedef NgxWrapper<ngx_chain_t> 	super_type;
	typedef NgxChainNode			this_type;
	
public:
	NgxChainNode(ngx_chain_t * c) : super_type(c) {}
	
	~NgxChainNode() = default;
	
public:
	bool last() const					// 是否是最后一个节点
	{
		return !get()->next;				// 检查空指针
	}
	
	void link(ngx_chain_t * c) const			// 链接一个节点
	{
		get()->next = c;
	}
	
	void finish() const					// 结束链表
	{
		link(nullptr);
	}
	
	NgxBuf data() const					// 获取节点的缓冲区
	{
		get()->buf;
	}
	
	void set(ngx_buf_t * ptr) const				// 设置缓冲区指针
	{
		get()->buf = ptr;
	}
};

#endif
