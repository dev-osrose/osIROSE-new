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
  std::unique_ptr<CCharISC> charIscClient = std::make_unique<CCharISC>();
  CMapServer mapIsc(true);
  std::unique_ptr<CMapISC> mapIscClient = std::make_unique<CMapISC>();

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

  charIscClient->Connect();
  mapIscClient->Connect();
  CMapISC *map = mapIscClient.get();
  CCharISC *charc = charIscClient.get();

  CCharServer::GetISCList().push_front(std::move(charIscClient));
  CMapServer::GetISCList().push_front(std::move(mapIscClient));

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

  map->Shutdown(true);
  charc->Shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  mapIsc.Shutdown(true);
  charIsc.Shutdown(true);
  loginIsc.Shutdown(true);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
