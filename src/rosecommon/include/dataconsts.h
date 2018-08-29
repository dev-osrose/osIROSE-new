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

#ifndef _DATACONSTS_H_
#define _DATACONSTS_H_

namespace RoseCommon {
#define MIN_SELL_TYPE     1
#define MAX_SELL_TYPE     11

#define MAX_UNION_COUNT   10
#define MAX_BUFF_STATUS   40
#define MAX_SKILL_COUNT   120
#define MAX_HOTBAR_ITEMS  32
#define MAX_DAMAGE        99999999

#define	DAMAGE_ACTION_ATTACK    0x02
#define	DAMAGE_ACTION_HIT       0x04
#define	DAMAGE_ACTION_CRITICAL  0x08
#define	DAMAGE_ACTION_DEAD      0x10
}

namespace ReviveReq {
    enum ReviveReq : uint8_t {
        REVIVE_POS,
        SAVE_POS,
        START_POST,
        CURRENT_POS
    };
}

#endif
