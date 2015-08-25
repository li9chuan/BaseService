using UnityEngine;
using System;
using System.Net;
using System.Threading;
using System.Collections;
using System.Net.Sockets;
using System.Collections.Generic;
using MSG_LEN = System.Int32;
namespace Net
{
	public class TcpClient 
	{
		public enum SocketState
		{
			SocketNull    = 0,
			SocketConnect = 1,
			SocketSend    = 2,
			SocketRecv    = 3,
		}

		public static TcpClient GetInstance()
		{
			if( null == Instance )
			{
				Instance = new TcpClient();
			}
			return Instance;
		}

		TcpClient()
		{
			LockObj		= new object();
			ErrorLock   = new object ();
			ErrorQueue	= new LinkedQueue< SocketError >();
			ErrorString = new LinkedQueue< string > ();
			SendBuff 	= new byte[1024];
			RecvBuff 	= new byte[4096];
			// 启动线程;
			//StartThreads ();
		}

		public void OnUpdate()
		{
			if ( !ErrorQueue.IsEmpty )
			{
				SocketError ErrID = SocketError.Success;
				ErrorQueue.pop( out ErrID );

				switch( ErrID )
				{
				case SocketError.SocketError:
				case SocketError.ConnectionReset:
				case SocketError.TimedOut:
				case SocketError.ConnectionRefused:
					Close();
					break;
				default:
					break;
				}
			}

			if ( !ErrorString.IsEmpty )
			{
				string error = "";
				ErrorString.pop( out error );
				if ( string.IsNullOrEmpty( error ) ) {
					return;
				}

				Notification notify = new Notification(NotificationNames.REFRESH_CONNECT_LOG, this , error );
				notify.Send();
			}
		}

		public void SendToServer( string ip , int port , Message msg )
		{
			IPAddress serverip;

			if( !IPAddress.TryParse( ip , out serverip ) )
			{
				IPAddress[] array = Dns.GetHostAddresses( ip );
				serverip = array[ 0 ];
			}

			Close();

			ServerHost 	= new IPEndPoint( serverip , port );
			TcpSocket  	= new Socket( AddressFamily.InterNetwork , SocketType.Stream , ProtocolType.Tcp );
			SendMsg		= msg;

			// 状态;
			SetSocketState( SocketState.SocketConnect );

			// 打开菊花;
			UIManager.GetInstance().OpenUI( EnumUIType.MtConnectingGUI );

			ErrorString.push( "tcp SendToServer \n" + msg.getType() );
		}

		public void ProcessMsg( Message msg )
		{
			if( null == msg ){
			return;
			}

			if( msg.getType() == SendMsg.getType() ) {
				UIManager.GetInstance().CloseUI( EnumUIType.MtConnectingGUI );
			}
		}

		public void ClearData()
		{
			Close();
		}

		private void StartThreads()
		{
			try {
				IsRunThreads = true;
				TcpThread = new Thread( new ThreadStart(  RunMsg ) );
				TcpThread.IsBackground = true;
				TcpThread.Start();
				ErrorString.push( "tcp StartThreads \n" );
			}
			catch{

			}
		}

		public void CloseThreads()
		{
			IsRunThreads = false;
			ClearError ();
			Close();
		}

		private void Close()
		{
			SetSocketState( SocketState.SocketNull );
			UIManager.GetInstance().CloseUI( EnumUIType.MtConnectingGUI );

			try {
				if( null != TcpSocket && TcpSocket.Connected ){
				 TcpSocket.Shutdown( SocketShutdown.Both );
				 TcpSocket.Close();
				 TcpSocket = null;
				}
			}
			catch (System.Exception what ){
				Debug.Log( " close socket " + what.ToString() );
			}
		}

		private void Connect()
		{
			if( null == TcpSocket ){
			return;
			}

			try {
				// 日志;
				ErrorString.push( "tcp Start Connect ing \n" + ServerHost.ToString() );
				// 连接;
				TcpSocket.Connect( ServerHost );
				// 状态;
				SetSocketState( SocketState.SocketSend );
				// 日志;
				ErrorString.push( "tcp End Connect ing \n" );
				return;
			}
			catch( System.ComponentModel.Win32Exception what ) {
				ErrorQueue.push( ( SocketError )what.NativeErrorCode );
				Debug.Log( " socket connect " + what.ToString() );
				ErrorString.push( "tcp Connect faile \n" + what.ToString() );
			}
			catch( System.Exception what ) {

				Debug.Log( " socket connect " + what.ToString() );
			}

			// 状态;
			SetSocketState( SocketState.SocketNull );
		}

