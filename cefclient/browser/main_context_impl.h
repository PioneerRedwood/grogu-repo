// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_IMPL_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_IMPL_H_
#pragma once

#include "include/base/cef_scoped_ptr.h"
#include "include/base/cef_thread_checker.h"
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "tests/cefclient/browser/main_context.h"
#include "tests/cefclient/browser/root_window_manager.h"

namespace client {

// Used to store global context in the browser process.
// 브라우저 프로세스 안에 전역 컨텍스트를 저장하기 위해 사용됩니다.
class MainContextImpl : public MainContext {
 public:
  MainContextImpl(CefRefPtr<CefCommandLine> command_line,
                  bool terminate_when_all_windows_closed);

  // MainContext members.
  std::string GetConsoleLogPath() OVERRIDE;
  std::string GetDownloadPath(const std::string& file_name) OVERRIDE;
  std::string GetAppWorkingDirectory() OVERRIDE;
  std::string GetMainURL() OVERRIDE;
  cef_color_t GetBackgroundColor() OVERRIDE;
  bool UseChromeRuntime() OVERRIDE;
  bool UseViews() OVERRIDE;
  bool UseWindowlessRendering() OVERRIDE;
  bool TouchEventsEnabled() OVERRIDE;
  void PopulateSettings(CefSettings* settings) OVERRIDE;
  void PopulateBrowserSettings(CefBrowserSettings* settings) OVERRIDE;
  void PopulateOsrSettings(OsrRendererSettings* settings) OVERRIDE;
  RootWindowManager* GetRootWindowManager() OVERRIDE;

  // Initialize CEF and associated main context state. This method must be
  // called on the same thread that created this object.
  // CEF와 메인 컨텍트스 상태와 관련된 것을 초기화합니다. 
  // 이 메서드는 해당 객체를 생성했던 동일한 스레드에서 호출돼야 합니다.
  bool Initialize(const CefMainArgs& args,
                  const CefSettings& settings,
                  CefRefPtr<CefApp> application,
                  void* windows_sandbox_info);

  // Shut down CEF and associated context state. This method must be called on
  // the same thread that created this object.
  // CEF와 컨텍스트 상태와 연관된 것들을 종료합니다.
  // 이 메서드는 해당 객체를 생성했던 동일한 스레드에서 호출돼야 합니다.
  void Shutdown();

 private:
  // Allow deletion via scoped_ptr only.
  // 오직 scoped_ptr에 의한 삭제를 허용합니다.
  friend struct base::DefaultDeleter<MainContextImpl>;

  ~MainContextImpl();

  // Returns true if the context is in a valid state (initialized and not yet
  // shut down).
  // 컨텍스트가 유효(아직 종료되지 않았고 초기화되지 않은)한 상태라면 true를 반환합니다.
  bool InValidState() const { return initialized_ && !shutdown_; }

  CefRefPtr<CefCommandLine> command_line_;
  const bool terminate_when_all_windows_closed_;

  // Track context state. Accessing these variables from multiple threads is
  // safe because only a single thread will exist at the time that they're set
  // (during context initialization and shutdown).
  // 컨텍스트 상태를 추적합니다. 변수들에 다수 스레드가 접근해도 안전합니다.
  // 이들이 설정되는 시점에는 단 하나의 스레드만 존재하기 때문입니다.
  bool initialized_;
  bool shutdown_;

  std::string main_url_;
  cef_color_t background_color_;
  cef_color_t browser_background_color_;
  bool use_windowless_rendering_;
  int windowless_frame_rate_;
  bool use_chrome_runtime_;
  bool use_views_;
  bool touch_events_enabled_;

  scoped_ptr<RootWindowManager> root_window_manager_;

#if defined(OS_WIN)
  bool shared_texture_enabled_;
#endif

  bool external_begin_frame_enabled_;

  // Used to verify that methods are called on the correct thread.
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(MainContextImpl);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_IMPL_H_
