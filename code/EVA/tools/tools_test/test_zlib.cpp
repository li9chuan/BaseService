#include "test_misc.h"
#include "zlib.h"

using namespace std;


void test_zlib( string& str )
{
    cout<<"---------------  test_zlib ----------------"<<endl;

    uLong destLen = compressBound(str.size()); /* 压缩后的长度是不会超过blen的 */  

    Bytef* buf = NULL;
    if((buf = (Bytef*)malloc(sizeof(Bytef) * destLen)) == NULL)  
    {  
        printf("no enough memory!\n");  
    }
    string bufstr;   bufstr.resize(destLen);

    cout<<"destLen:"<<destLen<<"    str.size:"<<str.size()<<endl;

    int err = compress( (Bytef*)&bufstr[0], &destLen, (const Bytef*)str.data(), str.size());
    if( err != Z_OK)  
    {  
        printf("compress failed!\n");  
    }

    cout<<"comp len:"<<destLen<<endl;

    //if(uncompress(text, &tlen, buf, blen) != Z_OK)  
    //{  
    //    printf("uncompress failed!\n");  
    //    return -1;  
    //}  

}

