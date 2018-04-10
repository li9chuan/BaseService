//
//	CObjectPool:


#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <string>
#include <vector>

#include <nel/misc/types_nl.h>
#include <nel/misc/debug.h>
#include "buf_fifo2.h"


namespace NLMISC {

template<class T>
class CObjectPool
{
public:
	CObjectPool();
	~CObjectPool();

	inline bool	init( std::string name, sint32 max );
	inline T*	alloc();
	inline void	free( T *pMem );
	inline std::string report();
	inline uint32 use()	{	return _fifo2.size();	}

private:
	std::string		_name;			///	对象池的标识
	uint32			_max;			///	对象池的Obj最大个数
	std::vector<T>  _pool;
	CBufFIFO2<T>    _fifo2;
};

template<class T>
CObjectPool<T>::CObjectPool():
_name(""),_max(0)
{}

template<class T>
CObjectPool<T>::~CObjectPool()
{}

template<class T>
inline bool CObjectPool<T>::init(std::string name,sint32 max)
{
	_name		= name;
	_max		= max;

	_pool.resize(_max);
	_fifo2.init(_max);

	for( uint32 i=0; i<_max; ++i )
	{
		_fifo2.push_back(&(_pool[i]));
	}

	return true;
}

template<class T>
inline T* CObjectPool<T>::alloc()
{
	return _fifo2.pop_front();
}

template<class T>
inline void CObjectPool<T>::free(T *pMem)
{
	pMem->reset();
	_fifo2.push_back(pMem);
}

template<class T>
inline std::string CObjectPool<T>::report()
{
	std::string str_report = "";	// use block : _fifo.size()/_max   ;   use memory : (_fifo.size()*sizeof(T))/(_max*sizeof(T))
	return str_report;
}

}

#endif
