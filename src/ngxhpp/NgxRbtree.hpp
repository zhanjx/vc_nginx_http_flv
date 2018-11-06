#ifndef _NGX_RBTREE_HPP
#define	_NGX_RBTREE_HPP

#include "Nginx.hpp"
#include "NgxCppInc.hpp"
#include "NgxWrapper.hpp"

// 使用成员变量指针封装了 ngx_rbtree_t
template<typename T,						// 节点数据类型
		ngx_rbtree_node_t T::* np,			// 红黑树节点成员函数指针
		ngx_rbtree_insert_pt func			// 插入方法函数指针
		>
class NgxRbtree final : public NgxWrapper<ngx_rbtree_t>
{
public:
	typedef NgxWrapper<ngx_rbtree_t>	super_type;
	typedef NgxRbtree			this_type;
	
	typedef ngx_rbtree_key_t		key_type;
	typedef ngx_rbtree_t			tree_type;
	typedef ngx_rbtree_node_t		node_type;
	
public:
	NgxRbtree(tree_type * t) : super_type(t) {}
	
	~NgxRbtree() = default;
	
public:
	static void init(tree_type &tree, node_type & s)	// 初始化一棵红黑树
	{
		ngx_rbtree_init(&tree, &s, func);
	}
	
	bool empty() const					// 判断红黑树是否为空
	{
		return get()->root == get()->sentinel;
	}
	
	void add(T & v) const					// 插入节点
	{
		ngx_rbtree_insert(get(), &(v.*np));
	}
	
	void del(T & v) const					// 删除节点
	{
		ngx_rbtree_delete(get(), &(v.*np));
	}
	
	key_type min_key() const				// 查找最小节点，返回键值，通常是整数
	{
		auto p = ngx_rbtree_min(get()->root, get()->sentinel);
		return p->key;
	}
	
	T & min() const							// 查找最小节点，返回实际的节点类型
	{
		auto p = ngx_rbtree_min(get()->root, get()->sentinel);
		constexpr auto offset = (std::size_t)&(((T*)0)->*np);	// 计算偏移量
		return *(T*)((u_char*)(p) - offset);			// 类型转换
	}

private:
	template<typename F>
	void do_traverse(node_type * p， F f) const			// 中序遍历的递归实现
	{
		if(p == get()->sentinel)
		{
			return ;
		}
		
		do_traverse(p->left, f);
		f(p);
		do_traverse(p->right, f);
	}
	
public:
	template<typename F>
	void traverse(F f) const					// 中序遍历，传入一个函数对象
	{
		do_traverse(get()->root, f);				// 传入根节点，开始遍历
	}
};

// 标准的红黑树，插入方法使用 ngx_rbtree_insert_value
template<typename T, ngx_rbtree_node_t T::* np>
using NgxValueTree = NgxRbtree<T, np, ngx_rbtree_insert_value>;

// 定时器红黑树，插入方法使用 ngx_rbtree_insert_timer_value
template<typename T, ngx_rbtree_node_t T::* np>
using NgxTimerTree = NgxRbtree<T, np, ngx_rbtree_insert_timer_value>;

// ngx_event_t 结构体专用红黑树，指定了节点成员函数指针 &ngx_event_t::timer
using NgxEventTimerTree = NgxTimerTree<ngx_event_t, &ngx_event_t::timer>;

#endif
