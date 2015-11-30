#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include "cnetwork_asio.h"

class CRose_Network : public CNetwork_Asio
{
public:
	CRose_Network();
	virtual ~CRose_Network();

private:
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
};

#endif
