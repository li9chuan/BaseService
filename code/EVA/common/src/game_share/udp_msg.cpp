#include "udp_msg.h"
#include "game_def.h"

// for htonl
#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#elif defined NL_OS_UNIX
#	include <arpa/inet.h>
#endif


void UDPToClientMsg::AddBuffer( uint8 *buf, uint32 len )
{
    uint32 net_length = htonl(len);
    serial(net_length);
    serialBuffer(buf,len);

    /////   填充数据                  控制位(1)  ACK_IDX(4)   MSG_LEN(4)

    //const uint  POKE_MSGLEN_IDX = sizeof(uint8) + sizeof(DEF::ACK_IDX);
    //const uint  HEAD_SIZE       = sizeof(uint8) + sizeof(DEF::ACK_IDX) + sizeof(uint32);


    //uint32 mem_length = htonl(length()-HEAD_SIZE);
    //poke(mem_length, POKE_MSGLEN_IDX);
}

