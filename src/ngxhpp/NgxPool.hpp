#ifndef _NGX_POOL_HPP
#define _NGX_POOL_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"			// 基本包装类
#include "NgxException.hpp"			// 异常类

// 封装了Nginx的内存池
class NgxPool final : public NgxWrapper<ngx_pool_t>
{
public:
	typedef NgxWrapper<ngx_pool_t> 	super_type;		// 简化类型定义
	typedef NgxPool			this_type;
	
public:
	NgxPool(ngx_pool_t * p) : super_type(p) {}
	
	template<typename T>					// 模板参数T可以是 ngx_http_request_t、ngx_conf_t
	NgxPool(T * x) : Nginx(x->pool) {}			// 等含有pool成员的结构

	~NgxPool() = default;
	
public:
	template<typename T, bool no_exception = false>
	T * palloc() const
	{
		auto p = ngx_pcalloc(get(), sizeof(T));		// 分配内存
		
		if(!p)						// 检查空指针
		{
			if(no_exception)			// 是否允许抛出异常
			{
				return nullptr;
			}
			
			NgxException::raise();			// 抛出异常
		}
		
		assert(p);
		return new (p) T();				// 构造T对象
	}
	
	template<typename T>
	T * alloc() const					// 抛异常版本
	{
		return palloc<T, false>();
	}
	
	template<typename T>
	T * alloc_noexcept() const				// 不抛异常版本
	{
		return palloc<T, true>();
	}
	
	template<typename T>
	T * nalloc(std::size_t n) const				// 分配n个字节内存
	{
		auto p = ngx_pnalloc(get(), n);
		
		NgxException::require(p);			// 检查空指针
		
		return reinterpret_cast<T*>(p);			// 转型为T*
	}
	
	template<typename T, typename ... Args>
	T * construct(const Args & ... args) const		// 任意数量参数构造对象
	{
		auto p = ngx_pcalloc(get(), sizeof(T));
		
		NgxException::require(p);
		
		return new (p) T(args ...);			// 转发参数构造
	}
	
public:
	template<typename F, typename T>
	ngx_pool_cleanup_t * cleanup(F func, T * data, std::size_t size = 0) const
	{
		auto p = ngx_pool_cleanup_add(get(), size);	// 调用Nginx的cleanup_add
		NgxException::require(p);
		
		p->handler = func;				// 设置清理函数
		
		if(data)					// 允许直接传入待释放的资源
		{
			p->data = data;
		}
		
		return p;					// 返回清理信息对象
	}
	
	template<typename T>
	static void destroy(void * p)				// 适配析构函数符合Nginx要求
	{
		(reinterpret_cast<T*>(p))->~T();		// 转换 void*，调用对应析构函数
	}
	
	template<typename T>
	void cleanup(T * data) const
	{
		cleanup(&this_type::destroy<T>, data);
	}
	
public:
	ngx_str_t dup(ngx_str_t & str) const			// 复制字符串
	{
		ngx_str_t tmp;					// 准备返回的字符串
		
		tmp.len  = str.len;				// 设置字符串长度
		tmp.data = ngx_pstrdup(get(), &str); 		// 内存池复制字符串
		
		NgxException::require(tmp.data);		// 检查空指针
		return tmp;					// 返回复制的字符串
	}
	
	ngx_str_t dup(boost::string_ref str) const		// 复制普通字符串
	{
		ngx_str_t tmp{ str.size(), (u_char*)str.data() };
		
		return dup(tmp);
	}
	
public:
	template<typename T>
	ngx_array_t * array(ngx_uint_t n = 10) const		// 默认容纳10个元素
	{
		auto p = ngx_array_create(get(), n, sizeof(T));	// 使用内存池创建数组
		
		NgxException::require(p);
		
		return p;
	}
public:
	ngx_buf_t* buffer(std::size_t n = 0) const		// 创建缓冲区
	{
		auto tmp = n > 0 ?
		ngx_create_temp_buf(get(), n) : 
		alloc<ngx_buf_t>()				// ngx_calloc_buf(get())
		;

		NgxException::require(tmp); 			// 检查空指针

		return tmp;
	}
	
	ngx_chain_t * chain() const				// 创建一个空节点
	{
		auto p = ngx_alloc_chain_link(get());
		
		NgxException::require(p);
		return p;
	}
	
	ngx_chain_t * chain(const ngx_bufs_t & bufs) const	// 创建多个节点
	{
		auto p = ngx_create_chain_of_bufs(get(), 
						const_cast<ngx_bufs_t*>(&bufs);
		
		NgxException::require(p);
		return p;
	}
};

#endif
