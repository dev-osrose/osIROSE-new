/*
 * cnetwork_asio.h
 *
 *  Created on: Nov 23, 2015
 *      Author: ctorres
 */

#include <asio.hpp>
#include "inetwork.h"

#define MAX_PACKET_SIZE 0x7FF

using asio::ip::tcp;

class CNetwork_Asio : public INetwork
{
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

	// Callback functions
	virtual bool OnConnect();
	virtual bool OnConnected();
	virtual bool OnListen();
	virtual bool OnListening();
	virtual bool OnDisconnect();
	virtual bool OnDisconnected();
	virtual bool OnReceive();
	virtual bool OnReceived(uint8_t* _buffer, uint16_t _size);
	virtual bool OnSend(uint8_t* _buffer, uint16_t _size);
	virtual bool OnSent();


private:
	asio::io_service m_io_service;
	tcp::socket m_socket;

	std::thread m_IOThread;
	uint8_t Buffer[MAX_PACKET_SIZE];
};
