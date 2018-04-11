#pragma once
#include<xmemory>

#define _XHY_BEGIN namespace xhy{
#define _XHY_END }
#define _XHY ::xhy::
#define _NOEXCEPT noexcept
#define _NOEXCEPT_OP(x) noexcept(x)

_XHY_BEGIN

using _Atomic_counter_t = unsigned long;
using _Atomic_integral_t = unsigned long;

class _Ref_count_base
{
private:
	virtual void _Destroy() _NOEXCEPT = 0;
	virtual void _Delete_this() _NOEXCEPT = 0;
private:
	_Atomic_counter_t _Uses;
	_Atomic_counter_t _Weaks;
protected:
	_Ref_count_base()
		:_Uses(1), _Weaks(1)
	{
	}
public:
	virtual ~_Ref_count_base() _NOEXCEPT
	{
	}
	bool _Incref_nz()
	{
		for (;;)
		{
			_Atomic_counter_t _Count =
				static_cast<volatile _Atomic_counter_t&>(_Uses);
			if (_Count == 0)
				return (false);
			if(static_cast<_Atomic_integral_t>(_InterlockedCompareExchange(
				reinterpret_cast<volatile long *>(&_Uses),
				_Count + 1, _Count)) == _Count)
				return (true);
		}
	}
	void _Incref()
	{
		_MT_INCR(_Uses);
	}
	void _Incwref()
	{
		_MT_INCR(_Weaks);
	}
	void _Decref()
	{
		if (_MT_DECR(_Uses) == 0)
		{
			_Destroy();
			_Decwref();
		}
	}
	void _Decwref()
	{
		if (_MT_DECR(_Weaks) == 0)
			_Delete_this();
	}
	long _Use_count()const _NOEXCEPT
	{
		return std::_Get_atomic_count(_Uses);
	}
	bool _Expired() const _NOEXCEPT
	{
		return (_Use_count() == 0);
	}
	virtual void *_Get_deleter(const type_info&)const _NOEXCEPT
	{
		return 0;
	}
};

template<typename _Ty>
class _Ref_count:public _Ref_count_base
{
public:
	_Ref_count(_Ty *_Px)
		:_Ref_count_base(), _Ptr(_Px)
	{
	}
private:
	virtual void _Destroy() _NOEXCEPT
	{
		delete _Ptr;
	}
	virtual void _Delete_this() _NOEXCEPT
	{
		delete this;
	}
	_Ty *_Ptr;
};

template<typename _Ty,typename _Dx>
class _Ref_count_del :public _Ref_count_base
{
public:
	_Ref_count_del(_Ty *_Px, _Dx _Dt)
		:_Ref_count_base(), _Mypair(std::_One_then_variadic_args_t, _Dt, _Px)
	{
	}
	virtual void *_Get_deleter(const type_info& _Typeid) const _NOEXCEPT
	{	// return address of deleter object
		return ((void *)(_Typeid == typeid(_Dx)
			? _STD addressof(_Mypair._Get_first()) : 0));
	}

private:
	virtual void _Destroy() _NOEXCEPT
	{	// destroy managed resource
		_Mypair._Get_first()(_Mypair._Get_second());
	}

	virtual void _Delete_this() _NOEXCEPT
	{	// destroy self
		delete this;
	}

	std::_Compressed_pair<_Dx, _Ty*> _Mypair;
};

template<class _Ty,
	class _Dx,
	class _Alloc>
	class _Ref_count_del_alloc
	: public _Ref_count_base
{	// handle reference counting for object with deleter and allocator
public:
	typedef _Ref_count_del_alloc<_Ty, _Dx, _Alloc> _Myt;
	typedef _Wrap_alloc<_Alloc> _Myalty0;
	typedef typename _Myalty0::template rebind<_Myt>::other _Myalty;

	_Ref_count_del_alloc(_Ty *_Px, _Dx _Dt, const _Alloc& _Ax)
		: _Ref_count_base(), _Mypair(_One_then_variadic_args_t(), _Dt,
			_One_then_variadic_args_t(), _Ax, _Px)
	{	// construct
	}

	virtual void *_Get_deleter(const type_info& _Typeid) const _NOEXCEPT
	{	// return address of deleter object
		return ((void *)(_Typeid == typeid(_Dx)
			? _STD addressof(_Mypair._Get_first()) : 0));
	}

private:
	virtual void _Destroy() _NOEXCEPT
	{	// destroy managed resource
		_Mypair._Get_first()(_Mypair._Get_second()._Get_second());
	}

	virtual void _Delete_this() _NOEXCEPT
	{	// destroy self
		_Myalty _Al = _Mypair._Get_second()._Get_first();
		_Al.destroy(this);
		_Al.deallocate(_Al.address(*this), 1);
	}

	_Compressed_pair<_Dx, _Compressed_pair<_Myalty, _Ty *> > _Mypair;
};

template<class _Ty>
class weak_ptr;
template<class _Ty>
class shared_ptr;
template<class _Ty>
class enable_shared_from_this;

template<class _Ty1,
	class _Ty2>
	void _Do_enable(_Ty1 *, enable_shared_from_this<_Ty2> *,
		_Ref_count_base *);

template<class _Ty>
inline void _Enable_shared(_Ty *_Ptr, _Ref_count_base *_Refptr,
	typename _Ty::_EStype * = 0)
{	// reset internal weak pointer
	if (_Ptr)
		_Do_enable(_Ptr,
		(enable_shared_from_this<typename _Ty::_EStype>*)_Ptr, _Refptr);
}

inline void _Enable_shared(const volatile void *, const volatile void *)
{	// not derived from enable_shared_from_this; do nothing
}


template<typename _Ty>
class _Ptr_base
{
	
public:
	typedef _Ptr_base<_Ty> _Myt;
	typedef _Ty element_type;

	constexpr _Ptr_base() _NOEXCEPT;
private:
	_Ty *_Ptr;
	

};
_XHY_END