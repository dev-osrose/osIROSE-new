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

#pragma once

#include <memory>
#include <string>

namespace Core {
  class ICrashReport {
  public:
    virtual ~ICrashReport() = default;

    // This forces the application to crash.
    static void crash() { volatile int* a = (int*)(nullptr); *a = 1; }
  };

  // Crashpad, unlike breakpad, writes minidumps from a separate handler
  // process. Constructing this spawns that handler and keeps it attached for
  // the lifetime of the process; the handler is what writes and (optionally)
  // uploads the dump, so it survives even a corrupted crashing process.
  //
  // The crashpad headers are kept out of this header on purpose: they need
  // crashpad's include directories, and Core is linked PUBLIC by every server.
  class CrashReport : public ICrashReport {
  public:
    // database_path  where minidumps and crashpad's own state are stored
    // app_name       reported as the product name alongside the dump
    // upload_url     crash server endpoint; empty disables uploading
    // handler_path   path to crashpad_handler; empty means "next to this
    //                executable", which is where the build puts it
    CrashReport(std::string database_path, std::string app_name,
                std::string upload_url = "", std::string handler_path = "");
    ~CrashReport() override;

    CrashReport(const CrashReport&) = delete;
    CrashReport& operator=(const CrashReport&) = delete;

    // False when crash reports are compiled out, or when the handler could not
    // be found or started. Not fatal: the server runs on without dumps.
    bool is_active() const noexcept { return active_; }

  private:
    class Impl;

    bool active_ = false;
    std::unique_ptr<Impl> impl_;
  };
}
