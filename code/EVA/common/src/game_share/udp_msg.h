#ifndef GAME_SHARE_UDP_MSG_H
#define GAME_SHARE_UDP_MSG_H

#include <nel/misc/mem_stream.h>
#include "define_sys.pb.h"

class UDPToServerMsg : public NLMISC::CMemStream
{
public:
    UDPToServerMsg():m_ControlFlag(0)
    {
        clear();
        serial (m_ControlFlag);     //  协议控制位
    }

    void  SetControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag |= flag;
        poke(m_ControlFlag,0);
    }

    void ClearControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag &= ~uint8(flag);
        poke(m_ControlFlag,0);
    }

    void AddBuffer(uint8 *buf, uint len)
    {
        serialBuffer(buf,len);
    }

private:

    uint8    m_ControlFlag; 
};

class UDPToClientMsg : public NLMISC::CMemStream
{
public:
    UDPToClientMsg():m_ControlFlag(0)
    {
        reset();
    }

    void reset()
    {
        clear();

        serial (m_ControlFlag);     //  协议控制位

        uint32  ack_idx=0;
        serial(ack_idx);
    }

    void SetControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag |= flag;
        poke(m_ControlFlag,0);
    }

    void ClearControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag &= ~uint8(flag);
        poke(m_ControlFlag,0);
    }

    void SetAckIdx( uint32 ack_idx )
    {
        ///   填充ACK_IDX
        poke(ack_idx,1);
    }

    void AddBuffer(uint8 *buf, uint32 len);


private:

    uint8    m_ControlFlag; 
};

class UDPCommand : public NLMISC::CMemStream
{
public:
    UDPCommand( TUdpControlCmd cmd ):m_ControlFlag(PROTO_CMD),m_Command(cmd)
    {
        clear();
        serial (m_ControlFlag);     //  协议控制位
        serial (m_Command);         //  命令
    }

    void  SetControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag |= flag;
        poke(m_ControlFlag, 0);
    }

    void ClearControlFlag( TUdpControlFlag flag )
    {
        m_ControlFlag &= ~uint8(flag);
        poke(m_ControlFlag, 0);
    }

    void  SetCommand( TUdpControlCmd cmd )
    {
        m_Command = cmd;
        poke(m_Command, 1);
    }

private:

    uint8    m_ControlFlag; 
    uint8    m_Command;
};



#endif
