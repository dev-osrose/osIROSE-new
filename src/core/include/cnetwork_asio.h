/*
 * cnetwork_asio.h
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#ifdef _WIN32
#pragma warning(push)
#pragma warning( disable: 6011 6031 6102 6255 6258 6326 6387  )
#define _WIN32_WINNT 0x0601
#endif

#include <asio.hpp>

#ifdef _WIN32
#pragma warning(pop)
#endif

#include <thread>
#include "inetwork.h"

#define MAX_PACKET_SIZE 0x7FF

using asio::ip::tcp;

class CNetwork_Asio : public INetwork
{
public:
	CNetwork_Asio();
	virtual ~CNetwork_Asio();

	virtual bool Init(std::string _ip, uint16_t _port);
	virtual bool Shutdown();

	virtual bool Connect();
	virtual bool Listen();
	virtual bool Reconnect();
	virtual bool Disconnect();

protected:
	virtual bool Send(uint8_t* _buffer, uint16_t _size);
	virtual bool Recv(uint16_t _size = 6);
	void AcceptConnection();

	// Callback functions
	virtual bool OnConnect();
	virtual void OnConnected();
	virtual bool OnListen();
	virtual void OnListening();
	virtual bool OnDisconnect();
	virtual void OnDisconnected();
	virtual bool OnReceive();
	virtual void OnReceived(uint8_t* _buffer, uint16_t _size);
	virtual bool OnSend(uint8_t* _buffer, uint16_t _size);
	virtual void OnSent();
	virtual bool OnAccept(tcp::socket _sock);
	virtual void OnAccepted(tcp::socket _sock);


private:
	asio::io_service m_io_service;
	tcp::socket m_socket;
	tcp::acceptor m_Listener;

	std::thread m_IOThread;
	uint8_t Buffer[MAX_PACKET_SIZE];
};
