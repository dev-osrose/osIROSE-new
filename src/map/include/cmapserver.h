#ifndef _CMAPSERVER_H_
#define _CMAPSERVER_H_

#include "croseserver.h"

class CMapServer : public CRoseServer
{
public:
        CMapServer( bool _isc = false );
        virtual ~CMapServer();

protected:
        virtual void OnAccepted( tcp::socket _sock );
};

#endif
