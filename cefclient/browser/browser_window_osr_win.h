// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_OSR_WIN_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_OSR_WIN_H_
#pragma once

#include "browser/browser_window.h"
#include "browser/osr_window_win.h"

namespace client {

// Represents a native child window hosting a single off-screen browser
// instance. The methods of this class must be called on the main thread unless
// otherwise indicated.
// 이는 단일 오프-스크린 브라우저 인스턴스를 호스팅하는 네이티브 자식 윈도우를 대표합니다.
// 특별히 명시되지 않는 한 이 클래스의 메서드는 메인 스레드에서 호출됩니다.
class BrowserWindowOsrWin : public BrowserWindow,
                            public OsrWindowWin::Delegate {
 public:
  // Constructor may be called on any thread.
  // |delegate| must outlive this object.
  // 생성자는 어떠한 스레드에서 호출될 수 있습니다.
  // |delegate| 인자는 해당 오브젝트에서
  // 유효(outlive; out of scope가 발생하지 않도록?)해야합니다.
  BrowserWindowOsrWin(BrowserWindow::Delegate* delegate,
                      const std::string& startup_url,
                      const OsrRendererSettings& settings);

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
  void SetDeviceScaleFactor(float device_scale_factor) OVERRIDE;
  float GetDeviceScaleFactor() const OVERRIDE;
  ClientWindowHandle GetWindowHandle() const OVERRIDE;

 private:
  // ClienHandler::Delegate methods.
  void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // OsrWindowWin::Delegate methods.
  void OnOsrNativeWindowCreated(HWND hwnd) OVERRIDE;

  // The below members are only accessed on the main thread.
  // 아래 멤버는 오직 메인 스레드에서만 접근합니다.
  scoped_refptr<OsrWindowWin> osr_window_;
  HWND osr_hwnd_;

  float device_scale_factor_;

  DISALLOW_COPY_AND_ASSIGN(BrowserWindowOsrWin);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_OSR_WIN_H_
