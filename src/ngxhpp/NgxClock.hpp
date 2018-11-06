#ifndef _NGX_CLOCK_HPP
#define _NGX_CLOCK_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

// Nginx的时间功能封装
class NgxClock final
{
public:
	NgxClock()  = default;
	~NgxClock() = default;
	
private:
	ngx_time_t m_time = now();			// 初始化为当前时间
	
public:
	static const ngx_time_t & now()			// 获取当前时间
	{
		ngx_time_update();			// 更新缓存时间
		
		return * ngx_timeofday();		// 获取缓存时间
	}
	
	ngx_time_t delta() const			// 计算流逝的时间
	{
		auto t = now();				// 获取当前时间
		
		t.sec -= m_time.sec;			// 计算差值
		t.msec -= m_time.msec;
		
		return t;
	}
	
	double elapsed() const				// 返回浮点数格式的时间
	{
		auto t = delta();			// 计算流逝的时间
							// 转换为 double
		return t.sec + t.msec * 1.0 / 1000;
	}
	
	void reset()					// 复位计时器
	{
		m_time = now();
	}
};

#endif
