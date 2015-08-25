using System;
using System.Text;
using System.Collections.Generic;
using System.Net.Sockets;
using STR_LEN = System.Int32;
using ZhiWa;

/*
public enum ProtocolType
{
	UDP = 0 ,
	TCP = 1 ,
}
*/
namespace Net
{
	public class Message
	{
		public UInt32  ack;

		public Message()
		{
			_buffer = new byte[4096];
			_position = 0;
            _size = 0;
		
		}

        public Message( string type )
        {
			_timeout_queue = new Queue< int >();

			_timeout_queue.Enqueue( 0 );			// 0 second;
			_timeout_queue.Enqueue( 150  );			// 0.15 second;
			_timeout_queue.Enqueue( 500  );			// 0.5 second;
			_timeout_queue.Enqueue( 2 * 1000 );		// 2 second;
			_timeout_queue.Enqueue( 5 * 1000 );		// 5 second;

            _buffer = new byte[2048];
            setType(type);
        }
		
		public void init( byte[] bytes, int size )
		{
            int pos = 1 + 4;       //  skip Format field

            // Type Size
            const int SIZE = sizeof(STR_LEN);
            byte[] bytesStrLen = new byte[SIZE];
            Array.Copy(bytes, pos, bytesStrLen, 0, SIZE);
            STR_LEN strLen = BitConverter.ToInt32(bytesStrLen, 0);
            pos += SIZE;

            //	Type String
            byte[] bytesStr = new byte[strLen];
            Array.Copy(bytes, pos, bytesStr, 0, strLen);
            _type = Encoding.Default.GetString( bytesStr );
            pos += strLen;
            _size = size - pos;

			if ( _size > 0 ) {
            	Array.Copy(bytes, pos, _buffer, 0, _size);
			}

            _position = 0;
		}
		
		public void setType(string type)
		{
			_position = 0;
            _size = 0;
			_type = type;
            write((Int32)0);    //HeaderSize=4
			write((byte)1);
			write(type);
		}
		
		public string getType()
		{
			return _type;
		}

		public void write(byte val)
		{
			byte[] bytes = BitConverter.GetBytes(val);
			_buffer[_position] = bytes[0];
			++_position;
            ++_size;
		}

		public void write(Int16 val)
		{
			byte[] bytes = BitConverter.GetBytes(val);
			for( int i=0; i<sizeof(short); ++i )
			{
				_buffer[_position] = bytes[i];
				++_position;
                ++_size;
			}
		}
		
		public void write(Int32 val)
		{
			byte[] bytes = BitConverter.GetBytes(val);
            for (int i = 0; i < sizeof(int); ++i)
			{
				_buffer[_position] = bytes[i];
				++_position;
                ++_size;
			}
		}
		
		public void write(Int64 val)
		{
			byte[] bytes = BitConverter.GetBytes(val);
			for( int i=0; i<sizeof(long); ++i )
			{
				_buffer[_position] = bytes[i];
				++_position;
                ++_size;
			}
		}

        public void write(byte[] bytes)
        {
            write((STR_LEN)bytes.Length);
            addToBuffer(bytes, 0, bytes.Length);
        }

		public void write(string val)
		{
			write((STR_LEN)val.Length);
            addToBuffer(Encoding.Default.GetBytes(val), 0, val.Length);	//	Encoding.UTF8.GetBytes(s); Encoding.UTF8.GetString(t_data, 0, i);
		}

        public void writeUnicode(string val)
        {
            write((STR_LEN)val.Length);
            addToBuffer(Encoding.Unicode.GetBytes(val), 0, val.Length);	//	Encoding.UTF8.GetBytes(s); Encoding.UTF8.GetString(t_data, 0, i);
        }

        public byte ReadByte()
		{
			byte val = _buffer[_position];
			++_position;
            return val;
		}

