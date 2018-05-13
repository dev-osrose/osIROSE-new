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

#include "crash_report.h"

namespace Core {
  
#ifdef ENABLE_CRASH_REPORTS
static bool dumpCallback(const wchar_t* dump_path, const wchar_t* minidump_id,
                         void* context, EXCEPTION_POINTERS* exinfo,
                         MDRawAssertionInfo* assertion, bool succeeded) {
    printf("Dump path: %ws\n", dump_path);
    return succeeded;
  }
  
  CrashReport::CrashReport(std::string path, [[maybe_unused]] std::string pipe /*= ""*/) : 
    _exception_handler(
            std::wstring(path.begin(), path.end()).c_str(), nullptr,
            dumpCallback,
            nullptr,
            google_breakpad::ExceptionHandler::ExceptionHandler::HANDLER_ALL,
            MiniDumpNormal, L"", nullptr)
  {
  }
#else
  CrashReport::CrashReport([[maybe_unused]] std::string path, [[maybe_unused]] std::string pipe /*= ""*/)
  {
  }
#endif
}