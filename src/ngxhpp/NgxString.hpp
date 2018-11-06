#ifndef _NGX_STRING_HPP
#define _NGX_STRING_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

// 字符串类
class NgxString final : public NgxWrapper<ngx_str_t>
{
public:
	typedef NgxWrapper<ngx_str_t>	super_type;
	typedef NgxString		this_type;
	
	typedef boost::string_ref	string_ref_type;	// char* 字符串引用类型
	
public:
	NgxString(ngx_str_t & str) : super_type(str) {}
	
	~NgxString() = default;
	
public:
	const char * data() const				// 获取字符串
	{
		return reinterpret_cast<const char*>(get()->data);
	}
	
	std::size_t size() const				// 获取长度
	{
		return get()->len;
	}
	
	bool empty() const					// 是否为空串
	{
		return !get()->data || !get()->len;
	}
	
	string_ref_type str() const				// 转换为 char* 字符串形式
	{
		return string_ref_type(data(), size());
	}
	
public:
	operator ngx_int_t () const				// 字符串转整数类型
	{
		return ngx_atoi(get()->data, get()->len);
	}
	
	// 重载比较操作符，大小写敏感比较两个 ngx_str_t 对象
	friend bool operator==(const this_type & l, const this_type & r)
	{
		return l.size() == r.size() &&
				ngx_strncmp(l.data(), r.data(), l.size()) == 0;
	}
	
	template<typename ... Args>
	void printf(const Args & ... args) const		// 安全格式化
	{
		auto p = ngx_snprintf(get()->data, get()->len, args ...);
		
		// 计算实际长度
		get()->len = static_cast<std::size_t>(p - get()->data);
	}
	
public:
	template<typename T>
	friend T & operator<<(T & o, const this_type & s)	// 重载流输出操作符
	{
		o.write(s.data(), s.size());			// 把字符串写入流
		return o;
	}
};

#endif
