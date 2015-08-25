using UnityEngine;
using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections.Generic;
using ATTRIB_TYPE;
using ZhiWa;
using System.IO;

namespace Net
{
	public class UdpClient
	{
		public static UdpClient GetInstance()
		{
			if( null == Instance )
			{
				Instance = new UdpClient();
			}
			return Instance;
		}
		
		UdpClient()
		{
			CreateThreads();
		}
		
		public void SendToServer( string ip , int port , Message msg, bool need_ack, bool auto_close_ui )
		{
			IPAddress serverip;
			
			if( !IPAddress.TryParse( ip , out serverip ) )
			{
				IPAddress[] array = Dns.GetHostAddresses( ip );
				serverip = array[ 0 ];
			}
			
			if( null == SendHost ) {
				SendHost = new IPEndPoint( serverip , port );
			}
			else
			{
				if( SendHost.Address.ToString() != ip || 
				   SendHost.Port != port )
				{
					SendHost = new IPEndPoint( serverip , port );
					RecvHost = SendHost as EndPoint;
				}
			}
			
			if( null == RecvHost ) {
				RecvHost = SendHost as EndPoint;
			}
			
			CreateThreads();
			
			msg.SetUdpControlFlag (TUdpControlFlag.PROTO_NEED_ACK, need_ack);

			SendBaseData BaseData = new SendBaseData();
			BaseData.bIsAutoClose = auto_close_ui;
			BaseData.strMsgName   = msg.getType();
			BaseData.nTimeOut     = System.Environment.TickCount;
			SendAckQueue.Add(BaseData);

			PushMsg( msg );
		}
		
		public bool GetThreadsState()
		{
			return IsRunThreads;
		}
		
		public void CreateThreads()
		{
			try {
				
				if ( IsRunThreads ) {
					return;
				}
				
				// 线程运行标记;
				IsRunThreads = true;
				
				SendThreads = new Thread( SendRun );
				//SendThreads.IsBackground = true; // 标记后台线程;
				//SendThreads.Name = "SendRun";
				SendThreads.Start();
				
				RecvThreads = new Thread( RecvRun );
				//RecvThreads.IsBackground = true; // 标记后台线程;
				//RecvThreads.Name = "RecvRun";
				RecvThreads.Start();
				
				UdpSocket = new Socket( AddressFamily.InterNetwork , SocketType.Dgram , ProtocolType.Udp );
				
				// set recv timeout;
				// UdpSocket.SetSocketOption( SocketOptionLevel.Socket , SocketOptionName.ReceiveTimeout , 5000 );
			}
			catch( System.Exception what ) {
				
				Debug.Log( "create threads faile " + what.ToString() );
			}
		}
		
		public void CloseThreads()
		{
			try {
				IsRunThreads = false;
				
				if( null != UdpSocket ) {
					UdpSocket.Close();
					UdpSocket = null;
				}
				
				ClearData();
			}
			catch( System.Exception what ) {
				
				Debug.Log( "close threads faile " + what.ToString() );
			}
		}
		
		public void ClearData()
		{
			lock( LockMsg )
			{
				MsgQueue.Clear();
			}

			lock (LockErrID) 
			{
				ErrorQueue.Clear();			
			}
			
			NetManager.GetInstance().MsgPool.Reset();
		}
		
		public void OnUpdate()
		{
			if ( ErrorQueue.Count <= 0 ) {
				return;
			}

			lock ( LockErrID )
			{
				SocketError errid = ErrorQueue.Dequeue();
				
				// 处理系错误;
				if( SocketError.Success != errid ) {
					
					switch( errid )
					{
					case SocketError.SocketError:
					case SocketError.ConnectionReset:
					case SocketError.TimedOut:
					case SocketError.ConnectionRefused:{
						UIManager.GetInstance().CloseUI( EnumUIType.MtConnectingGUI );
					}
						break;
					default:
						break;
					}
				}
			}
		}
		
		public void ProcessMsg( Message msg )
		{
			if( null == msg ){
				return;
			}

			for ( int i = SendAckQueue.Count-1 ; i >= 0 ; i-- )
			{
				SendBaseData BaseData = SendAckQueue[ i ];
				if ( null == BaseData ) {
					continue;
				}

				// 检查超时；
				if ( System.Environment.TickCount - BaseData.nTimeOut >= 30000 ) {
					SendAckQueue.Remove ( BaseData );
					continue;
				}

				// 检查ACK索引;
				if ( BaseData.strMsgName == msg.getType() ) {
					if ( BaseData.bIsAutoClose ) {
						UIManager.GetInstance().CloseUI( EnumUIType.MtConnectingGUI );
					}
					
					SendAckQueue.Remove ( BaseData );
					break;
				}
			}
		}
		
