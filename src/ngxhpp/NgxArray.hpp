#ifndef _NGX_ARRAY_HPP
#define	_NGX_ARRAY_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

template<typename T>						// T是数组的元素类型
class NgxArray final : public NgxWrapper<ngx_array_t>
{
public:
	typedef NgxWrapper<ngx_array_t>		super_type;
	typedef NgxArray			this_type;
	
	typedef NgxArray<ngx_str_t>		NgxStrArray;	// 字符串数组类型
	
public:
	NgxArray(const NgxPool & p, ngx_uint_t n = 10) : 	// 调用内存池创建数组
				super_type(p.array<T>(n)) {}
								
	NgxArray(ngx_array_t * arr) : super_type(arr) {}	// 指针参数
	
	NgxArray(ngx_array_t & arr) : super_type(arr) {}	// 引用参数
	
	~NgxArray() = default;

private:
	T * elts() const					// 获取元素数组指针，并转换为元素类型
	{
		return reinterpret_cast<T*>(get()->elts);
	}
	
public:
	ngx_uint_t size() const					// 获得数组大小
	{
		return get() ? get()->nelts : 0;		// 防止空指针
	}
	
	T & operator[](ngx_uint_t i) const
	{
		NgxException::require(i < size() && get());	// 越界、空指针检查
		
		return elts()[i];				// 调用elts()访问元素
	}
	
	T * prepare() const
	{
		auto tmp = ngx_array_push(get());		// 添加元素
		
		NgxException::require(tmp);			// 检查空指针
		
		return *reinterpret_cast<T*>(tmp);		// 返回元素的左引用
	}
	
	void push(const T & x) const
	{
		prepare() = x;					// 左引用可直接赋值
	}
	
	template<typename V>
	void visit(V v) const					// 访问数组里的所有元素，类似 for_each
	{
		auto p = elts();				// 获得数组首地址
		
		for(ngx_uint_t i = 0; i < size(); ++i)		// 遍历数组
		{
			v(p[i]);				// 执行函数对象
		}
	}
};

#endif
