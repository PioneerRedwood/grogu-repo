// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_STD_WIN_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_STD_WIN_H_
#pragma once

#include "tests/cefclient/browser/browser_window.h"

// updated 2021-12-15 

namespace client {

// Represents a native child window hosting a single windowed browser instance.
// The methods of this class must be called on the main thread unless otherwise
// indicated.
// 단일 창으로된 브라우저 인스턴스를 호스팅하는 네이티브 자식 윈도우를 나타냅니다.
// 특별한 명시가 없다면 해당 클래스의 메서드는 오직 메인 스레드에서만 호출됩니다.
class BrowserWindowStdWin : public BrowserWindow {
 public:
  // Constructor may be called on any thread.
  // |delegate| must outlive this object.
  // 생성자는 어느 스레드에서든 호출될 수 있습니다.
  BrowserWindowStdWin(Delegate* delegate, const std::string& startup_url);

  // BrowserWindow methods.
  void CreateBrowser(ClientWindowHandle parent_handle,
                     const CefRect& rect,
                     const CefBrowserSettings& settings,
                     CefRefPtr<CefDictionaryValue> extra_info,
                     CefRefPtr<CefRequestContext> request_context) OVERRIDE;
  void GetPopupConfig(CefWindowHandle temp_handle,
                      CefWindowInfo& windowInfo,
                      CefRefPtr<CefClient>& client,
                      CefBrowserSettings& settings) OVERRIDE;
  void ShowPopup(ClientWindowHandle parent_handle,
                 int x,
                 int y,
                 size_t width,
                 size_t height) OVERRIDE;
  void Show() OVERRIDE;
  void Hide() OVERRIDE;
  void SetBounds(int x, int y, size_t width, size_t height) OVERRIDE;
  void SetFocus(bool focus) OVERRIDE;
  ClientWindowHandle GetWindowHandle() const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserWindowStdWin);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_STD_WIN_H_
