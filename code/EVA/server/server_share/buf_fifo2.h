#ifndef NL_BUF_FIFO_DOUBLE_H
#define NL_BUF_FIFO_DOUBLE_H

#include <nel/misc/types_nl.h>
#include <vector>

namespace NLMISC {

template<class T>
class CBufFIFO2
{
public:
	CBufFIFO2 ():_PosReadMax(0),_PosRead(0),_PosWrite(0),_pRead(NULL),_pWrite(NULL),_MaxSize(0){}
	~CBufFIFO2 (){}

	void init( uint32 size )
	{
		_MaxSize = size;
		_Data1.resize( _MaxSize );
		_Data2.resize( _MaxSize );
		_pRead = &_Data1;
		_pWrite = &_Data2;
	}

	bool push_back (T* data)
	{
		_WriteMutex.enter();
		if( _PosWrite < _MaxSize )		//	如果队列未满
		{
			(*_pWrite)[_PosWrite++] = data;
			_WriteMutex.leave();
			return true;
		}
		_WriteMutex.leave();
		return false;
	}

    //   交换
    void swap()
    {
        _ReadMutex.enter();
        _WriteMutex.enter();

        _PosReadMax = _PosWrite;
        _PosWrite   = 0;
        _PosRead    = 0;
        std::vector<T*>* pTmp = _pRead;
        _pRead  = _pWrite;
        _pWrite = pTmp;

        _WriteMutex.leave();
        _ReadMutex.leave();
    }

	T* pop_front()
	{
		T* pRet = NULL;

		_ReadMutex.enter();
		if( _PosRead < _PosReadMax )
		{
			pRet = (*_pRead)[_PosRead++];
		}
		_ReadMutex.leave();

        if ( pRet==NULL )
        {
            swap();
        }
		return pRet;
	}

	uint32 size()
	{
		return _PosWrite + _PosReadMax - _PosRead;
	}

private:

	CMutex _WriteMutex;
	CMutex _ReadMutex;
	uint32 _PosRead;
    uint32 _PosReadMax;
	uint32 _PosWrite;
	uint32 _MaxSize;

	std::vector<T*>* _pRead;
	std::vector<T*>* _pWrite;
	std::vector<T*> _Data1;
	std::vector<T*> _Data2;
};


} // NLMISC

#endif // NL_BUF_FIFO_DOUBLE_H

/* End of buf_fifo2.h */
