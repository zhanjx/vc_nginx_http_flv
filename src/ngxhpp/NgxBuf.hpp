#ifndef _NGX_BUF_HPP
#define	_NGX_BUF_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"
#include "NgxPool.hpp"

class NgxBuf final : public NgxWrapper<ngx_buf_t>
{
public:
	typename NgxWrapper<ngx_buf_t>	super_type;
	typename NgxBuf			this_type;

public:
	NgxBuf(const NgxPool & p, std::size_t n) : 		// 从内存池创建缓冲区
				super_type(p.buffer(n)) {}
	
	NgxBuf(ngx_buf_t * buf) : super_type(buf) {}		// 代理一个缓冲区
	
	~NgxBuf() = default;
	
public:
	bool memory() const					// 缓冲区在内存
	{
		return ngx_buf_in_memory(get());
	}
	
	bool memoryonly() const					// 缓冲区不在文件
	{
		return ngx_buf_in_memory_only(get());
	}
	
	bool special() const					// 是特殊控制缓冲区
	{
		return ngx_buf_special(get());
	}
	
	std::size_t size() const				// 缓冲区的大小
	{
		return ngx_buf_size(get());
	}
	
public:
	void range(u_char * a, u_char * b) const		// 使用两个指针设置缓冲区
	{
		get()->pos  = a;
		get()->last = b;
		get()->memory = true;				// 设置内存标志
	}
	
	void boundary(u_char * a, u_char * b) const		// 设置缓冲区边缘
	{
		get()->start = a;
		get()->end   = b;
	}
	
	void range(ngx_str_t * s) const				// 使用 ngx_str_t 设置缓冲区
	{
		range(s->data, s->data + s->len);
		boundary(s->data, s->data + s->len);
	}
	
	ngx_str_t range() const					// 返回缓冲区内的有效数据
	{
		return ngx_str_t{ get()->last - get()->pos, get()->pos };
	}
	
	ngx_str_t boundary() const				// 返回缓冲区的边界
	{
		return ngx_str_t{ get()->end - get()->start, get()->start() };
	}
	
	bool last() const					// 是否是最后一块缓冲区
	{
		return get()->last_buf || get()->last_in_chain;
	}
	
	void finish(bool flag = true) const			// 置为最后一块缓冲区
	{
		get()->last_buf = flag;
		get()->last_in_chain = flag;
	}
};

#endif
