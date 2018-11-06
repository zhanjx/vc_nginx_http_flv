#ifndef _NGX_LIST_ITERATOR_HPP
#define	_NGX_LIST_ITERATOR_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxException.hpp"

template<typename T>
class NgxListIterator final : 
		public boost::iterator_facade<					// Boost 工具类
					NgxListIterator<T>, T, 			// 迭代器的值类型
					boost::single_pass_traversal_tag>	// 单向遍历
{
public:
	typedef boost::iterator_facade<...>	super_type;
	typedef typename super_type::reference	reference;
	
public:
	NgxListIterator(ngx_list_t * l) : 
		m_part(&l->part), 				// 初始化头结点
		m_data(static_cast<T*>(m_part->elts)) {}	// 初始化数组指针
		
	NgxListIterator()  = default;
	
	~NgxListIterator() = default;
	
private:
	ngx_list_part_t * 	m_part  = nullptr;		// 节点指针
	T * 				m_data  = nullptr;	// 节点内数组指针
	ngx_uint_t			m_count = 0;		// 节点内数组索引
	
private:
	friend class boost::iterator_core_access;		// 必需的友元声明
	
	reference dereference() const				// 解引用的操作
	{
		NgxException::require(m_data);			// 检查空指针
		return m_data[m_count];				// 访问节点内数组元素
	}
	
	void increment()					// 前进迭代器
	{
		if(!m_part || !m_data)				// 检查空指针
		{
			return;
		}
		
		++mcount;					// 索引前进
		
		if(m_count >= m_part->nelts)			// 检查数组越界
		{
			m_count = 0;				// 索引初始化
			m_part = m_part->next;			// 跳至下一个节点
			
			m_data = m_part ?			// 是否是尾节点
					static_cast<T*>(m_part->elts) :		// 访问下一个节点数组
					nullptr;		// 否则设为空
		}
	}
	
	bool equal(NgxListIterator const & o) const		// 比较两个迭代器是否相等
	{
		return m_part == o.m_part &&
			   m_data == o.m_data &&
			  m_count == o.m_count;
	}

public:
	BOOST_EXPLICIT_OPERATOR_BOOL()				// Boost 显式bool转型
	
	bool operator!() const					// 检查指针是否失效
	{
		return !m_part || !m_data || !m_part->nelts;	// 指针为空等情况则失效
	}
};

#endif