		private void Send()
		{
			try{
				// 发送;
				byte[] bytes = BitConverter.GetBytes( ( MSG_LEN )SendMsg.getSize() );
				int	   buffsize = 0;
				
				SendBuff[ 0 ] = bytes[ 3 ];
				SendBuff[ 1 ] = bytes[ 2 ];
				SendBuff[ 2 ] = bytes[ 1 ];
				SendBuff[ 3 ] = bytes[ 0 ];
				
				Array.Copy( SendMsg.getBuffer() , 0  , SendBuff , sizeof( MSG_LEN ) , SendMsg.getSize() );
				buffsize = sizeof( MSG_LEN ) + SendMsg.getSize();

				ErrorString.push( "tcp send \n" + SendMsg.getType() );
			
				int SendSize = TcpSocket.Send( SendBuff , 0 , buffsize , SocketFlags.None );
				
				Debug.Log( " Tcp send size " + SendSize );
				
				// 状态;
				SetSocketState( SocketState.SocketRecv );

				return;
			}
			catch( System.ComponentModel.Win32Exception what ) {

				ErrorQueue.push( ( SocketError )what.NativeErrorCode );
				Debug.Log( " socket send " + what.ToString() );
			}
			catch( System.Exception what ) {

				Debug.Log( " socket send " + what.ToString() );
			}

			// 状态;
			SetSocketState( SocketState.SocketNull );
		}

		private void Recv()
		{
			try{
				// 发送;
				if( TcpSocket.Available > 0 )
				{
					int RecvSize = TcpSocket.Receive( RecvBuff );
					if( RecvSize > 0 ) {
					ErrorString.push( "tcp recv pack \n" + RecvSize );
					NetManager.GetInstance().MsgPool.parseBuffer( RecvBuff , RecvSize, ProtocolType.Tcp );
					Debug.Log( " Tcp Recv Package Size  " + RecvSize );
					}
				}
				return;
			}
			catch( System.ComponentModel.Win32Exception what ) {

				ErrorQueue.push( ( SocketError )what.NativeErrorCode );
				Debug.Log( " socket recv " + what.ToString() );
			}
			catch ( System.Exception what ) {

				Debug.Log( " socket recv " + what.ToString() );
			}

			// 状态;
			SetSocketState( SocketState.SocketNull );
		}

		private void RunMsg()
		{
			while( IsRunThreads )
			{
				try
				{
				if( IsStateNull() ) {
						//PushErrorStrint( "tcp IsStateNull() \n" );
					Thread.Sleep( 100 );
				}
				if( IsStateConnect() ){
						ErrorString.push( "tcp IsStateConnect() \n" );
					Connect();
				}
				if( IsStateSend() ){
						ErrorString.push( "tcp IsStateSend() \n" );
					Send();
				}
				if( IsStateRecv() ){
						//PushErrorStrint( "tcp IsStateRecv() \n" );
					Recv();
				}
				}
				catch( System.Exception what )
				{
					ErrorString.push( "tcp threads Exception  " + what.ToString() + "\n" );
				}
			}
		}

		private void SetSocketState( SocketState state ) {
			State = state;
		}
		private SocketState GetSocketState() {
			return State;
		}

		private bool IsStateNull() {
			return ( State == SocketState.SocketNull );
		}
		private bool IsStateConnect() {
			return ( State == SocketState.SocketConnect );
		}
		private bool IsStateSend() {
			return ( State == SocketState.SocketSend );
		}
		private bool IsStateRecv() {
			return ( State == SocketState.SocketRecv );
		}

		private void ClearError()
		{
			while ( !ErrorString.IsEmpty )
			{
				string error = "";
				ErrorString.pop( out error );
			}

			while ( !ErrorString.IsEmpty ) 
			{
				SocketError error = SocketError.Success;
				ErrorQueue.pop( out error );
			}
		}

		private static object 	 	 	   LockObj;
		private static object 		 	   ErrorLock;
		private LinkedQueue< SocketError > ErrorQueue;
		private LinkedQueue< string >      ErrorString;
		private byte[]			 	 SendBuff;
		private byte[]			 	 RecvBuff;
		private Message			 	 SendMsg;
		private volatile bool	 	 IsRunThreads;
		private SocketState		 	 State;
		private IPEndPoint 		 	 ServerHost;
		private Thread				 TcpThread;
		private volatile Socket 	 TcpSocket;
		private static TcpClient 	 Instance;
	}

}
