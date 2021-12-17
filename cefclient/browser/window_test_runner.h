// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_WINDOW_TEST_RUNNER_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_WINDOW_TEST_RUNNER_H_
#pragma once

#include "include/cef_browser.h"

namespace client {
namespace window_test {

// Implement this interface for different platforms. Methods will be called on
// the browser process UI thread unless otherwise indicated.
// 다른 플랫폼에 적절한 인터페이스 구현입니다. 
// 특별히 명시하지 않으면 메서드는 브라우저 프로세스 UI 스레드에서 호출됩니다.
class WindowTestRunner {
 public:
  virtual void SetPos(CefRefPtr<CefBrowser> browser,
                      int x,
                      int y,
                      int width,
                      int height) = 0;
  virtual void Minimize(CefRefPtr<CefBrowser> browser) = 0;
  virtual void Maximize(CefRefPtr<CefBrowser> browser) = 0;
  virtual void Restore(CefRefPtr<CefBrowser> browser) = 0;

  // Fit |window| inside |display|. Coordinates are relative to the upper-left
  // corner of the display.
  static void ModifyBounds(const CefRect& display, CefRect& window);

  virtual ~WindowTestRunner() {}
};

}  // namespace window_test
}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_WINDOW_TEST_RUNNER_H_
