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

CNetwork_Asio::CNetwork_Asio () : INetwork(), m_IOThread( [&m_io_service](){ m_io_service.run(); } )
{
}

CNetwork_Asio::~CNetwork_Asio ()
{
}

bool
CNetwork_Asio::Init ( std::string _ip, uint16_t _port )
{
	m_IpAddress = _ip;
	m_wPort = _port;
	return true;
}

bool
CNetwork_Asio::Shutdown ()
{
	return true;
}

bool
CNetwork_Asio::Connect ()
{
	tcp::resolver resolver(m_io_service);
	auto endpoint_iterator = resolver.resolve({ m_IpAddress, m_wPort });

	OnConnect();
	asio::async_connect(m_socket, endpoint_iterator,
		[this](std::error_code errorCode, tcp::resolver::iterator)
		{
			if (!errorCode)
			{
				OnConnected();
				Recv();
			}
		});
	return true;
}

bool
CNetwork_Asio::Listen ()
{
	return true;
}

bool
CNetwork_Asio::Reconnect ()
{
	return true;
}

bool
CNetwork_Asio::Disconnect ()
{
	m_io_service.post( [this]() { m_socket.close(); } );
	return true;
}

bool
CNetwork_Asio::Send ( uint8_t* _buffer, uint16_t _size )
{
	asio::async_write(m_socket,
	        asio::buffer(_buffer,
	          _size),
	        [this](std::error_code ec, std::size_t /*length*/)
	        {
	          if (!ec)
	          {
	          	OnSent();
	          }
	          else
	          {
	            m_socket.close();
	          }
	        });
	return true;
}

bool
CNetwork_Asio::Recv (uint16_t _size /*= 6*/)
{
	OnReceive();
	asio::async_read(m_socket,
	        asio::buffer(Buffer, _size),
	        [this](std::error_code errorCode, std::size_t length)
	        {
	          if (!errorCode)
	          {
	          	Recv( (uint16_t)Buffer[0] );
	          }
	          else
	          {
	            m_socket.close();
	          }
	        });
	OnReceived(Buffer, (uint16_t)Buffer[0]);
	return true;
}

bool
CNetwork_Asio::OnConnect ()
{
	return true;
}

bool
CNetwork_Asio::OnConnected ()
{
	return true;
}

bool
CNetwork_Asio::OnListen ()
{
	return true;
}

bool
CNetwork_Asio::OnListening ()
{
	return true;
}

bool
CNetwork_Asio::OnDisconnect ()
{
	return true;
}

bool
CNetwork_Asio::OnDisconnected ()
{
	return true;
}

bool
CNetwork_Asio::OnReceive ()
{
	return true;
}

bool
CNetwork_Asio::OnReceived ( uint8_t* _buffer, uint16_t _size )
{
	return true;
}

bool
CNetwork_Asio::OnSend ( uint8_t* _buffer, uint16_t _size )
{
	// Crypt packet here
	return true;
}

bool
CNetwork_Asio::OnSent ()
{
	return true;
}
