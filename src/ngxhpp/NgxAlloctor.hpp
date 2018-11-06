#ifndef _NGX_ALLOCTOR_HPP
#define _NGX_ALLOCTOR_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

// 内存配置器
template<typename T>
class NgxAlloctor : public NgxWrapper<ngx_pool_t>
{
public:
	typedef NgxWrapper<ngx_pool_t>	super_type;		// 简化类型定义
	typedef NgxAlloctor		this_type;
	
public:
	typedef std::size_t	size_type;			// 内部类型定义
	typedef T*		pointer;
	typedef T		value_type;
	
public:
	NgxAlloctor(ngx_pool_t * p) : super_type(p) {}
	
	~NgxAlloctor() = default;
	
	pointer allocate(size_type n)				// 分配n个元素所需的内存
	{
		return reinterpret_cast<pointer>(		// 使用Nginx内存池
					ngx_pnalloc(get(), n * sizeof(T)));
	}
	
	void deallocate(pointer ptr, size_type n)		// 释放内存，由Nginx负责内存回收
	{
		boost::ignore_unused(n);			// 忽略出口参数n
		ngx_pfree(get(), ptr);				// 归还内存池
	}
};

#ifdef NGX_STD_CONTAINER
#include <vector>

template<typename T>
using NgxStdVector = std::vector<T, NgxAlloctor<T> >;

#endif