		private void SendRun( )
		{
			DateTime LastSendMsgTime = DateTime.UtcNow;
			
			while( IsRunThreads )
			{
				try
				{
					if( null != UdpSocket ) 
					{
						if( SendWaitMsg() )
						{
							LastSendMsgTime = DateTime.UtcNow;
							continue;
						}
						
						TimeSpan timeSpan = DateTime.UtcNow - LastSendMsgTime;
						
						if( timeSpan.TotalSeconds > 30 && SendHost!=null )
						{
							if( NetManager.GetInstance().NetState )
							{
								SendHeartBeat();
							}
							
							LastSendMsgTime = DateTime.UtcNow;
						}
					}
					
					Thread.Sleep(10); 
				}
				catch( System.ComponentModel.Win32Exception what ) 
				{
					lock ( LockErrID )
					{
						ErrorQueue.Enqueue( ( SocketError )what.NativeErrorCode );
					}
				}
				
				catch( System.Exception what ) {
					
					Debug.Log( " send faile " + what.ToString() );
				}
			}
		}
		
		
		
		private void RecvRun( )
		{
			while( IsRunThreads )
			{
				try
				{
					if( null != UdpSocket )
					{
						if( UdpSocket.Available > 0 )
						{
							int RecvSize = UdpSocket.ReceiveFrom( RecvBuffer , ref RecvHost );
							
							if( RecvSize < 2 )
							{
								continue;
							}
							
							byte ControlFlag = RecvBuffer[0];
							
							if( IsProtoCMD(ControlFlag) )
							{
								byte ConsoleCmd = RecvBuffer[1];
								
								if( IsCmdACK(ConsoleCmd) && RecvSize==(ControlHeadSize+ACKContentSize) )
								{
									RecvAcK( RecvBuffer, ControlHeadSize );
								}
							}
							else if( IsProtoData(ControlFlag) )
							{
								const int HeadSize = ControlFlagSize + ACKContentSize;
								int DataSize = RecvSize - HeadSize;
								
								byte[] buffer = new byte[ DataSize ];
								Array.Copy( RecvBuffer, HeadSize, buffer, 0, DataSize );
								
								if( IsProtoNeedACK( RecvBuffer[0] ) )
								{
									if( !SendAck( RecvBuffer , RecvSize ) )
										continue;
								}
								
								byte[] AckContent	= new byte[ ACKContentSize ];
								Array.Copy( RecvBuffer , ControlFlagSize , AckContent , 0 , ACKContentSize );
								uint AckIndex = System.BitConverter.ToUInt32( AckContent , 0 );
								
								NetManager.GetInstance().MsgPool.parseBuffer( buffer, DataSize, ProtocolType.Udp, AckIndex );
							}
							
							continue;
						}
					}
					
					Thread.Sleep(2);
				}
				
				catch( System.ComponentModel.Win32Exception what ) {
					lock ( LockErrID )
					{
						ErrorQueue.Enqueue( ( SocketError )what.NativeErrorCode );
					}
					Debug.Log( " recv faile " + what.ToString() );
				}
				
				catch( System.Exception what ) {
					Debug.Log( " recv faile " + what.ToString() );
				}
			}
		}
		
		private bool SendWaitMsg()
		{
			if ( MsgQueue.Count <= 0 ) {
				return false;
			}
			
			lock ( LockMsg )
			{
				for ( int i = 0 ; i < MsgQueue.Count; i++ )
				{
					Message CurMsg = MsgQueue[ i ];
					if ( CurMsg.IsEmptyTimeOut() ) {
						MsgQueue.Remove ( CurMsg );
						continue;
					}
					
					int nDifference	= System.Environment.TickCount - CurMsg.EnterMsgQueueTime;
					int nTimeOut	= CurMsg.GetMsgTimeout();
					
					if( nDifference >= nTimeOut ) {
						CurMsg.RemoveMsgTimeOut();
						
						_SendBuffer[0] = CurMsg.GetUdpControlFlag();
						Array.Copy( CurMsg.getBuffer(), 0, _SendBuffer, ControlFlagSize, CurMsg.getSize() );
						
						int SendSize = UdpSocket.SendTo( _SendBuffer, CurMsg.getSize()+ControlFlagSize , SocketFlags.None , SendHost );
						
						GameDebug.Log ("UDP Send: " + CurMsg.getType () + "  Idx:" + CurMsg.MsgIndex + "  TimeOut:" + nTimeOut );
						
						if( !IsProtoNeedACK( CurMsg.GetUdpControlFlag() ) ) 
						{
							MsgQueue.Remove( CurMsg );
						}
					} 
				}
			}
			
			return true;
		}
		
