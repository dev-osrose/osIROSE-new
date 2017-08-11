#include "gtest/gtest.h"

#include "cnetwork_asio.h"
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

  charIscClient->set_socket(std::make_unique<Core::CNetwork_Asio>());
  mapIscClient->set_socket(std::make_unique<Core::CNetwork_Asio>());

  loginIsc.init("127.0.0.1", 29010);

  charIsc.init("127.0.0.1", 29110);
  charIscClient->init("127.0.0.1", 29010);
  charIscClient->SetLogin(true);

  mapIsc.init("127.0.0.1", 29210);
  mapIscClient->init("127.0.0.1", 29110);
  mapIscClient->set_type(to_underlying(Isc::ServerType::CHAR));

  loginIsc.listen();
  charIsc.listen();
  mapIsc.listen();

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  charIscClient->connect();
  mapIscClient->connect();
  CMapISC *map = mapIscClient.get();
  CCharISC *charc = charIscClient.get();

  CCharServer::GetISCList().push_front(std::move(charIscClient));
  CMapServer::GetISCList().push_front(std::move(mapIscClient));

  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

  map->shutdown(true);
  charc->shutdown(true);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  mapIsc.shutdown(true);
  charIsc.shutdown(true);
  loginIsc.shutdown(true);

  std::this_thread::sleep_for(std::chrono::seconds(1));
}
