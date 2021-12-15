// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/cefclient/browser/client_browser.h"
#include "tests/cefclient/browser/main_context.h"

#include "include/cef_command_line.h"
#include "include/cef_crash_util.h"
#include "include/cef_file_util.h"
#include "tests/shared/common/client_switches.h"

// updated 2021-12-15 

namespace client {
namespace browser {

namespace {

class ClientBrowserDelegate : public ClientAppBrowser::Delegate {
 public:
  ClientBrowserDelegate() {}

  void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) OVERRIDE {
    if (CefCrashReportingEnabled()) {
      // Set some crash keys for testing purposes. Keys must be defined in the
      // "crash_reporter.cfg" file. See cef_crash_util.h for details.
      // 테스트 목적으로 충돌 키를 설정합니다. 키는 반드시
      // "crash_reporter.cfg"파일에 정의돼있어야 합니다.
      // 자세한 것은 cef_crash_util.h를 참고하십시오.
      CefSetCrashKeyValue("testkey_small1", "value1_small_browser");
      CefSetCrashKeyValue("testkey_small2", "value2_small_browser");
      CefSetCrashKeyValue("testkey_medium1", "value1_medium_browser");
      CefSetCrashKeyValue("testkey_medium2", "value2_medium_browser");
      CefSetCrashKeyValue("testkey_large1", "value1_large_browser");
      CefSetCrashKeyValue("testkey_large2", "value2_large_browser");
    }

    const std::string& crl_sets_path =
        CefCommandLine::GetGlobalCommandLine()->GetSwitchValue(
            switches::kCRLSetsPath);
    if (!crl_sets_path.empty()) {
      // Load the CRLSets file from the specified path.
      // 명시된 경로로부터 CRLSets 파일을 로드합니다.
      CefLoadCRLSetsFile(crl_sets_path);
    }
  }

  void OnBeforeCommandLineProcessing(
      CefRefPtr<ClientAppBrowser> app,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {
    // Append Chromium command line parameters if touch events are enabled
    // 터치 이벤트가 활성화될 경우에 크로미움 명령줄 파라미터를 추가합니다.
    if (client::MainContext::Get()->TouchEventsEnabled())
      command_line->AppendSwitchWithValue("touch-events", "enabled");
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ClientBrowserDelegate);
  IMPLEMENT_REFCOUNTING(ClientBrowserDelegate);
};

}  // namespace

void CreateDelegates(ClientAppBrowser::DelegateSet& delegates) {
  delegates.insert(new ClientBrowserDelegate);
}

}  // namespace browser
}  // namespace client
