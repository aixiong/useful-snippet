#pragma once
#include<iostream>
#include<allocators>
#include<algorithm>

namespace xhy
{
	struct __avl_tree_node_base
	{
		typedef __avl_tree_node_base* base_ptr;
		typedef size_t height_type;
		__avl_tree_node_base():left(nullptr),right(nullptr),height(1){}
		base_ptr left;
		base_ptr right;
		height_type height;
		static base_ptr minimum(base_ptr x)
		{
			while (x->left != nullptr)x = x->left;
			return x;
		}
		static base_ptr maximum(base_ptr x)
		{
			while (x->right != nullptr)x = x->right;
			return x;
		}
	};
	template<class Value>
	struct __avl_tree_node :public __avl_tree_node_base
	{
		__avl_tree_node(Value v):value_field(v){}
		typedef __avl_tree_node<Value>* link_type;
		Value value_field;
	};

	template<typename Key, typename Value, typename KeyOfValue, typename Compare,
		template<typename> class Alloc = std::allocator>
		class avl_tree
	{
	protected:
		typedef void* void_pointer;
		typedef __avl_tree_node_base* base_ptr;
		typedef __avl_tree_node<Value> avl_tree_node;
		typedef std::allocator<avl_tree_node> avl_tree_node_allocator;
		avl_tree_node_allocator alloc;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef Value* pointer;
		typedef const Value* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef avl_tree_node* link_type;
		typedef int size_type;
		typedef ptrdiff_t difference_type;
	protected:
		link_type get_node()
		{
			return alloc.allocate(1);
		}
		void put_node(link_type p)
		{
			alloc.deallocate(p, 1);
		}

		link_type create_node(const value_type& x)
		{
			link_type tmp = get_node();
			try
			{
				alloc.construct(tmp, x);
			}
			catch (...)
			{
				put_node(tmp);
			}
			return tmp;
		}
		void destroy_node(link_type p)
		{
			destroy_node(&p->value_field);
			put_node(p);
		}
	protected:
		link_type root;
		size_type node_count;
		Compare key_compare;
	public:
		void print(link_type x)
		{
			if (x == nullptr)return;
			cout << x->value_field << " ";
			print((link_type)x->left);
			print((link_type)x->right);
		}
	private:
		size_type getHeight(base_ptr x)
		{
			if (x == nullptr)return 0;
			else return x->height;
		}
		size_type getBalance(link_type x);
		link_type balance(link_type x);
		link_type rightRotate(link_type x);
		link_type leftRotate(link_type x);
		link_type __insert(link_type x, const value_type& v,link_type& pos);
		bool __remove(base_ptr x, const value_type& v);
	public:
		avl_tree():root(nullptr),node_count(0){}
		link_type insert(const value_type& v);
		bool remove(const value_type& v);
	};
	

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	inline typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::getBalance(link_type x)
	{
		return getHeight(x->left) - getHeight(x->r);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::balance(link_type x)
	{
		size_type diff = getBalance(x);
		if (diff<= 1 && diff>=-1)return x;
		if (diff >1)
		{
			link_type y = (link_type)x->left;
			if (getHeight(y->left) > getHeight(y->right))
			{
				return rightRotate(x);
			}
			else
			{
				x->left = leftRotate(y);
				return rightRotate(x);
			}
		}
		else if (diff < -1)
		{
			link_type y =(link_type) x->right;
			if (getHeight(y->right) > getHeight(y->left))
			{
				return leftRotate(x);
			}
			else
			{
				x->right = rightRotate(y);
				return leftRotate(x);
			}
		}
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::rightRotate(link_type x)
	{
		link_type y = (link_type)x->left;
		link_type z = (link_type)y->left;
		x->left = z;
		y->right = x;
		x->height = std::max(getHeight(z), getHeight(x->right)) + 1;
		y->height = std::max(getHeight(x), getHeight(y->left)) + 1;
		return y;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::leftRotate(link_type y)
	{
		link_type x = (link_type)y->right;
		link_type z = (link_type)x->left;
		y->right = z;
		x->left = y;
		y->height = std::max(getHeight(z), getHeight(y->left)) + 1;
		x->height = std::max(getHeight(y), getHeight(x->right)) + 1;
		return x;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, const value_type & v,link_type& pos)
	{
		if (x == nullptr)
		{
			pos= create_node(v);
			return pos;
		}
		link_type xl = link_type(x->left);
		link_type xr = link_type(x->right);
		if (v <x->value_field)
		{
			x->left = __insert(xl, v,pos);
		}
		else if (v > x->value_field)
		{
			x->right= __insert(xr, v,pos);
		}
		if (pos != nullptr)
		{
			x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
			x=balance(x);
		}
		return x;

	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	bool avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::__remove(base_ptr x, const value_type & v)
	{
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	typename avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
		avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert(const value_type & v)
	{
		link_type ans = nullptr;
		root=__insert(root,v,ans);
		print(root);
		std::cout << std::endl;
		return ans;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, template<typename> class Alloc>
	bool avl_tree<Key, Value, KeyOfValue, Compare, Alloc>::remove(const value_type & v)
	{
		return false;
	}



	template<typename Value,template<typename>class Alloc=std::allocator>
	using simple_avlTree = avl_tree<Value, Value, void, std::less<Value>,Alloc>;

}