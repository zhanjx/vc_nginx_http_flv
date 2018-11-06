#ifndef _NGX_LIST_HPP
#define	_NGX_LIST_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"
#include "NgxException.hpp"
#include "NgxListIterator.hpp"

template<typename T>						// T是数组的元素类型
class NgxList final : public NgxWrapper<ngx_list_t>
{
public:
	typedef NgxWrapper<ngx_list_t>		super_type;
	typedef NgxList				this_type;
	
public:
	NgxList(ngx_list_t * l) : super_type(l) {}		// 指针参数
	
	NgxList(ngx_list_t & l) : super_type(&l) {}		// 引用参数
	
	~NgxList() = default;
	
public:
	T & prepare() const
	{
		auto tmp = ngx_list_push(get());		// 添加元素
		
		NgxException::require(tmp);			// 检查空指针
		
		return *reinterpret_cast<T*>(tmp);		// 返回元素的左引用
	}
	
	void push(const T & x) const
	{
		prepare() = x;
	}
	
public:
	typedef NgxListIterator<T> 	iterator;		// 迭代器类型定义
	typedef const iterator		const_iterator;
	
	iterator begin() const					// 迭代器起点
	{
		return iterator(get());				// 使用链表指针初始化
	}
	
	iterator end() const					// 迭代器终点
	{
		return iterator();				// 指针为空的迭代器
	}
	
	template<typename Predicate>
	iterator find(Predicate p) const			// 使用谓词函数对象查找
	{
		auto iter = begin();
		for( ; iter; ++iter)
		{
			if(p(*iter))
			{
				return iter;
			}
		}
		
		return end();
	}
};

#endif
