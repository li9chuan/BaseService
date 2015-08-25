using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;

namespace Net
{
	public class LinkedQueue < T >
	{
		private class node< K >
		{
			internal K 	  	 item;
			internal node<K> next;
			
			public node( K _item , node< K > _next )
			{
				this.item = _item;
				this.next = _next;
			}
		}
		
		private node< T > _head;
		private node< T > _tail;
		
		
		public LinkedQueue()
		{
			_head = new node< T >( default( T ) , null );
			_tail = _head;
		}
		
		public bool IsEmpty
		{
			get  { return ( _head.next == null ); }
		}
		
		public void push( T item )
		{
			node< T > newNode = new node<T>( item , null );
			
			while ( true )
			{
				node< T > curTail = _tail;
				node< T > residue = curTail.next;
				
				if ( curTail == _tail )
				{
					if ( residue == null )
					{
						if ( Interlocked.CompareExchange<node<T>>( 
						     ref curTail.next , newNode , residue ) == residue )
						{
							Interlocked.CompareExchange<node<T>>(
								ref _tail , newNode , curTail );
							return;
						}
					}
				}
				else
				{
					Interlocked.CompareExchange<node<T>>( ref _tail , residue , curTail );
				}
			}
		}
		
		public bool pop( out T result )
		{
			node< T > curHead;
			node< T > curTail;
			node< T > next;
			
			do
			{
				curHead = _head;
				curTail = _tail;
				next    = curHead.next;
				
				if ( curHead == _head )
				{
					if ( next == null )
					{
						result = default( T );
						return false;
					}
					if ( curHead == curTail )
					{
						Interlocked.CompareExchange< node< T > >( ref _tail , next , curTail );
					}
					else
					{
						result = next.item;
						if( Interlocked.CompareExchange<node<T>>(
							ref _head , next , curHead ) == curHead ){
							break;
						}
					}
				}
			}
			while( true );
			return true;
		}
	}
}


