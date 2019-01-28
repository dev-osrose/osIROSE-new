// Copyright 2019 Chirstopher Torres (Raven), L3nn0x
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

#ifndef _NODESESSIONS_H_
#define _NODESESSIONS_H_

#include <vector>
#include <iostream>
#include <sqlite3.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <singleton.h>

class NodeSessions : public Singleton<NodeSessions> {
  public:
    // struct sessions {
    //   uint32_t id;
    //   uint16_t charPort;
    //   uint16_t worldPort;
    //   std::string charIp;
    //   std::string worldIp;
    // };
  
    NodeSessions();
    virtual ~NodeSessions() = default;
    
    // auto& getDb() { return db; }
    
  private:
    // sqlpp::container::connection<std::vector<sessions>> db;
  
};

#endif