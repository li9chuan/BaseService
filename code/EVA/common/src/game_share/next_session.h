#ifndef FES_NEXT_SESSION_H
#define FES_NEXT_SESSION_H

#include <nel/misc/types_nl.h>

namespace NLMISC
{

/** next session generator.
  * \author li9chuan@qq.com
  * \date 2015
  */
class CNextSession
{
public:
	// The max value that can be returned by the session generator
	enum { TSessionMax = 0x7fff };
public:
	// ctor
	inline CNextSession();

    /// 生成下一个session  [0, TSessionMax]
	inline sint32   next();

    /// 设置初始种子
	inline void     set_seed(sint32 seed);
private:
	sint32 _Seed;
};


//===========================================================================
inline CNextSession::CNextSession() : _Seed(1)
{
}

//===========================================================================
// NB : In fact this random generator has the same behaviour than the VC6 one
inline sint32 CNextSession::next()
{
	return ((_Seed = _Seed * 214013L + 2531011L) >> 16) & TSessionMax;
}

//===========================================================================
inline void CNextSession::set_seed(sint32 seed)
{
	_Seed = seed;
}



}

#endif


