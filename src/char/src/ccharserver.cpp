// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ccharserver.h"
#include "ccharclient.h"
#include "ccharisc.h"
#include "platform_defines.h"

using namespace RoseCommon;

CCharServer::CCharServer(bool _isc) : CRoseServer( _isc ),
                                      client_count_( 0 ),
                                      server_count_( 0 ) {}

CCharServer::~CCharServer() { Shutdown(); }

void CCharServer::OnAccepted(Core::INetwork* _sock) {
  //  if (_sock.is_open()) {
  // Do Something?
  //    std::string _address = _sock.remote_endpoint().address().to_string();
  if ( IsISCServer() == false ) {
    std::lock_guard<std::mutex> lock( client_list_mutex_ );
    CCharClient* nClient = new CCharClient( std::move( _sock ) );
    nClient->SetId( client_count_++ );
    nClient->SetLastUpdateTime( Core::Time::GetTickCount() );
    //      logger_->info( "[{}] Client connected from: {}", nClient->GetId(),
    //                       _address.c_str());
    client_list_.push_front( nClient );
  }
  else {
    std::lock_guard<std::mutex> lock( isc_list_mutex_ );
    CCharISC* nClient = new CCharISC( std::move( _sock ) );
    nClient->SetId( server_count_++ );
    nClient->SetLastUpdateTime( Core::Time::GetTickCount() );
    //      logger_->info( "Server connected from: {}", _address.c_str() );
    isc_list_.push_front( nClient );
  }
  //  }
}
