#ifndef __CMAPISC_H__
#define __CMAPISC_H__

#include "croseisc.h"

class CMapISC : public CRoseISC {
 public:
  CMapISC();
  CMapISC(tcp::socket _sock);

  bool IsChar() const { return char_server_; }
  void SetChar(bool val) { char_server_ = val; }

 protected:
  bool HandlePacket(uint8_t* _buffer);
  virtual void OnConnected();
  bool char_server_;
};

#endif