		// not user;
		private void RecvWaitMsg( string name )
		{
			if ( MsgQueue.Count <= 0 ) {
				return;
			}
			
			bool result = false;
			
			lock ( LockMsg ) 
			{
				for ( int i = 0 ; i < MsgQueue.Count; i++ )
				{
					Message CurMsg = MsgQueue[ i ];
					if ( null == CurMsg ) {
						continue;
					}
					
					if ( CurMsg.getType() == name ) {
						MsgQueue.Remove( CurMsg );
						result = true;
						break;
					}
				}
			}
			
			if ( result ) 
			{
				lock ( LockErrID )
				{
					ErrorQueue.Enqueue ( SocketError.SocketError );
				}
			}
		}
		
		private void PushMsg( Message msg )
		{
			msg.EnterMsgQueueTime = System.Environment.TickCount;
			
			lock ( LockMsg ) 
			{
				MsgQueue.Add ( msg );
			}
		}
		
		private bool RecvAcK( byte[] buff , int offset )
		{
			// ACK 解析数据包;
			byte[] CheckByte	= new byte[ ACKContentSize ];
			Array.Copy( buff , offset , CheckByte , 0 , ACKContentSize );
			
			Int32	MsgContent  = BitConverter.ToInt32( CheckByte, 0 );
			
			if ( MsgQueue.Count <= 0 ) {
				return true;
			}
			
			lock ( LockMsg ) 
			{
				for ( int i = 0 ; i < MsgQueue.Count; i++ )
				{
					Message CurMsg = MsgQueue[ i ];
					if ( null == CurMsg ) {
						continue;
					}
					if ( CurMsg.MsgIndex == MsgContent ) {
						 MsgQueue.Remove ( CurMsg );
						 break;
					}
				}
			}
			
			return true;
		}
		
		private MsgSession msgSession = new MsgSession();
		ProtobufSerializer Serial = new ProtobufSerializer ();
		
		private void SendHeartBeat()
		{
			msgSession.index = NetManager.GetInstance().GetBuffIndex();
			msgSession.check = NetManager.GetInstance().GetRand();
			msgSession.session = (ulong)Game.Instance.GameState[ GameStateKey.HeroID ];
			
			MemoryStream sendstream = new MemoryStream(); 
			Serial.Serialize (sendstream, msgSession);  
			byte[] data = sendstream.ToArray();
			
			///   构造心跳消息    
			byte[] HeartBeatBuff = new byte[ ControlHeadSize + sizeof(System.Int32) + data.Length ];
			///   填充心跳协议头   ConsoleFlag(8bit)  ACK_CMD(8bit)
			HeartBeatBuff [0] =   (byte)TUdpControlFlag.PROTO_CMD;				//  协议控制位   PROTO_NEED_ACK=0
			HeartBeatBuff [1] =   (byte)TUdpControlCmd.PROTO_CMD_HEARTBEAT;		//  HEARTBEAT 指令
			
			byte[] proto_data_head = BitConverter.GetBytes(data.Length);
			Array.Copy ( proto_data_head, 0, HeartBeatBuff, ControlHeadSize, 4 );
			Array.Copy ( data, 0, HeartBeatBuff, ControlHeadSize+sizeof(System.Int32), data.Length );
			
			//byte[] session_bytes = BitConverter.GetBytes( ( ulong )Game.Instance.GameState[ GameStateKey.HeroID ] );
			//Array.Copy (session_bytes, 0, HeartBeatBuff, ControlHeadSize, sizeof(ulong));
			
			int SendSize = UdpSocket.SendTo (HeartBeatBuff, HeartBeatBuff.Length, SocketFlags.None, SendHost);
			GameDebug.SaveLog (" send heart beat size " + SendSize);
		}
		
