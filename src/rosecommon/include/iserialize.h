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

/*!
 * \file iserialize
 * \brief Contains the ISerialize interface
 *
 * \author L3nn0x
 * \date october 2016
 */

#ifndef __ISERIALIZE_H__
#define __ISERIALIZE_H__

namespace RoseCommon {

class CRoseBasePolicy;
class CRoseReader;

/*!
 * \class ISerialize
 * \brief This class is an Interface to allow serialization. Mostly used with \s Item
 *
 * \author L3nn0x
 * \date october 2016
 */
class ISerialize {
	public:
		virtual ~ISerialize() = default;

        virtual bool read(CRoseReader& reader) = 0;
        virtual bool write(CRoseBasePolicy& writer) const = 0;
};

}

#endif
