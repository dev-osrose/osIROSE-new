#ifndef _CLOGINISC_H_
#define _CLOGINISC_H_

#include <forward_list>
#include "croseisc.h"
#include "crosepacket.h"

struct tChannelInfo;

class CLoginISC : public CRoseISC {
 public:
  CLoginISC();
  CLoginISC(tcp::socket _sock);

  std::string GetName() { return server_name_; }
  std::string GetIP() { return network_ip_address; }
  uint16_t GetPort() { return network_port_; }
  uint8_t GetServerType() { return network_type_; }
  bool IsTestServer() { return test_server_; }
  std::forward_list<tChannelInfo> GetChannelList() const {
    return channel_list_;
  }

 protected:
  bool HandlePacket(uint8_t* _buffer);
  bool ServerRegister(CRosePacket* P);
  bool ServerShutdown(CRosePacket* P);

  std::string server_name_;
  uint32_t channel_count_;
  uint32_t min_right_;
  bool test_server_;

  std::forward_list<tChannelInfo> channel_list_;
};

#endif
