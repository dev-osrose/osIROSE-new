#ifndef _CLOGINSERVER_H_
#define _CLOGINSERVER_H_

#include "croseserver.h"
#include "cmysql_database.h"

class CLoginServer : public RoseCommon::CRoseServer {
 public:
  CLoginServer(bool _isc = false);
  virtual ~CLoginServer();

  uint32_t GetClientCount() {
    return (client_list_.empty() ? 0 : std::distance(std::begin(client_list_),
                                                     std::end(client_list_)));
  }

 protected:
  virtual void OnAccepted(tcp::socket _sock);
  std::unique_ptr<Core::CMySQL_Database>	database_;
  uint32_t client_count_;
};

#endif
