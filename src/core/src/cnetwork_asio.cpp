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
    : INetwork( ), log_( "CNetwork_ASIO" ), io_work_( new asio_worker::element_type(io_service_) ), socket_( io_service_ ), listener_( io_service_ ), packet_offset_( 0 ), packet_size_( 6 ), active_( true )
{
	log_.oicprintf( CL_RESET CL_WHITE "Starting NetworkIO Thread...\n" CL_RESET );
	io_thread_ = std::thread( [this]( ) {
		io_service_.run( );
	} );
	//	m_IOThread.detach();
	log_.oicprintf( CL_RESET CL_WHITE "Starting NetworkProcess Thread...\n" CL_RESET );
	process_thread_ = std::thread( [this]( ) {
		Run( );
	} );
	//	m_ProcessThread.detach();
}

CNetwork_Asio::~CNetwork_Asio( )
{
	//	std::lock_guard< std::mutex > lock( m_SendMutex );
	//	std::lock_guard< std::mutex > lock2( m_DiscardMutex );

	io_work_.reset();
	Shutdown( );
	io_service_.stop( );
	io_thread_.join( );
	process_thread_.join( );
//	m_io_service.stop( );
}

bool CNetwork_Asio::Run( )
{
	//	std::cout << "run start\n";
	//	m_Active = true;
	//asio::io_service::work _Work( m_io_service );
	//std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	while ( active_ == true )
	{
		ProcessSend( );
		ProcessRecv( );
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
	}

	log_.oicprintf( CL_RESET CL_WHITE "Network Process thread shutting down...\n" CL_RESET );
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
	std::lock_guard< std::mutex > lock( send_mutex_ );
	std::lock_guard< std::mutex > lock2( discard_mutex_ );
	//if ( m_socket.is_open( ) )
	active_ = false;
	Disconnect( );

	//m_io_service.post( m_Listener.FinishConnection());

	if ( listener_.is_open( ) )
	{
	//m_Log.icprintf( CL_RESET CL_WHITE "Listener should be closing...\n" CL_RESET );
		io_service_.post( [this]( ) {
			std::error_code ignored;
//			m_Listener.cancel( ignored );
			listener_.close( ignored );
		} );

	//m_Log.icprintf( CL_RESET CL_WHITE "Listener should be closed...\n" CL_RESET );
	}

	//	std::lock_guard< std::mutex > lock( m_SendMutex );
	//	std::lock_guard< std::mutex > lock2( m_DiscardMutex );
	while ( !send_queue_.empty( ) )
	{
		uint8_t* _buffer = std::move( send_queue_.front( ) );
		send_queue_.pop( );
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
	tcp::resolver resolver( io_service_ );
	auto endpoint_iterator = resolver.resolve( m_IpAddress, std::to_string( m_wPort ) );

	OnConnect( );
	asio::async_connect( socket_, endpoint_iterator,
	                     [this]( std::error_code errorCode, const asio::ip::tcp::endpoint ) {
		                     if ( !errorCode )
		                     {
			                     OnConnected( );
		                     }
		                 } );
	active_ = true;
	return true;
}

bool CNetwork_Asio::Listen( )
{
	OnListen( );
	tcp::endpoint endpoint( tcp::v4( ), m_wPort );
	listener_.open( endpoint.protocol( ) );
	listener_.set_option( tcp::acceptor::reuse_address( true ) );
	listener_.bind( endpoint );
	listener_.listen( );
	log_.icprintf( "Listening started on %s:%i\n", GetIpAddress( ).c_str( ), GetPort( ) );
	active_ = true;
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
	io_service_.dispatch( [this]( ) {
		std::error_code ignored;
		socket_.shutdown( asio::socket_base::shutdown_both, ignored );
		OnDisconnected( );
	} );
	return true;
}

bool CNetwork_Asio::Send( uint8_t* _buffer )
{
	std::lock_guard< std::mutex > lock( send_mutex_ );
	send_queue_.push( _buffer );
	return true;
}

bool CNetwork_Asio::Recv( uint16_t _size /*= 6*/ )
{
	OnReceive( );
	{
		(void)_size;

		uint8_t BytesToRead = packet_size_ - packet_offset_;
		asio::async_read( socket_,
		                  asio::buffer( &buffer_[ packet_offset_ ], BytesToRead ),
		                  asio::transfer_exactly( BytesToRead ), // We want at least 6 bytes of data
		                  [this]( std::error_code errorCode, std::size_t length ) {
			                  packet_offset_ += length;
			                  if ( !errorCode || errorCode.value( ) == 11 )
			                  {
				                  if ( OnReceived( ) == false )
						  {
							log_.eicprintf(CL_RESET "Something bad happend in OnReceived... Shutting down...\n" CL_RESET);
							Shutdown();
						  }
			                  }
			                  else
			                  {
						if( errorCode.value( ) == 2 )
						{
							log_.icprintf( CL_RESET CL_WHITE "Client disconnected.\n" );
							OnDisconnected();
						}
						else
				                	log_.eicprintf( CL_RESET CL_WHITE "Error occurred[CNetwork_Asio::Recv:%i]: %s\n" CL_RESET, errorCode.value( ), errorCode.message( ).c_str( ) );

						Shutdown();
						return;
			                  }

			                  recv_condition_.notify_all( );
			                  if ( active_ )
				                  Recv( );
			              } );
	}
	return true;
}

void CNetwork_Asio::AcceptConnection( )
{
	listener_.async_accept(
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
	if ( !listener_.is_open( ) )
		Recv( );
}

bool CNetwork_Asio::OnListen( )
{
	return true;
}

void CNetwork_Asio::OnListening( )
{
	if ( !listener_.is_open( ) )
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
	if ( socket_.is_open( ) )
	{
		// Loop though all of m_SendQueue
		std::lock_guard< std::mutex > lock( send_mutex_ );

//		if ( m_SendQueue.empty( ) )
//			return;

		while ( !send_queue_.empty( ) )
		{
			uint8_t* _buffer = std::move( send_queue_.front( ) );
			send_queue_.pop( );
			uint16_t _size = (uint16_t)_buffer[ 0 ];
			uint16_t _command = (uint16_t)_buffer[ 2 ];

			if( OnSend( _buffer ) )
				asio::write( socket_, asio::buffer( _buffer, _size ) );
			else
				log_.eicprintf( CL_RESET "Not sending packet: Header[%i, 0x%X]\n", _size, _command );

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
