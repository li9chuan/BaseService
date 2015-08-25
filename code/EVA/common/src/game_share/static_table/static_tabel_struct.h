#ifndef static_tabel_struct_h__
#define static_tabel_struct_h__
#include <vector>
#include <nel/misc/sstring.h>

///  各种公式
struct Formula
{
    NLMISC::CSString    rmb2money;
    NLMISC::CSString    rmb2soul;
};

typedef std::map<uint32/*品质*/,uint32/*需要的生长时间*/>     TGrowTime;

struct GameConfig
{
    TGrowTime           grow_time;
    Formula             formula;
};


#endif // static_tabel_struct_h__
