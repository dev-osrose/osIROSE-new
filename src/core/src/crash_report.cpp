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

#ifdef ENABLE_CRASH_REPORTS

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <system_error>
#include <utility>
#include <vector>

#include "client/crash_report_database.h"
#include "client/crashpad_client.h"
#include "client/settings.h"
#include "version.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace Core {
namespace {

#ifdef _WIN32
  constexpr const char* kHandlerName = "crashpad_handler.exe";
#else
  constexpr const char* kHandlerName = "crashpad_handler";
#endif

  // base::FilePath is wchar_t based on Windows and char based everywhere else.
  base::FilePath to_file_path(const std::filesystem::path& path) {
#ifdef _WIN32
    return base::FilePath(path.wstring());
#else
    return base::FilePath(path.string());
#endif
  }

  // The build drops crashpad_handler into the same bin/ directory as the
  // servers, so "next to me" is the sane default lookup.
  std::filesystem::path executable_directory() {
#ifdef _WIN32
    std::wstring buffer(MAX_PATH, L'\0');
    for (;;) {
      const DWORD written =
          GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
      if (written == 0) return {};
      if (written < buffer.size()) {
        buffer.resize(written);
        break;
      }
      buffer.resize(buffer.size() * 2);
    }
    return std::filesystem::path(buffer).parent_path();
#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string buffer(size, '\0');
    if (_NSGetExecutablePath(buffer.data(), &size) != 0) return {};
    buffer.resize(std::strlen(buffer.c_str()));
    return std::filesystem::path(buffer).parent_path();
#else
    std::error_code ec;
    const auto self = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (ec) return {};
    return self.parent_path();
#endif
  }

} // namespace

class CrashReport::Impl {
public:
  crashpad::CrashpadClient client;
  std::unique_ptr<crashpad::CrashReportDatabase> database;
};

CrashReport::CrashReport(std::string database_path, std::string app_name,
                         std::string upload_url, std::string handler_path)
    : impl_(std::make_unique<Impl>()) {
  const std::filesystem::path handler =
      handler_path.empty() ? executable_directory() / kHandlerName
                           : std::filesystem::path(handler_path);

  std::error_code ec;
  if (!std::filesystem::is_regular_file(handler, ec)) {
    std::fprintf(stderr, "crash reports disabled: no crashpad handler at '%s'\n",
                 handler.string().c_str());
    return;
  }

  const std::filesystem::path database_dir(database_path);
  std::filesystem::create_directories(database_dir, ec);
  if (ec) {
    std::fprintf(stderr, "crash reports disabled: cannot create '%s': %s\n",
                 database_dir.string().c_str(), ec.message().c_str());
    return;
  }

  impl_->database = crashpad::CrashReportDatabase::Initialize(to_file_path(database_dir));
  if (!impl_->database || impl_->database->GetSettings() == nullptr) {
    std::fprintf(stderr, "crash reports disabled: cannot open crash database in '%s'\n",
                 database_dir.string().c_str());
    return;
  }

  // The handler refuses to upload unless the database says so, regardless of
  // the url it was started with.
  impl_->database->GetSettings()->SetUploadsEnabled(!upload_url.empty());

  // Same keys the breakpad uploader used, so an existing crash server keeps
  // recognising the fields.
  const std::map<std::string, std::string> annotations{
      {"product_name", std::move(app_name)},
      {"version", PROJECT_VERSION},
      {"branch", GIT_BRANCH},
      {"commit_hash", GIT_COMMIT_HASH},
      {"last_tag", GIT_LATEST_TAG},
  };

  // Crashpad throttles to one upload per hour by default, which would hide
  // every crash after the first during an incident.
  const std::vector<std::string> arguments{"--no-rate-limit"};

  active_ = impl_->client.StartHandler(to_file_path(handler),
                                       to_file_path(database_dir),
                                       to_file_path(database_dir), // metrics dir
                                       upload_url,
                                       /*http_proxy=*/std::string{},
                                       annotations,
                                       arguments,
                                       /*restartable=*/true,
                                       /*asynchronous_start=*/false);

  if (!active_) {
    std::fprintf(stderr, "crash reports disabled: crashpad handler '%s' failed to start\n",
                 handler.string().c_str());
    return;
  }

  std::fprintf(stderr, "crash reports enabled, minidumps go to '%s'%s\n",
               database_dir.string().c_str(),
               upload_url.empty() ? " (no upload url configured)" : "");
}

CrashReport::~CrashReport() = default;

} // namespace Core

#else // ENABLE_CRASH_REPORTS

namespace Core {

class CrashReport::Impl {};

CrashReport::CrashReport([[maybe_unused]] std::string database_path,
                         [[maybe_unused]] std::string app_name,
                         [[maybe_unused]] std::string upload_url,
                         [[maybe_unused]] std::string handler_path) {}

CrashReport::~CrashReport() = default;

} // namespace Core

#endif // ENABLE_CRASH_REPORTS