		private byte[] AckBuff = new byte[ ControlHeadSize + ACKContentSize ];
		private bool SendAck( byte[] buff , int size )
		{
			if( size <= sizeof( uint ) )
				return false;
			
			///   填充ACK返回消息
			{
				AckBuff [0] = (byte)TUdpControlFlag.PROTO_CMD;			//  协议控制位
				AckBuff [1] = (byte)TUdpControlCmd.PROTO_CMD_ACK;		//  ACK 指令
				
				//  ConsoleFlag(8bit)  ACK_CMD(8bit)  ACK_IDX(32bit)
				
				Array.Copy (buff, ControlFlagSize, AckBuff, ControlHeadSize, ACKContentSize);
				int SendSize = UdpSocket.SendTo (AckBuff, AckBuff.Length, SocketFlags.None, SendHost);
			}
			
			///   
			byte[] AckContent	= new byte[ ACKContentSize ];
			Array.Copy( buff , ControlFlagSize , AckContent , 0 , ACKContentSize );
			uint AckIndex = System.BitConverter.ToUInt32( AckContent , 0 );
			return PushAckOverlap( AckIndex );
		}
		
		private bool PushAckOverlap( uint msgAck )
		{
			try
			{
				if( AckQueue.Contains( msgAck ) ) {
					return false;
				}
				
				if( AckQueue.Count >= 150 ) {
					AckQueue.Dequeue();
				}
				
				AckQueue.Enqueue( msgAck );
			}
			catch( System.Exception what )
			{
				Debug.Log( " push ack exception " + what.ToString() );
			}
			
			//Debug.Log (" handle ack " + msgAck.ToString ()); 
			
			return true;
		}
		
		/// <summary>
		/// Determines whether this instance is proto CM the specified ConsoleFlag.
		/// </summary>
		/// <returns><c>true</c> if this instance is proto CM the specified ConsoleFlag; otherwise, <c>false</c>.</returns>
		/// <param name="ConsoleFlag">Console flag.</param>
		
		private bool IsProtoCMD( byte ConsoleFlag ) {
			return (ConsoleFlag&(byte)TUdpControlFlag.PROTO_CMD) > 0;
		}
		
		private bool IsProtoData( byte ConsoleFlag ) {
			return (ConsoleFlag&(byte)TUdpControlFlag.PROTO_CMD) == 0;
		}
		
		private bool IsProtoNeedACK( byte ConsoleFlag ) {
			return (ConsoleFlag&(byte)TUdpControlFlag.PROTO_NEED_ACK) > 0;
		}
		
		/// <summary>
		/// Determines whether this instance is cmd AC the specified ConsoleCmd.
		/// </summary>
		/// <returns><c>true</c> if this instance is cmd AC the specified ConsoleCmd; otherwise, <c>false</c>.</returns>
		/// <param name="ConsoleCmd">Console cmd.</param>
		private bool IsCmdACK( byte ConsoleCmd ) {
			return (ConsoleCmd&(byte)TUdpControlCmd.PROTO_CMD_ACK) > 0;
		}
		
		private Thread						SendThreads;		// 发送线程;
		private Thread						RecvThreads;		// 接受线程;
		private Socket						UdpSocket;			// 收发终端;
		private IPEndPoint					SendHost = null;	// 发送地址;
		private EndPoint					RecvHost = null;	// 接受地址;
		private static UdpClient 			Instance;
		private List< Message >				MsgQueue 	 = new List< Message >();
		private Queue< SocketError >		ErrorQueue	 = new Queue< SocketError >();
		private Queue< uint >               AckQueue     = new  Queue< uint >();
		private static object 				LockMsg		 = new object();
		private static object				LockErrID    = new object();
		private byte[]						RecvBuffer 	 = new byte[4096];
		private bool 						IsRunThreads = false;
		private byte[]                      _SendBuffer  = new byte[4096];
		private List< SendBaseData >		SendAckQueue = new List< SendBaseData >();

		private const int  ControlFlagSize	   = sizeof( System.Byte );
		private const int  ControlCmdSize	   = sizeof( System.Byte );
		private const int  ACKContentSize      = sizeof( System.Int32 );
		private const int  ControlHeadSize     = ControlFlagSize + ControlCmdSize;
	}
}
