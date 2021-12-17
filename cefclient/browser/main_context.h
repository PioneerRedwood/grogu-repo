// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_H_
#pragma once

#include <string>

#include "include/base/cef_ref_counted.h"
#include "include/internal/cef_types_wrappers.h"
#include "tests/cefclient/browser/osr_renderer_settings.h"

namespace client {

class RootWindowManager;

// Used to store global context in the browser process. The methods of this
// class are thread-safe unless otherwise indicated.
// 브라우저 프로세스에 전역 컨텍스트를 저장하기 위해 사용됩니다.
// 특별한 명시가 없다면 해당 클래스의 메서드는 스레드 안전합니다.
class MainContext {
 public:
  // Returns the singleton instance of this object.
  // 해당 객체의 싱글턴 인스턴스를 반환합니다.
  static MainContext* Get();

  // Returns the full path to the console log file.
  // 콘솔 로그 파일의 전체 경로를 반환합니다.
  virtual std::string GetConsoleLogPath() = 0;

  // Returns the full path to |file_name|.
  // |file_name|에 대한 전체 경로를 반환합니다.
  virtual std::string GetDownloadPath(const std::string& file_name) = 0;

  // Returns the app working directory including trailing path separator.
  // 경로 구분자를 포함하여 앱 작업 디렉토리를 반환합니다.
  virtual std::string GetAppWorkingDirectory() = 0;

  // Returns the main application URL.
  // 메인 어플리케이션 URL을 반환합니다.
  virtual std::string GetMainURL() = 0;

  // Returns the background color.
  // 배경색을 반환합니다.
  virtual cef_color_t GetBackgroundColor() = 0;

  // Returns true if the Chrome runtime will be used.
  // 크롬 런타임이 사용된다면 true를 반환합니다.
  virtual bool UseChromeRuntime() = 0;

  // Returns true if the Views framework will be used.
  // Views 프레임워크를 사용된다면 true를 반환합니다.
  virtual bool UseViews() = 0;

  // Returns true if windowless (off-screen) rendering will be used.
  // 창 없는 OSR이 사용된다면 true를 반환합니다.
  virtual bool UseWindowlessRendering() = 0;

  // Returns true if touch events are enabled.
  // 터치 이벤트가 활성화돼 있다면 true를 반환합니다.
  virtual bool TouchEventsEnabled() = 0;

  // Populate |settings| based on command-line arguments.
  // 명령줄을 기반으로 |settings|를 채웁니다.
  virtual void PopulateSettings(CefSettings* settings) = 0;
  virtual void PopulateBrowserSettings(CefBrowserSettings* settings) = 0;
  virtual void PopulateOsrSettings(OsrRendererSettings* settings) = 0;

  // Returns the object used to create/manage RootWindow instances.
  // 루트 윈도우 인스턴스를 생성하거나 관리하기 위해 객체를 반환합니다.
  virtual RootWindowManager* GetRootWindowManager() = 0;

 protected:
  MainContext();
  virtual ~MainContext();

 private:
  DISALLOW_COPY_AND_ASSIGN(MainContext);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_MAIN_CONTEXT_H_
