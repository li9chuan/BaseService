using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Xml;
using System.IO;
using ZhiWa;

namespace Net
{
	public class SendBaseData
	{
		public string   strMsgName   = "";
		public bool     bIsAutoClose = true;
		public int      nTimeOut     = 0;
	}

	public class NetManager
	{
		public enum ConnectTime : uint{

			MinTimeOut = 10000 ,
			BigTimeOut = 15000 ,
		}

		public static NetManager GetInstance()
		{
			if( null == Instnace ) {
				Instnace = new NetManager();
			}
			return Instnace;
		}

		NetManager()
		{
			MsgPool = new MessagePool();
		}

		public bool NetState
		{
			get
			{
				return m_NetState;
			}
		}

		private bool m_NetState = true;

		public void OnUpdate()
		{
			Profiler.BeginSample( " udp update " );
			UdpClient.GetInstance().OnUpdate();
			Profiler.EndSample();

			Profiler.BeginSample( " tcp update " );
			//TcpClient.GetInstance().OnUpdate();
			Profiler.EndSample();

			Profiler.BeginSample( " pool msg update " );
			MsgPool.update();
			Profiler.EndSample();

			if (Application.internetReachability == NetworkReachability.NotReachable)
			{
				m_NetState = false;
			}
			else
			{
				m_NetState = true;
			}
		}

		public void SendMsgToServer( string key, ProtoBuf.IExtensible protoMsg)
		{
			MemoryStream msgStream = new MemoryStream(); 
			Game.Serial.Serialize (msgStream, protoMsg);  
			byte[] buf = msgStream.ToArray();

			SendMsgToServer(key,buf);
		}

		public void BackMsgToServer(string key)
		{
			SendMsgToServer(key,m_ZeroBytes,false,false);
		}

		public void BackMsgToServer( string key, ProtoBuf.IExtensible protoMsg)
		{
			MemoryStream msgStream = new MemoryStream(); 
			Game.Serial.Serialize (msgStream, protoMsg);  
			byte[] buf = msgStream.ToArray();
			
			SendMsgToServer(key,buf,false,false);
		}

		public void SendMsgToServer( string key )
		{
			SendMsgToServer (key, m_ZeroBytes, true, true);
		}

		public void SendMsgToServer( string key, byte[] buf )
		{
			SendMsgToServer (key, buf, true, true);
		}

		public void SendMsgToServer( string key, bool open_wait_ui, bool auto_close_ui )
		{
			SendMsgToServer (key, m_ZeroBytes, open_wait_ui, auto_close_ui);
		}

		public void SendMsgToServer( string key, byte[] buf, bool open_wait_ui, bool auto_close_ui )
		{
			Message	msg = new Message( key );
			ulong 	session 	=	( ulong )Game.Instance.GameState[ GameStateKey.HeroID ];
			string	serverhost 	=	( string )Game.Instance.GameState[ GameStateKey.ServerHost ];
			string	serverport 	=	( string )Game.Instance.GameState[ GameStateKey.ServerPort ];
			int		port		=	Convert.ToInt32( serverport );
			msg.MsgIndex = GetBuffIndex();

			uint crc = NetCrC32.crc32 ( 0 , buf.Length);
			if( buf.Length > 0 )
			{
				crc = NetCrC32.crc32 (crc, buf, (uint)buf.Length);
			}

			uint rnd = GetRand() + crc;

			MsgSession msgSession = new MsgSession();
			msgSession.index = msg.MsgIndex;
			msgSession.check = rnd;
			msgSession.session = session;
			MemoryStream sendstream = new MemoryStream(); 
			//封包
			Game.Serial.Serialize (sendstream, msgSession);  
			byte[] data = sendstream.ToArray();
			msg.write( data );
			msg.write( buf );

			// 选择协议;
			//if( IsUdpProtocol( key ) )
			{
				UdpClient.GetInstance().SendToServer( serverhost , port + 1 , msg, true, auto_close_ui );
			}
			//else
			//{
			//	TcpClient.GetInstance().SendToServer( serverhost , port , msg );
			//}

			if (open_wait_ui)
			{
				UIManager.GetInstance ().OpenUI (EnumUIType.MtConnectingGUI);
			}
		}

		public void ClearData()
		{
			UdpClient.GetInstance().ClearData();
			TcpClient.GetInstance().ClearData();
		}

		public bool AddCallback(string msgtype, MsgHandler callback)
		{
			return MsgPool.addCallback(msgtype, callback);
		}
	
		public void ProcessMsg( Message msg )
		{
			if( null == msg ) {
			return;
			}

			if( msg.GetProtocol()==ProtocolType.Udp ) {
				UdpClient.GetInstance().ProcessMsg( msg );
			}
			else {
				TcpClient.GetInstance().ProcessMsg( msg );
			}
		}

		public void SetSeedRandom( int Seed )
		{
			buffindex = 0;
			m_NextSession.set_seed( Seed );
		}

		public void ResetMessagePool()
		{
			MsgPool.Reset();
		}

		/*
		public bool LoadMsgConfig()
		{
			XmlDocument xmldoc = StaticFunction.LoadXml("Config/msg");
			if( null == xmldoc ){
				return false;
			}

			XmlNode node = xmldoc.FirstChild;
			node = node.NextSibling;

			if( null == node ){
			return false;
			}

			XmlNodeList nodelist = node.ChildNodes;

			for( int index = 0 ; index < nodelist.Count; index++ )
			{
				XmlNode rootnode = nodelist[ index ];
				if( rootnode.Name == "leaf" ) {
					string 	 name	= rootnode.Attributes[ "name" ].Value;
					string[] array	= rootnode.Attributes[ "protocol" ].Value.Split(' ');

					if( null != array ) {
						ProtocolType type = ProtocolType.Tcp;
						if( array[0] == "UDP" ) {
							type = ProtocolType.Udp;
						}
						protocol.Add( name , type );
					}
				}
			}

			protocol.Add( MsgKeyValue.LOGIN , ProtocolType.Udp );
			return true;
		}

		private bool IsTcpProtocol( string name )
		{
			if( protocol.ContainsKey( name ) )
			{
				if( protocol[ name ] == ProtocolType.Tcp ) {
					return true;
				}
			}
			return false;
		}
		
		private bool IsUdpProtocol( string name )
		{
			if( protocol.ContainsKey( name ) )
			{
				if( protocol[ name ] == ProtocolType.Udp ) {
					return true;
				}
			}
			return false;
		}
		*/

		public uint GetBuffIndex()
		{
			return buffindex++;
		}

		public uint GetRand()
		{
			return (uint)m_NextSession.next ();
		}

		private uint m_ConnectTimeOut = 7000;
		public  uint ConnectTimeOut
		{
			get{ return m_ConnectTimeOut; }
			set{ m_ConnectTimeOut = value ;}
		}

		public void SetMinTimeOut()
		{
			ConnectTimeOut = ( uint )ConnectTime.MinTimeOut;
		}

		public void SetBigTimeOut()
		{
			ConnectTimeOut = ( uint )ConnectTime.BigTimeOut;
		}

		private NextSession   	m_NextSession = new NextSession();
		//private NetCrC32    	CRC32 = new NetCrC32();
		public	MessagePool 	MsgPool;
		private int         	Seed;
		private uint 			buffindex = 0;
		private byte[]  		m_ZeroBytes = new byte[0];
		//private Dictionary< string , ProtocolType > protocol = new Dictionary< string , ProtocolType >();	// 协议;
		static private NetManager Instnace;
	}

}
