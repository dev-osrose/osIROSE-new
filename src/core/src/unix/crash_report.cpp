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
#include "common/linux/http_upload.h"
#endif
#include "version.h"

namespace Core {

#ifdef ENABLE_CRASH_REPORTS
  static std::string crash_report_app_name = "";
  static std::string crash_report_upload_url = "";
  static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
    [[maybe_unused]] void* context, bool succeeded) {
    printf("Dump path: %s\n", descriptor.path());

    if (succeeded == true && crash_report_upload_url.size()) {
      std::map<string, string> parameters;
      std::map<string, string> files;
      std::string proxy_host;
      std::string proxy_userpasswd;
      std::string url(crash_report_upload_url);

      // Add any attributes to the parameters map.
      // Note that several attributes are automatically extracted.
      parameters["product_name"] = crash_report_app_name;
      parameters["version"] = PROJECT_VERSION;
      parameters["branch"] = GIT_BRANCH;
      parameters["commit_hash"] = GIT_COMMIT_HASH;
      parameters["last_tag"] = GIT_LATEST_TAG;
      files["upload_file_minidump"] = descriptor.path();

      std::string response, error;
    //   google_breakpad::HTTPUpload::SendRequest(url,
    //                                         parameters,
    //                                         files,
    //                                         proxy_host,
    //                                         proxy_userpasswd,
    //                                         "",
    //                                         &response,
    //                                         NULL,
    //                                         &error);
    }
    return succeeded;
  }

  CrashReport::CrashReport(std::string path, std::string app_name, [[maybe_unused]] std::string pipe /*= ""*/) : _exception_handler(google_breakpad::MinidumpDescriptor(path), NULL, dumpCallback, NULL, true, -1)
  {
    crash_report_app_name = app_name;
  }

  void CrashReport::set_url(std::string url)
  {
    crash_report_upload_url = url;
  }
#else
  CrashReport::CrashReport([[maybe_unused]] std::string path, [[maybe_unused]] std::string app_name, [[maybe_unused]] std::string pipe /*= ""*/)
  {
  }

  void CrashReport::set_url([[maybe_unused]] std::string url)
  {
  }
#endif
}