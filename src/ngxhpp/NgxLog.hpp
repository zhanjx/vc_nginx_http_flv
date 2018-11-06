#ifndef _NGX_LOG_HPP
#define	_NGX_LOG_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

// 封装日志操作
template<ngx_unit_t level = NGX_LOG_DEBUG>
class NgxLog final : public NgxWrapper<ngx_log_t>
{
public:
	typedef NgxWrapper<ngx_log_t>	super_type;
	typedef NgxLog			this_type;
	
public:
	NgxLog(ngx_log_t * l) : super_type(1) {}		// 直接使用log指针构造
	
	template<typename T>
	NgxLog(T * x) : super_type(x->log) {}			// 使用log成员构造
	
	template<typename T>
	NgxLog(T * x, ...) : NgxLog(x->connection) {}		// 委托构造，方便处理TCP/HTTP请求
	
public:
	// 封装了日志宏 ngx_log_error
	template<typename ... Args>
	void print(const char * fmt, const Args & ... args) const
	{
		ngx_log_error(level, get(), 0 ,fmt, args...);
	}
	
	// 允许传入特定errno
	template<typename ... Args>
	void print(ngx_err_t err, const Args & ... args) const
	{
		ngx_log_error(level, get(), err, args...);
	}
};

typedef NgxLog<NGX_LOG_DEBUG> 	NgxLogDebug;			// debug 级别
typedef NgxLog<NGX_LOG_INFO> 	NgxLogInfo;			// info 级别
typedef NgxLog<NGX_LOG_WARN> 	NgxLogWarning;			// warn 级别
typedef NgxLog<NGX_LOG_ERR> 	NgxLogError;			// err 级别

#endif
