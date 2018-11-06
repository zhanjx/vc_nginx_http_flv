#ifndef _NGX_VALUE_HPP
#define _NGX_VALUE_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"

// 无效值的封装
class NgxUnsetValue final				// final 禁止被继承
{
public:
	template<typename T>
	operator T () const				// 转型到类型T
	{
		return static_cast<T>(-1);		// 使用 static_cast 转型 -1
	}

	template<typename T>
	operator T * () const				// 转型到类型T*，即指针类型
	{
		return reinterpret_cast<T*>(-1);
	}

public:
	static const NgxUnsetValue & get()		// 获取全局唯一对象
	{
		static NgxUnsetValue const v = {};	// 静态变量，空类
		return v;
	}
};

auto&& ngx_nil = NgxUnsetValue::get();			// 右值引用，创建别名


// 封装对整数类型的基本操作
class NgxValue final
{
public:
	NgxValue()  = default;				// 默认构造/析构函数
	~NgxValue() = default;

public:
	template<typename T>
	static bool invalid(const T & v)		// 无效值判断
	{
		return v == static_cast<T>(NgxUnsetValue::get());
	}

	template<typename T, typename U>
	static void init(T & x, const U & v)
	{
		if(invalid(x))
		{
			x = v;				// 如果为无效值则初始化
		}
	}

	template<typename T, typename U, typename V>
	static void merge(T & c, const U & p, const V & d)	// 条件赋值
	{
		if(invalid(c))
		{
			c = invalid(p) ? d : p;			// 检查p,无效则初始化
		}
	}

	template<typename T>
	bool operator==(const T & x, const NgxUnsetValue &)	// 与 ngx_nil 比较
	{
		return NgxValue::invalid(x);
	}

public:
	template<typename T, typename ... Args>
	static void unset(T & v, Args & ... args)		// 批量未初始化
	{
		v = NgxUnsetValue::get();			// 置为未初始化状态
		unset(args...);					// 递归处理剩余模板参数

		static void unset() {}				// 递归终结函数
	}
};

#endif
