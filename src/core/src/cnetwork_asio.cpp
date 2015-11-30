/*
 * cnetwork_asio.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include "cnetwork_asio.h"

CNetwork_Asio::CNetwork_Asio( )
    : INetwork( ), m_socket( m_io_service ), m_Listener( m_io_service )
{
	m_IOThread = std::thread( [this]( ) {
		m_io_service.run( );
	} );
}

CNetwork_Asio::~CNetwork_Asio( )
{
	Shutdown( );
	m_io_service.stop( );
	m_IOThread.join();
}

bool CNetwork_Asio::Init( std::string _ip, uint16_t _port )
{
	if ( _ip.length( ) < 2 ) // We can actually use hostnames instead of IP addresses. Ex. google.com
		return false;

	m_IpAddress = _ip;
	m_wPort     = _port;
	return true;
}

bool CNetwork_Asio::Shutdown( )
{
	if ( m_socket.is_open( ) )
		Disconnect( );

	if ( m_Listener.is_open( ) )
		m_io_service.post( [this]()
	{
		m_Listener.cancel(); 
		m_Listener.close();
	} );
	return true;
}

bool CNetwork_Asio::Connect( )
{
	tcp::resolver resolver( m_io_service );
	auto endpoint_iterator = resolver.resolve( m_IpAddress, std::to_string( m_wPort ) );

	OnConnect( );
	asio::async_connect( m_socket, endpoint_iterator,
	                     [this]( std::error_code errorCode, const asio::ip::tcp::endpoint ) {
		                     if ( !errorCode )
		                     {
			                     OnConnected( );
			                     Recv( );
		                     }
		                 } );
	return true;
}

bool CNetwork_Asio::Listen( )
{
	OnListen( );
	tcp::endpoint endpoint( tcp::v4( ), m_wPort );
	m_Listener.open( tcp::v4( ) );
	m_Listener.bind( endpoint );
	m_Listener.listen( );
	AcceptConnection( );
	OnListening( );
	return true;
}

bool CNetwork_Asio::Reconnect( )
{
	return true;
}

bool CNetwork_Asio::Disconnect( )
{
	OnDisconnect();
	m_io_service.post( [this]( ) { m_socket.shutdown(asio::socket_base::shutdown_both); OnDisconnected(); } );
	return true;
}

bool CNetwork_Asio::Send( uint8_t* _buffer, uint16_t _size )
{
	OnSend(_buffer, _size);
	asio::async_write( m_socket,
	                   asio::buffer( _buffer,
	                                 _size ),
	                   [this]( std::error_code ec, std::size_t /*length*/ ) {
		                   if ( !ec )
		                   {
			                   OnSent( );
		                   }
		                   else
		                   {
			                   m_socket.close( );
		                   }
		               } );
	return true;
}

bool CNetwork_Asio::Recv( uint16_t _size /*= 6*/ )
{
	OnReceive( );
	asio::async_read( m_socket,
	                  asio::buffer( Buffer, _size ),
	                  [this]( std::error_code errorCode, std::size_t length ) {
		                  if ( !errorCode )
		                  {
					(void)length;
					Recv( (uint16_t)Buffer[ 0 ] );
					OnReceived( Buffer, (uint16_t)length );
		                  }
		                  else
		                  {
			                  m_socket.close( );
		                  }
		              } );
	OnReceived( Buffer, (uint16_t)Buffer[ 0 ] );
	return true;
}

void CNetwork_Asio::AcceptConnection( )
{
	m_Listener.async_accept(
	    [this]( std::error_code ec, tcp::socket socket ) {
		    if ( !ec )
		    {
			if ( this->OnAccept(std::move(socket)) ) // This should be changed to use a client session instead of a CNetwork_Asio class
			{
			    // Do something here for the new connection.
			    // Make sure to use std::move(socket)
			    //std::make_shared<CClientSesson>( std::move(socket) );
				//this->OnAccepted();
			}
			else
			{
				//Kill the socket
				socket.close( );
			}
		    }

		    AcceptConnection( );
		} );
}

bool CNetwork_Asio::OnConnect( )
{
	return true;
}

void CNetwork_Asio::OnConnected( )
{
}

bool CNetwork_Asio::OnListen( )
{
	return true;
}

void CNetwork_Asio::OnListening( )
{
}

bool CNetwork_Asio::OnDisconnect( )
{
	return true;
}

void CNetwork_Asio::OnDisconnected( )
{
}

bool CNetwork_Asio::OnReceive( )
{
	return true;
}

void CNetwork_Asio::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
}

bool CNetwork_Asio::OnSend( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
	return true;
}

void CNetwork_Asio::OnSent( )
{
}

bool CNetwork_Asio::OnAccept(tcp::socket _sock)
{
	if( _sock.is_open() )
	{
		return true;
	}
	return false;
}

void CNetwork_Asio::OnAccepted(tcp::socket _sock)
{
	if( _sock.is_open() )
	{
		//Do Something?
	}
}
