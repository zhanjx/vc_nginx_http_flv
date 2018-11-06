#ifndef _NGX_DATETIME_HPP
#define _NGX_DATETIME_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

// Nginx的日期功能封装
class NgxDatetime final
{
public:
	NgxDatetime()  = default;
	~NgxDatetime() = default;
	
public:
	static ngx_str_t since()			// 封装 ngx_time()
	{
		return ngx_time();
	}
	
	static ngx_str_t today()			// 获取当前日期
	{
		ngx_tm_t tm;
		
		ngx_localtime(since(), &tm);		// 转换为本地时间
		
		static u_char buf[20] = {};		// 静态字符数组用作输出
							// 格式化为 yyyy-mm-dd，获取字符串末尾
		auto p = ngx_snprintf(buf, 20, "%d-%02d-%02d",
					tm.ngx_tm_year, tm.ngx_tm_mon, tm.ngx_tm_mday);
								
		return ngx_str_t{staic_cast<std::size_t>(p - buf), buf};
	}
	
	static ngx_str_t http(std::time_t t = since())	// 时间戳转日期字符串
	{
		static u_char buf[50] = {};
		
		auto p = ngx_http_time(buf, t);
		
		return ngx_str_t{static_cast<std::size_t>(p - buf), buf};
	}
	
	static std::time_t http(ngx_str_t & str)	// 日期字符串转时间戳
	{
		return ngx_parse_http_time(str.data, str.len);
	}
};

#endif
