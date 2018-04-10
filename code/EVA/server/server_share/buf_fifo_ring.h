#ifndef NL_BUF_FIFO_RING_H
#define NL_BUF_FIFO_RING_H

#include <nel/misc/types_nl.h>
#include <vector>

namespace NLMISC {

template<class T>
class CBufFIFORing
{
public:

	CBufFIFORing ():_PosRead(0),_PosWrite(0),_MaxSize(1)
	{
		_Data.resize(1);
	}
	~CBufFIFORing (){};

	uint32 init( uint32 size )	//  扩展成2的倍数,<=2147483648。
	{
		if( size < 0x80000000 )
		{
			for( uint32 ndx = 0; ndx < 32; ndx++ )
			{
				_MaxSize = ( _MaxSize << 1 );
				if( _MaxSize >= size )
				{
					break;
				}
			}
		}
		else
		{
			_MaxSize = 0x80000000;
		}

		_Mask = _MaxSize - 1;
		_Data.resize( _MaxSize );
		return _MaxSize;
	}

	bool push_back (T* data)
	{
		bool res = false;
		
		if( _PosWrite - _PosRead < _MaxSize )		//	如果队列未满
		{
			_mutex.enter();
			uint32 write = _PosWrite++;
			_Data[write&_Mask] = data;
			_mutex.leave();
			res = true;
		}
		
		return res;
	}

	T* pop_front()
	{
		T* res = NULL;
		
		if( _PosRead != _PosWrite )		//	如果队列不为空
		{
			_mutex.enter();
			uint32 read = _PosRead++;
			res = _Data[read&_Mask];
			_mutex.leave();
		}
		
		return res;
	}

	uint32 size()
	{
		return _PosWrite - _PosRead;
	}

    bool empty( void )
    {
        return ( size() <= 0 );
    }

	void clear()
	{
		_PosRead  = 0;
		_PosWrite = 0;
	}

private:
	CMutex _mutex;
	volatile uint32 _PosRead;
	volatile uint32 _PosWrite;
	uint32  _MaxSize;
	uint32  _Mask;

	std::vector<T*> _Data;
};


} // NLMISC


#endif // NL_BUF_FIFO_RING_H

/* End of buf_fifo_ring.h */
