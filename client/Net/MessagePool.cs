using System;
using System.Collections.Generic;
using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace Net
{
    public delegate void MsgHandler( Message msg );

	public class MessagePool
	{
		private LoginModule loginModule = null;
		
		public MessagePool()
		{
			m_buffer            = new byte[4096];
			m_MsgPool           = new Queue<Message>();
            m_CallBacks = new  Dictionary<string, MsgHandler>();
			Reset();
			
			loginModule = ModuleManager.Instance.GetModule<LoginModule>();
		}

		public void parseBuffer( byte[] buffer, int size, ProtocolType protocol )
		{
			parseBuffer ( buffer, size, protocol, 0 );
		}

		public void parseBuffer( byte[] buffer, int size, ProtocolType protocol, UInt32 ack )
		{
			int inPosition = 0;
            while (inPosition<size)
			{
				if( !UdpClient.GetInstance().GetThreadsState() ) {

					Reset();
					return;
				}

				if( m_MessageSize == 0 )
				{
					int NEED_LEN = MSG_LEN-m_Position;
					if( size-inPosition >= NEED_LEN )
					{
						Array.Copy( buffer, inPosition, m_buffer, m_Position, NEED_LEN );
						Array.Copy( m_buffer, m_MsgLenBytes, MSG_LEN );
						m_MessageSize = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(m_MsgLenBytes, 0));
						inPosition += NEED_LEN;
						
						if( size-inPosition == 0 )
						{
							return;
						}
					}
					else
					{
						Array.Copy( buffer, inPosition, m_buffer, m_Position, size-inPosition );
						m_Position += size-inPosition;
						return;
					}
				}
				
				if( size-inPosition < m_MessageSize )
				{
					Array.Copy( buffer, inPosition, m_buffer, m_Position , size-inPosition );
					m_Position    += ( size-inPosition );
					m_MessageSize -= ( size-inPosition );
					return;
				}
				else
				{
					if( inPosition == 0 )
					{
						//m_Position == sizeof(MSG_LEN)
						Array.Copy(buffer, inPosition, m_buffer, m_Position , m_MessageSize);
						inPosition += m_MessageSize;
						m_MessageSize += m_Position;
					}
					else
					{
						//m_Position == sizeof(MSG_LEN)
						Array.Copy(buffer, inPosition, m_buffer, 0, m_MessageSize);
						inPosition += m_MessageSize;
					}


                    Message msg = new Message();
                    msg.init(m_buffer,m_MessageSize);
					msg.SetProtocol(protocol);
					msg.ack = ack;

                    lock (m_lock)
                    {
                       m_MsgPool.Enqueue(msg);
                    }
 
					m_Position = 0;
					m_MessageSize = 0;
				}
			}
		}

        public bool addCallback(string msgtype, MsgHandler callback) 
		{
			if( !m_CallBacks.ContainsKey(msgtype) )
			{
				m_CallBacks[msgtype] = callback;
                return true;
			}
            return false;
		}
		
		public void update()
		{
			if ( m_MsgPool.Count <= 0 ) {
				return;
			}

			Message msg;
			lock ( m_lock )
			{
				msg = m_MsgPool.Dequeue();
			}

			execute( msg );
		}
		
		private void execute( Message msg )
		{
			if( m_CallBacks.ContainsKey(msg.getType()) )
			{
				NetManager.GetInstance().ProcessMsg(msg);
				m_CallBacks[msg.getType()](msg);

				GameDebug.Log ("Execute: "+msg.getType() +"  ack_idx:"+msg.ack +" size:"+msg.getSize());
			}
			else
			{
				GameDebug.Log ("Msg Not Found Callback: "+msg.getType() +"  ack_idx:"+msg.ack +" size:"+msg.getSize());
			}
		}

		public void Reset()
		{
			m_MessageSize = 0;
			m_Position    = 0;
		}
	    
		private const int 		MSG_LEN = 4;

		private byte[] 			m_MsgLenBytes = new byte[MSG_LEN];
		private System.Int32 	m_MessageSize;
		private int     		m_Position;
		private byte[]  		m_buffer;
        private static object	m_lock = new object();
		private Queue<Message>	m_MsgPool;
        private Dictionary<string, MsgHandler> m_CallBacks;
	}
}