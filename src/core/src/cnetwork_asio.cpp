/*
 * cnetwork_asio.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#include <cstdlib>
#include <iostream>
#include <thread>
#include "logconsole.h"
#include "cnetwork_asio.h"

CNetwork_Asio::CNetwork_Asio( )
    : INetwork( ), m_Log( "CNetwork_ASIO" ), m_socket( m_io_service ), m_Listener( m_io_service ), BufCount( 0 ), m_Active( true )
{
	m_Log.oicprintf( CL_RESET CL_WHITE "Starting NetworkIO Thread...\n" CL_RESET );
	m_IOThread = std::thread( [this]( ) {
		asio::io_service::work _Work( m_io_service );
		m_io_service.run( );
	} );
	//	m_IOThread.detach();
	m_Log.oicprintf( CL_RESET CL_WHITE "Starting NetworkProcess Thread...\n" CL_RESET );
	m_ProcessThread = std::thread( [this]( ) {
		Run( );
	} );
	//	m_ProcessThread.detach();
}

CNetwork_Asio::~CNetwork_Asio( )
{
	Shutdown( );
	m_io_service.stop( );
	m_IOThread.join( );
	m_ProcessThread.join( );
}

bool CNetwork_Asio::Run( )
{
	//	std::cout << "run start\n";
	//	m_Active = true;
	//asio::io_service::work _Work( m_io_service );
	//std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	while ( m_Active == true )
	{
		ProcessSend( );
		ProcessRecv( );
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}

	m_Log.oicprintf( CL_RESET CL_WHITE "Network Prcess thread shutting down...\n" CL_RESET );
	//	std::cout << "run ending\n";
	//m_io_service.run( ); // this is to clean up
	return true;
}

bool CNetwork_Asio::Init( std::string _ip, uint16_t _port )
{
	//	std::cout << "runing init\n";
	if ( _ip.length( ) < 2 ) // We can actually use hostnames instead of IP addresses. Ex. google.com
		return false;

	m_IpAddress = _ip;
	m_wPort     = _port;
	return true;
}

bool CNetwork_Asio::Shutdown( )
{
	//if ( m_socket.is_open( ) )
	m_Active = false;
	Disconnect( );

	if ( m_Listener.is_open( ) )
		m_io_service.dispatch( [this]( ) {
			std::error_code ignored;
			m_Listener.close( ignored );
		} );

	while ( !m_SendQueue.empty( ) )
	{
		uint8_t* _buffer = std::move( m_SendQueue.front( ) );
		m_SendQueue.pop( );
		delete _buffer;
	}

	while ( !m_DiscardQueue.empty( ) )
	{
		uint8_t* _buffer = std::move( m_DiscardQueue.front( ) );
		m_DiscardQueue.pop( );
		delete _buffer;
	}
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
		                     }
		                 } );
	m_Active = true;
	return true;
}

bool CNetwork_Asio::Listen( )
{
	OnListen( );
	tcp::endpoint endpoint( tcp::v4( ), m_wPort );
	m_Listener.open( endpoint.protocol( ) );
	m_Listener.set_option( tcp::acceptor::reuse_address( true ) );
	m_Listener.bind( endpoint );
	m_Listener.listen( );
	m_Log.icprintf( "Listening started on %s:%i\n", GetIpAddress( ).c_str( ), GetPort( ) );
	m_Active = true;
	AcceptConnection( );
	OnListening( );
	return true;
}

bool CNetwork_Asio::Reconnect( )
{
	Disconnect( );
	Connect( );
	return true;
}

bool CNetwork_Asio::Disconnect( )
{
	OnDisconnect( );
	m_io_service.dispatch( [this]( ) {
		std::error_code ignored;
		m_socket.shutdown( asio::socket_base::shutdown_both, ignored );
		OnDisconnected( );
	} );
	return true;
}

bool CNetwork_Asio::Send( uint8_t* _buffer )
{
	OnSend( _buffer );
	std::lock_guard< std::mutex > lock( m_SendMutex );
	m_SendQueue.push( _buffer );
	return true;
}

bool CNetwork_Asio::Recv( uint16_t _size /*= 6*/ )
{
	OnReceive( );
	if ( m_RecvMutex.try_lock( ) )
	{
//		asio::error_code errorCode;
//		std::size_t length = asio::read( m_socket, asio::buffer( &Buffer[BufCount], _size ), asio::transfer_at_least( 6 ), errorCode );
//		BufCount += length;
//
//		if ( !errorCode || errorCode.value() == 11 )
//		{
//			m_Log.icprintf( "length = %i, BufCount = %i\n", length, BufCount );
//			OnReceived( Buffer, (uint16_t)BufCount );
//		}
//		else
//		{
//			//EC 2 = reached end of file (Nothing waiting to be read, this is okay)
//			if ( errorCode.value( ) != 2 )
//				m_Log.eicprintf( CL_RESET CL_WHITE "Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET, errorCode.value( ), errorCode.message( ).c_str( ) );
//		}
//		m_RecvMutex.unlock( );

 		asio::async_read( m_socket,
 		                  asio::buffer( &Buffer[ BufCount ], _size ),
 		                  asio::transfer_at_least( 6 ), // We want at least 6 bytes of data
 		                  [this]( std::error_code errorCode, std::size_t length ) {
 			                  BufCount += length;
 			                  if ( !errorCode || errorCode.value() == 11 )
 			                  {
 				                  m_Log.icprintf( "length = %i, BufCount = %i\n", length, BufCount );
 				                  OnReceived( Buffer, (uint16_t)BufCount );
 			                  }
 			                  else
 			                  {
 				                  //EC 2 = reached end of file (Nothing waiting to be read, this is okay)
 				                  if ( errorCode.value( ) != 2 )
 					                  m_Log.eicprintf( CL_RESET CL_WHITE "Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET, errorCode.value( ), errorCode.message( ).c_str( ) );
 			                  }
 			                  m_RecvMutex.unlock( );
 			                  Recv( );
 			              } );
	}
	return true;
}

void CNetwork_Asio::AcceptConnection( )
{
	m_Listener.async_accept(
	    [this]( std::error_code ec, tcp::socket socket ) {
		    if ( !ec )
		    {
			    if ( this->OnAccept( ) ) // This should be changed to use a client session instead of a CNetwork_Asio class
			    {
					socket.non_blocking( true );
				    // Do something here for the new connection.
				    // Make sure to use std::move(socket)
				    //std::make_shared<CClientSesson>( std::move(socket) );
				    this->OnAccepted( std::move( socket ) );
			    }
			    else
			    {
				    //Kill the socket
				    std::error_code ignored;
				    socket.close( ignored );
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

bool CNetwork_Asio::OnSend( uint8_t* _buffer )
{
	(void)_buffer;
	return true;
}

void CNetwork_Asio::OnSent( )
{
}

bool CNetwork_Asio::OnAccept( )
{
	return true;
}

void CNetwork_Asio::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
	}
}

void CNetwork_Asio::ProcessSend( )
{
	if ( m_socket.is_open( ) )
	{
		// Loop though all of m_SendQueue
		std::lock_guard< std::mutex > lock( m_SendMutex );

		if ( m_SendQueue.empty( ) )
			return;

		uint8_t* _buffer = std::move( m_SendQueue.front( ) );
		m_SendQueue.pop( );
		uint16_t _size = (uint16_t)_buffer[ 0 ];

		{
			std::lock_guard< std::mutex > lock2( m_DiscardMutex );
			m_DiscardQueue.push( _buffer );
		}

		asio::async_write( m_socket,
		                   asio::buffer( _buffer,
		                                 _size ),
		                   [this]( std::error_code ec, std::size_t /*length*/ ) {

			                   if ( !ec )
			                   {
				                   std::lock_guard< std::mutex > lock( m_DiscardMutex );
				                   uint8_t* _buffer = std::move( m_DiscardQueue.front( ) );
				                   m_DiscardQueue.pop( );

				                   delete _buffer;

				                   OnSent( );
			                   }
			                   else
			                   {
				                   m_Log.eicprintf( CL_RESET CL_WHITE "Error occurred[CNetwork_Asio::ProcessSend]: %s\n" CL_RESET, ec.message( ).c_str( ) );
				                   //			                   Disconnect( );
			                   }
			               } );
	}
}

void CNetwork_Asio::ProcessRecv( )
{
	// Loop though all of m_RecvQueue
	if ( m_socket.is_open( ) )
		Recv( );
}

bool CNetwork_Asio::HandlePacket( uint8_t* _buffer )
{
	(void)_buffer;
	return true;
}
