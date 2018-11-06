#ifndef _NGX_WRAPPER_HPP
#define _NGX_WRAPPER_HPP

#include <boost/type_traits.hpp>	// 使用 type_traits 元函数计算类型

// 包装代理一个Nginx数据对象，是一个抽象类，只能被继承使用
template<typename T>
class NgxWrapper
{
public:
	typedef typename boost::remove_pointer<T>::type wrapped_type;

	typedef wrapped_type *		pointer_type;
	typedef wrapped_type &		reference_type;

private:
	pointer_type m_ptr = nullptr;       	// 使用指针保存对象，默认是空指针


protected:
	// 参数是指针类型
	NgxWrapper(pointer_type p):m_ptr(p) {}
	// 参数是引用类型
	NgxWrapper(reference_type x):m_ptr(&x) {}
	// 析构函数不做任何事(NgxWrapper 不负责对象的生命周期管理，由内存池负责)
	~NgxWrapper() = default;

public:
	pointer_type get() const		// 访问指针成员
	{
		return m_ptr;
	}

	operator bool () const			// 转型为bool
	{
		return get();
	}

	operator pointer_type () const		// 转型为指针类型
	{
		return get();
	}

	pointer_type operator->() const		// 指针操作符重载
	{
		return get();
	}
};

#endif
