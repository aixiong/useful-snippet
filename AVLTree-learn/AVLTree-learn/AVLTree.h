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
		virtual ~__avl_tree_node_base() = 0{}
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

	template<typename Value,  typename Compare,template<typename> class Alloc = std::allocator>
		class avl_tree
	{
	protected:
		typedef __avl_tree_node_base* base_ptr;
		typedef __avl_tree_node<Value> avl_tree_node;
		typedef std::allocator<avl_tree_node> avl_tree_node_allocator;
	public:
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
			alloc.destroy(&p->value_field);
			put_node(p);
		}
	protected:
		link_type root;
		size_type node_count;
		Compare key_compare;
		avl_tree_node_allocator alloc;
	private:
		size_type getHeight(base_ptr x)
		{
			if (x == nullptr)return 0;
			else return x->height;
		}
		size_type getBalance(base_ptr x)
		{
			return getHeight(x->left) - getHeight(x->right);
		}
		base_ptr balance(base_ptr x_);
		base_ptr rightRotate(base_ptr x_);
		base_ptr leftRotate(base_ptr x_);
		base_ptr __insert(base_ptr x_, const value_type& v,link_type& pos);
		base_ptr __remove(base_ptr x_, const value_type& v,bool& found);
		base_ptr __search(base_ptr x_,const value_type& v);
		void clear(base_ptr x_)
		{
			link_type x = (link_type)x_;
			if (x->left)clear(x->left);
			if (x->right)clear(x->right);
			destroy_node(x);
		}
	public:
		avl_tree():root(nullptr),node_count(0){}
		~avl_tree()
		{
			if (root)clear(root);
		}
		link_type insert(const value_type& v);
		bool remove(const value_type& v);
		link_type search(const value_type& v);
	private:
		//test
		void print(base_ptr x_)
		{
			link_type x = (link_type)x_;
			std::cout << x->value_field << " ";
			if(x->left)print(x->left);
			if(x->right)print(x->right);
		}
	public:
		//test
		void print()
		{
			if (root != nullptr)
			{
				print(root);
			}
		}
	};
	
	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr
		avl_tree<Value,Compare, Alloc>::balance(base_ptr x)
	{
		
		size_type diff = getBalance(x);
		if (diff<= 1 && diff>=-1)return x;
		if (diff >1)
		{
			base_ptr y = x->left;
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
			base_ptr y = x->right;
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

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr 
		avl_tree<Value,Compare, Alloc>::rightRotate(base_ptr x)
	{
		base_ptr y = x->left;
		base_ptr z = y->right;
		x->left = z;
		y->right = x;
		x->height = std::max(getHeight(z), getHeight(x->right)) + 1;
		y->height = std::max(getHeight(x), getHeight(y->left)) + 1;
		return y;
	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr
		avl_tree<Value,Compare, Alloc>::leftRotate(base_ptr y)
	{
		base_ptr x = y->right;
		base_ptr z = x->left;
		y->right = z;
		x->left = y;
		y->height = std::max(getHeight(z), getHeight(y->left)) + 1;
		x->height = std::max(getHeight(y), getHeight(x->right)) + 1;
		return x;
	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr
		avl_tree<Value,Compare, Alloc>::__insert(base_ptr x_, const value_type & v,link_type& pos)
	{
		if (x_ == nullptr)
		{
			pos= create_node(v);
			return pos;
		}
		link_type x = (link_type)x_;
		if (v <x->value_field)
		{
			x->left = __insert(x->left, v,pos);
		}
		else if (v > x->value_field)
		{
			x->right= __insert(x->right, v,pos);
		}
		if (pos != nullptr)
		{
			x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
			x=(link_type)balance(x);
		}
		return x;

	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr
		avl_tree<Value,Compare, Alloc>::__remove(base_ptr x_, const value_type & v,bool& found)
	{
		if (x_ == nullptr)
		{
			return nullptr;
		}
		link_type x = (link_type)x_;
		if (x->value_field == v)
		{
			if (x->left == nullptr || x->right == nullptr)
			{
				found = true;
				return x->left ? x->left : x->right;
			}
			else
			{
				link_type z = (link_type)x->right->minimum(x->right);
				swap(x->value_field, z->value_field);
				x->right = __remove(x->right, v, found);
			}
		}
		else if (x->value_field < v)
		{
			x->right = __remove(x->right, v, found);
		}
		else
		{
			x->left = __remove(x->left, v, found);
		}
		if (found)
		{
			x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
			x=(link_type)balance(x);
		}
		return x;
	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::base_ptr
		avl_tree<Value,Compare, Alloc>::__search(base_ptr x_, const value_type & v)
	{
		if (x_ == nullptr)return x_;
		link_type x = (link_type)x_;
		if (x->value_field == v)return x;
	    base_ptr ans = nullptr;
		if (x->value_field < v)
		{
			ans= __search(x->right, v);
		}
		else
		{
			ans= __search(x->left, v);
		}
		return ans;
	}
	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::link_type
		avl_tree<Value,Compare, Alloc>::insert(const value_type & v)
	{
		link_type ans = nullptr;
		root=(link_type)__insert(root,v,ans);
		return ans;
	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	bool avl_tree<Value,Compare, Alloc>::remove(const value_type & v)
	{
		bool found = false;
		root =(link_type) __remove(root, v,found);
		return found;
	}

	template<typename Value, typename Compare, template<typename> class Alloc>
	typename avl_tree<Value,Compare, Alloc>::link_type
		avl_tree<Value,Compare, Alloc>::search(const value_type & v)
	{
		return (link_type)__search(root, v);
	}


	template<typename Value,template<typename>class Alloc=std::allocator>
	using simple_avlTree = avl_tree<Value,std::less<Value>,Alloc>;

}