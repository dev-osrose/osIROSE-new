/*
 * cnetwork_asio.h
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#ifndef _CNETWORK_ASIO_H_
#define _CNETWORK_ASIO_H_

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 6011 6031 6102 6255 6258 6326 6387 )
#define _WIN32_WINNT 0x0601
#endif

#include <asio.hpp>
#include <queue>
#include <mutex>

#ifdef _WIN32
#pragma warning( pop )
#endif

#include <thread>
#include "inetwork.h"
#include "logconsole.h"
#include <condition_variable>

#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE 0x7FF
#endif

using asio::ip::tcp;
class CLogConsole;

class CNetwork_Asio : public INetwork
{
public:
	CNetwork_Asio( );
	virtual ~CNetwork_Asio( );

	virtual bool Init( std::string _ip, uint16_t _port );
	virtual bool Shutdown( );

	virtual bool Connect( );
	virtual bool Listen( );
	virtual bool Run( );
	virtual bool Reconnect( );
	virtual bool Disconnect( );

	virtual bool Send( uint8_t* _buffer );
	virtual bool Recv( uint16_t _size = MAX_PACKET_SIZE );
	bool         IsActive() { return m_Active; }
	void         SetExtraMessageInfo( bool _enabled ) { m_Log.SetDisplayOmittable( _enabled ); }
protected:
	void AcceptConnection( );
	void ProcessSend( );
	void ProcessRecv( );

	// Callback functions
	virtual bool OnConnect( );
	virtual void OnConnected( );
	virtual bool OnListen( );
	virtual void OnListening( );
	virtual bool OnDisconnect( );
	virtual void OnDisconnected( );
	virtual bool OnReceive( );
	virtual bool OnReceived( );
	virtual bool OnSend( uint8_t* _buffer );
	virtual void OnSent( );
	virtual bool OnAccept( );
	virtual void OnAccepted( tcp::socket _sock );
	virtual bool HandlePacket( uint8_t* _buffer );

	void SetSocket(tcp::socket _sock) { m_socket = std::move(_sock); }
	void ResetBuffer() 
	{ 
		PacketOffset = 0; 
		PacketSize = 6;
	}
	CLogConsole m_Log;

protected:
	//std::unique_ptr<asio::io_service::work> m_Work;
	asio::io_service		m_io_service;
	tcp::socket				m_socket;
	tcp::acceptor			m_Listener;
	std::queue< uint8_t* >	m_SendQueue;
	std::queue< uint8_t* >	m_DiscardQueue;
	std::mutex				m_SendMutex;
	std::mutex				m_RecvMutex;
	std::mutex				m_DiscardMutex;
	std::condition_variable m_RecvCondition;

	std::thread m_IOThread;
	std::thread m_ProcessThread;
	uint8_t     Buffer[ MAX_PACKET_SIZE ];
	uint16_t    PacketOffset;
	uint16_t	PacketSize;
	bool        m_Active;
};

#endif
