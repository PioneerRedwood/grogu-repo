// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_H_
#pragma once

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_browser.h"
#include "tests/cefclient/browser/client_handler.h"
#include "tests/cefclient/browser/client_types.h"

namespace client {

// Represents a native child window hosting a single browser instance. The
// methods of this class must be called on the main thread unless otherwise
// indicated.
// 단일 창으로된 브라우저 인스턴스를 호스팅하는 네이티브 자식 윈도우를 나타냅니다.
// 특별한 명시가 없다면 해당 클래스의 메서드는 오직 메인 스레드에서만 호출됩니다.
class BrowserWindow : public ClientHandler::Delegate {
 public:
  // This interface is implemented by the owner of the BrowserWindow. The
  // methods of this class will be called on the main thread.
  // 해당 인터페이스는 BrowserWindow의 소유자에 의해 구현됩니다.
  // 이 클래스의 메서드는 메인 스레드에서 호출될 것입니다.
  class Delegate {
   public:
    // Called when the browser has been created.
    // 브라우저가 생성됐을 때 호출됩니다.
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

    // Called when the BrowserWindow is closing.
    // 브라우저가 닫힐 때 호출됩니다.
    virtual void OnBrowserWindowClosing() {}

    // Called when the BrowserWindow has been destroyed.
    // 브라우저가 파괴(Destroyed)될 때 호출됩니다.
    virtual void OnBrowserWindowDestroyed() = 0;

    // Set the window URL address.
    // 윈도우 URL 주소 설정
    virtual void OnSetAddress(const std::string& url) = 0;

    // Set the window title.
    // 윈도우 제목 설정
    virtual void OnSetTitle(const std::string& title) = 0;

    // Set fullscreen mode.
    // 전체화면 모드로 설정
    virtual void OnSetFullscreen(bool fullscreen) = 0;

    // Auto-resize contents.
    // 콘텐츠 자동 크기 조절 
    virtual void OnAutoResize(const CefSize& new_size) = 0;

    // Set the loading state.
    // 로딩 상태 설정
    virtual void OnSetLoadingState(bool isLoading,
                                   bool canGoBack,
                                   bool canGoForward) = 0;

    // Set the draggable regions.
    // 드래그 가능한 범위 지정
    virtual void OnSetDraggableRegions(
        const std::vector<CefDraggableRegion>& regions) = 0;

   protected:
    virtual ~Delegate() {}
  };

  // Create a new browser and native window.
  // 새로운 브라우저 혹은 네이티브 창을 생성합니다.
  virtual void CreateBrowser(ClientWindowHandle parent_handle,
                             const CefRect& rect,
                             const CefBrowserSettings& settings,
                             CefRefPtr<CefDictionaryValue> extra_info,
                             CefRefPtr<CefRequestContext> request_context) = 0;

  // Retrieve the configuration that will be used when creating a popup window.
  // The popup browser will initially be parented to |temp_handle| which should
  // be a pre-existing hidden window. The native window will be created later
  // after the browser has been created. This method will be called on the
  // browser process UI thread.
  // 팝업 창을 생성할 때 사용될 설정을 가져옵니다.
  // 팝업 창은 사전에 히든 창으로 존재하는 |temp_handle|를 부모로 초기화될 것입니다.
  // 이 브라우저가 생성된 후에 네이티브 창이 생성될 것입니다.
  // UI 스레드의 브라우저 프로세스에서 해당 메서드가 호출될 것입니다.
  virtual void GetPopupConfig(CefWindowHandle temp_handle,
                              CefWindowInfo& windowInfo,
                              CefRefPtr<CefClient>& client,
                              CefBrowserSettings& settings) = 0;

  // Show the popup window with correct parent and bounds in parent coordinates.
  // 정확한 부모와 부모의 좌표에 맞는 바운드로 팝업 창을 보이게 합니다.
  virtual void ShowPopup(ClientWindowHandle parent_handle,
                         int x,
                         int y,
                         size_t width,
                         size_t height) = 0;

  // Show the window.
  // 창을 보이게 합니다.
  virtual void Show() = 0;

  // Hide the window.
  // 창을 숨깁니다.
  virtual void Hide() = 0;

  // Set the window bounds in parent coordinates.
  // 부모 좌표에 있는 바운드로 이 창을 설정합니다.
  virtual void SetBounds(int x, int y, size_t width, size_t height) = 0;

  // Set focus to the window.
  virtual void SetFocus(bool focus) = 0;

  // Set the device scale factor. Only used in combination with off-screen
  // rendering.
  // 기기 스케일 요소를 설정합니다. 오프-스크린 렌더링과 함께 사용됩니다.
  virtual void SetDeviceScaleFactor(float device_scale_factor);

  // Returns the device scale factor. Only used in combination with off-screen
  // rendering.
  // 기기 스케일 요소를 반환합니다. 오프-스크린 렌더링과 함께 사용됩니다.
  virtual float GetDeviceScaleFactor() const;

  // Returns the window handle.
  // 창 핸들을 반환합니다.
  virtual ClientWindowHandle GetWindowHandle() const = 0;

  // Returns the browser owned by the window.
  // 창에 의해 소유되는 브라우저를 반환합니다.
  CefRefPtr<CefBrowser> GetBrowser() const;

  // Returns true if the browser is closing.
  // 닫히고 있다면 true를 반환합니다.
  bool IsClosing() const;

 protected:
  // Allow deletion via scoped_ptr only.
  // scoped_ptr를 통해서만 삭제가 허용됩니다.
  friend struct base::DefaultDeleter<BrowserWindow>;

  // Constructor may be called on any thread.
  // |delegate| must outlive this object.
  // 생성자는 어느 스레드에서든 호출될 수 있습ㄴ디ㅏ.
  explicit BrowserWindow(Delegate* delegate);

  // ClientHandler::Delegate methods.
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  void OnBrowserClosing(CefRefPtr<CefBrowser> browser) OVERRIDE;
  void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE;
  void OnSetAddress(const std::string& url) OVERRIDE;
  void OnSetTitle(const std::string& title) OVERRIDE;
  void OnSetFullscreen(bool fullscreen) OVERRIDE;
  void OnAutoResize(const CefSize& new_size) OVERRIDE;
  void OnSetLoadingState(bool isLoading,
                         bool canGoBack,
                         bool canGoForward) OVERRIDE;
  void OnSetDraggableRegions(
      const std::vector<CefDraggableRegion>& regions) OVERRIDE;

  Delegate* delegate_;
  CefRefPtr<CefBrowser> browser_;
  CefRefPtr<ClientHandler> client_handler_;
  bool is_closing_;

 private:
  DISALLOW_COPY_AND_ASSIGN(BrowserWindow);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_BROWSER_WINDOW_H_
