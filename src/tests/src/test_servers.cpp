#include "gtest/gtest.h"

#include <stdint.h>
#include "cnetwork_asio.h"
#include "iscpackets.pb.h"
#include "epackettype.h"
#include "crosepacket.h"
#include "cloginserver.h"
#include "ccharserver.h"
#include "cmapserver.h"

#include "ccharisc.h"
#include "cmapisc.h"

using namespace RoseCommon;

TEST(TestFinalServers, TestISCConnections) {
  CLoginServer loginIsc(true);
  CCharServer charIsc(true);
  CCharISC* charIscClient = new CCharISC();
  CMapServer mapIsc(true);
  CMapISC* mapIscClient = new CMapISC();

  charIscClient->set_socket(new Core::CNetwork_Asio());
  mapIscClient->set_socket(new Core::CNetwork_Asio());

  loginIsc.init("127.0.0.1", 29010);

  charIsc.init("127.0.0.1", 29110);
  charIscClient->init("127.0.0.1", 29010);
  charIscClient->SetLogin(true);

  mapIsc.init("127.0.0.1", 29210);
  mapIscClient->init("127.0.0.1", 29110);
  mapIscClient->set_type(iscPacket::ServerType::CHAR);

  loginIsc.listen();
  charIsc.listen();
  mapIsc.listen();

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  CCharServer::GetISCList().push_front(charIscClient);
  CMapServer::GetISCList().push_front(mapIscClient);

  charIscClient->connect();
  mapIscClient->connect();

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

  mapIscClient->shutdown(true);
  charIscClient->shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  mapIsc.shutdown(true);
  charIsc.shutdown(true);
  loginIsc.shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
