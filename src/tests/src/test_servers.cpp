#include "gtest/gtest.h"

#include <stdint.h>
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

  loginIsc.Init("127.0.0.1", 29010);

  charIsc.Init("127.0.0.1", 29110);
  charIscClient->Init("127.0.0.1", 29010);
  charIscClient->SetLogin(true);

  mapIsc.Init("127.0.0.1", 29210);
  mapIscClient->Init("127.0.0.1", 29110);
  mapIscClient->SetType(iscPacket::ServerType::CHAR);

  loginIsc.Listen();
  charIsc.Listen();
  mapIsc.Listen();

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  CCharServer::GetISCList().push_front(std::shared_ptr<CRoseClient>(charIscClient));
  CMapServer::GetISCList().push_front(std::shared_ptr<CRoseClient>(mapIscClient));

  charIscClient->Connect();
  mapIscClient->Connect();

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

  mapIscClient->Shutdown(true);
  charIscClient->Shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  mapIsc.Shutdown(true);
  charIsc.Shutdown(true);
  loginIsc.Shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