        public Int16 ReadInt16()
		{
			const int SIZE = sizeof(Int16);
			byte[] bytes = new byte[SIZE];
            Array.Copy(_buffer, _position, bytes, 0, SIZE);
			Int16 val = BitConverter.ToInt16( bytes, 0 );
			_position += SIZE;
            return val;
		}
		
		public Int32 ReadInt32()
		{
			const int SIZE = sizeof(Int32);
			byte[] bytes = new byte[SIZE];
            Array.Copy(_buffer, _position, bytes, 0, SIZE);
			Int32 val = BitConverter.ToInt32( bytes, 0 );
			_position += SIZE;
            return val;
		}

        public Int64 ReadInt64()
		{
			const int SIZE = sizeof(Int64);
			byte[] bytes = new byte[SIZE];
            Array.Copy(_buffer, _position, bytes, 0, SIZE);
            Int64 val = BitConverter.ToInt64(bytes, 0);
			_position += SIZE;
            return val;
		}

        public byte[] ReadBytes()
        {
            Int32 size = ReadInt32();
            byte[] bytes = new byte[size];
            Array.Copy(_buffer, _position, bytes, 0, size);
            _position += size;
            return bytes;
        }

        public string ReadString()
		{
            Int32 size = ReadInt32();
			byte[] bytes = new byte[size];
            string str = Encoding.Default.GetString(_buffer, _position, size);
			_position += size;
            return str;
		}

        public void readUnicode(ref string val)
        {
            Int32 size = ReadInt32();
            val = Encoding.UTF8.GetString(_buffer, _position, size);
            _position += size;
        }
		
		public int getSize()
		{
			return _size;
		}
		
		public byte[] getBuffer()
		{
			return _buffer;
		}
		
		
		private void addToBuffer( byte[] bytes, int index, int size )
		{
            Array.Copy(bytes, index, _buffer, _position, size);
			_position += size;
			_size += size;
		}

		public uint MsgIndex
		{
			set{ _msgindex = value; }
			get{ return _msgindex  ;}
		}

		public int EnterMsgQueueTime
		{
			set{ _enter_msg_queue_time = value; }
			get{ return _enter_msg_queue_time ; }
		}

		public int GetMsgTimeout()
		{
			if( _timeout_queue.Count <= 0 ) {
				return 0;
			}

			return _timeout_queue.Peek();
		}

		public void RemoveMsgTimeOut()
		{
			if( _timeout_queue.Count <= 0 ) {
				return;
			}

			_timeout_queue.Dequeue();
		}

		public void ClearMsgTimeOut()
		{
			_timeout_queue.Clear();
		}

		public bool IsEmptyTimeOut()
		{
			if( _timeout_queue.Count > 0 ) {
				return false;
			}

			return true;
		}

		public byte SetUdpControlFlag( TUdpControlFlag proto_type, bool is_set )
		{
			if (is_set) {
				_udp_control_flag |= (byte)proto_type;
			}
			else {
				_udp_control_flag &= (byte)~(byte)proto_type;
			}
			return _udp_control_flag;
		}

		public byte GetUdpControlFlag()	{ return _udp_control_flag; }

		public void SetProtocol(ProtocolType proto) { _protocol = proto; }
		public ProtocolType GetProtocol()	{ return _protocol; }

		private int _position = 0;
		private int _size = 0;
		private string _type;
		private byte[] _buffer;
		private uint   _msgindex;
		private int   _enter_msg_queue_time;
		private Queue< int > _timeout_queue;
		private byte  _udp_control_flag = (byte)TUdpControlFlag.PROTO_NEED_ACK;
		private ProtocolType _protocol;

	}
}


	//System.Text.UnicodeEncoding converter = new System.Text.UnicodeEncoding(); 
	//byte[] inputBytes =converter.GetBytes(inputString); 
	//string inputString = converter.GetString(inputBytes); 
	//string inputString = System.Convert.ToBase64String(inputBytes); 
	//byte[] inputBytes = System.Convert.FromBase64String(inputString);

