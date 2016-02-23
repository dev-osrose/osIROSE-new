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
    : INetwork( ), m_Log( "CNetwork_ASIO" ), m_Work( new asio_worker::element_type(m_io_service) ), m_socket( m_io_service ), m_Listener( m_io_service ), PacketOffset( 0 ), PacketSize( 6 ), m_Active( true )
{
	m_Log.oicprintf( CL_RESET CL_WHITE "Starting NetworkIO Thread...\n" CL_RESET );
	m_IOThread = std::thread( [this]( ) {
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
	//	std::lock_guard< std::mutex > lock( m_SendMutex );
	//	std::lock_guard< std::mutex > lock2( m_DiscardMutex );

	m_Work.reset();
	Shutdown( );
	m_io_service.stop( );
	m_IOThread.join( );
	m_ProcessThread.join( );
//	m_io_service.stop( );
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
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
	}

	m_Log.oicprintf( CL_RESET CL_WHITE "Network Process thread shutting down...\n" CL_RESET );
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
	std::lock_guard< std::mutex > lock( m_SendMutex );
	std::lock_guard< std::mutex > lock2( m_DiscardMutex );
	//if ( m_socket.is_open( ) )
	m_Active = false;
	Disconnect( );

	//m_io_service.post( m_Listener.FinishConnection());

	if ( m_Listener.is_open( ) )
	{
	//m_Log.icprintf( CL_RESET CL_WHITE "Listener should be closing...\n" CL_RESET );
		m_io_service.post( [this]( ) {
			std::error_code ignored;
//			m_Listener.cancel( ignored );
			m_Listener.close( ignored );
		} );

	//m_Log.icprintf( CL_RESET CL_WHITE "Listener should be closed...\n" CL_RESET );
	}

	//	std::lock_guard< std::mutex > lock( m_SendMutex );
	//	std::lock_guard< std::mutex > lock2( m_DiscardMutex );
	while ( !m_SendQueue.empty( ) )
	{
		uint8_t* _buffer = std::move( m_SendQueue.front( ) );
		m_SendQueue.pop( );
		delete _buffer;
	}

//	while ( !m_DiscardQueue.empty( ) )
//	{
//		uint8_t* _buffer = std::move( m_DiscardQueue.front( ) );
//		m_DiscardQueue.pop( );
//		delete _buffer;
//	}

//	m_io_service.stop( );
	//        m_IOThread.join( );
	//        m_ProcessThread.join( );
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
	std::lock_guard< std::mutex > lock( m_SendMutex );
	m_SendQueue.push( _buffer );
	return true;
}

bool CNetwork_Asio::Recv( uint16_t _size /*= 6*/ )
{
	OnReceive( );
	{
		(void)_size;

		uint8_t BytesToRead = PacketSize - PacketOffset;
		asio::async_read( m_socket,
		                  asio::buffer( &Buffer[ PacketOffset ], BytesToRead ),
		                  asio::transfer_exactly( BytesToRead ), // We want at least 6 bytes of data
		                  [this]( std::error_code errorCode, std::size_t length ) {
			                  PacketOffset += length;
			                  if ( !errorCode || errorCode.value( ) == 11 )
			                  {
				                  if ( OnReceived( ) == false )
						  {
							m_Log.eicprintf(CL_RESET "Something bad happend in OnReceived... Shutting down...\n" CL_RESET);
							Shutdown();
						  }
			                  }
			                  else
			                  {
						if( errorCode.value( ) == 2 )
						{
							m_Log.icprintf( CL_RESET CL_WHITE "Client disconnected.\n" );
							OnDisconnected();
						}
						else
				                	m_Log.eicprintf( CL_RESET CL_WHITE "Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET, errorCode.value( ), errorCode.message( ).c_str( ) );

						Shutdown();
						return;
			                  }

			                  m_RecvCondition.notify_all( );
			                  if ( m_Active )
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
				    //socket.non_blocking( true );
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
	if ( !m_Listener.is_open( ) )
		Recv( );
}

bool CNetwork_Asio::OnListen( )
{
	return true;
}

void CNetwork_Asio::OnListening( )
{
	if ( !m_Listener.is_open( ) )
		Recv( );
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

bool CNetwork_Asio::OnReceived( )
{
	return true;
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

//		if ( m_SendQueue.empty( ) )
//			return;

		while ( !m_SendQueue.empty( ) )
		{
			uint8_t* _buffer = std::move( m_SendQueue.front( ) );
			m_SendQueue.pop( );
			uint16_t _size = (uint16_t)_buffer[ 0 ];
			uint16_t _command = (uint16_t)_buffer[ 2 ];

			if( OnSend( _buffer ) )
				asio::write( m_socket, asio::buffer( _buffer, _size ) );
			else
				m_Log.eicprintf( CL_RESET "Not sending packet: Header[%i, 0x%X]\n", _size, _command );

//			m_Log.oicprintf( "ProcessSend Sent a packet on CRoseISC: Header[%i, 0x%X]\n", _size, _command );
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
			delete _buffer;
			_buffer = nullptr;
			OnSent();
		}
	}
}

void CNetwork_Asio::ProcessRecv( )
{
	// Loop though all of m_RecvQueue
	// 	if ( m_socket.is_open( ) )
	// 		Recv( );
}

bool CNetwork_Asio::HandlePacket( uint8_t* _buffer )
{
	(void)_buffer;
	return true;
}
