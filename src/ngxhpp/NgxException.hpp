#ifndef _NGX_EXCEPTION_HPP
#define _NGX_EXCEPTION_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

// 异常类
class NgxException final : public virtual std::exception,
			   public virtual boost::exception
{
public:
	typedef boost::string_ref string_ref_type;
	
private:
	ngx_int_t		m_code = NGX_ERROR;		// 错误代码
	std::string		m_msg;				// 错误信息
	
public:
	NgxException(ngx_int_t x, string_ref_type msg):
					m_code(x), m_msg(msg) {}
				 
	NgxException(ngx_int_t x = NGX_ERROR):
			NgxException(x, "") {}			// 委托构造
			
	NgxException(string_ref_type msg):
			NgxException(NGX_ERROR, msg) {}		// 委托构造
			
	virtual ~NgxException() noexcept {}			// 虚析构函数
	
public:
	ngx_int_t code() const					// 获取错误码
	{
		return m_code; 
	}
	
	virtual const char * what() const noexcept override
	{
		return m_msg.c_str();
	}
	
public:
	static void raise(ngx_int_t rc = NGX_ERROR, string_ref_type msg = "")
	{
		throw NgxException(rc, msg);			// 抛出异常
	}
	
	// 检查条件是否满足
	static void require(bool cond, ngx_int_t e = NGX_ERROR)
	{
		if(!cond)					// 如果不符合预期则抛出异常
		{
			raise(e);
		}
	}
	
	// 检查错误码，默认要求是NGX_OK
	static void require(ngx_int_t rc, ngx_int_t x = NGX_OK)
	{
		require(rc == x, rc);				// 如果不是OK则抛出异常
	}
	
	// 检查空指针，要求指针非空
	static void require(T * p, ngx_int_t e = NGX_ERROR)
	{
		require(p != nullptr, e);			// 如果是空指针则抛出异常
	}
	
	// require 的反函数
	static void fail(bool cond, ngx_int_t e = NGX_ERROR)
	{
		if(cond)
		{
			raise(e);				// 如果符合预期则抛出异常
		}
	}
};

#endif
