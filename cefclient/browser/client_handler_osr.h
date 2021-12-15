// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_OSR_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_OSR_H_
#pragma once

#include "tests/cefclient/browser/client_handler.h"

// updated 2021-12-15

namespace client {

// Client handler implementation for windowless browsers. There will only ever
// be one browser per handler instance.
// 창 없는 브라우저를 위한 클라이언트 핸들러 구현입니다.
// 하나의 브라우저에는 하나의 핸들러 인스턴스만 존재할 것입니다.
class ClientHandlerOsr : public ClientHandler,
                         public CefAccessibilityHandler,
                         public CefRenderHandler {
 public:
  // Implement this interface to receive notification of ClientHandlerOsr
  // events. The methods of this class will be called on the CEF UI thread.
  // 클라이언트 OSR 이벤트에 대한 통지를 수신하는 인터페이스를 구현합니다.
  // 이 클래스의 메서드는 CEF UI 스레드에서만 호출될 수 있습니다.
  class OsrDelegate {
   public:
    // These methods match the CefLifeSpanHandler interface.
    // CefLifeSpanHandler 인터페이스와 일치합니다.
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) = 0;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;

    // These methods match the CefRenderHandler interface.
    // CefRenderHandler 인터페이스와 일치합니다.
    virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                   CefRect& rect) = 0;
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) = 0;
    virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                int viewX,
                                int viewY,
                                int& screenX,
                                int& screenY) = 0;
    virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                               CefScreenInfo& screen_info) = 0;
    virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) = 0;
    virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                             const CefRect& rect) = 0;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                         CefRenderHandler::PaintElementType type,
                         const CefRenderHandler::RectList& dirtyRects,
                         const void* buffer,
                         int width,
                         int height) = 0;
    virtual void OnAcceleratedPaint(
        CefRefPtr<CefBrowser> browser,
        CefRenderHandler::PaintElementType type,
        const CefRenderHandler::RectList& dirtyRects,
        void* share_handle) {}
    virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefDragData> drag_data,
                               CefRenderHandler::DragOperationsMask allowed_ops,
                               int x,
                               int y) = 0;
    virtual void UpdateDragCursor(
        CefRefPtr<CefBrowser> browser,
        CefRenderHandler::DragOperation operation) = 0;
    virtual void OnImeCompositionRangeChanged(
        CefRefPtr<CefBrowser> browser,
        const CefRange& selection_range,
        const CefRenderHandler::RectList& character_bounds) = 0;

    // These methods match the CefDisplayHandler interface.
    // CefDisplayHandler 인터페이스와 일치합니다.
    virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                CefCursorHandle cursor,
                                cef_cursor_type_t type,
                                const CefCursorInfo& custom_cursor_info) = 0;

    virtual void UpdateAccessibilityTree(CefRefPtr<CefValue> value) = 0;
    virtual void UpdateAccessibilityLocation(CefRefPtr<CefValue> value) = 0;

   protected:
    virtual ~OsrDelegate() {}
  };

  ClientHandlerOsr(Delegate* delegate,
                   OsrDelegate* osr_delegate,
                   const std::string& startup_url);

  // This object may outlive the OsrDelegate object so it's necessary for the
  // OsrDelegate to detach itself before destruction.
  // 해당 객체는 OsrDelegate 객체보다 오래 살아남습니다. 그래서 
  // 파괴되기 전에 분리될 수 있습니다.
  void DetachOsrDelegate();

  // CefClient methods.
  // CefClient 메서드
  CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }
  CefRefPtr<CefAccessibilityHandler> GetAccessibilityHandler() OVERRIDE {
    return this;
  }

  // CefLifeSpanHandler methods.
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefRenderHandler methods.
  bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
  bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                      int viewX,
                      int viewY,
                      int& screenX,
                      int& screenY) OVERRIDE;
  bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                     CefScreenInfo& screen_info) OVERRIDE;
  void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
  void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE;
  void OnPaint(CefRefPtr<CefBrowser> browser,
               CefRenderHandler::PaintElementType type,
               const CefRenderHandler::RectList& dirtyRects,
               const void* buffer,
               int width,
               int height) OVERRIDE;
  void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                          CefRenderHandler::PaintElementType type,
                          const CefRenderHandler::RectList& dirtyRects,
                          void* share_handle) OVERRIDE;
  bool StartDragging(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefDragData> drag_data,
                     CefRenderHandler::DragOperationsMask allowed_ops,
                     int x,
                     int y) OVERRIDE;
  void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                        CefRenderHandler::DragOperation operation) OVERRIDE;
  void OnImeCompositionRangeChanged(
      CefRefPtr<CefBrowser> browser,
      const CefRange& selection_range,
      const CefRenderHandler::RectList& character_bounds) OVERRIDE;

  // CefDisplayHandler methods.
  bool OnCursorChange(CefRefPtr<CefBrowser> browser,
                      CefCursorHandle cursor,
                      cef_cursor_type_t type,
                      const CefCursorInfo& custom_cursor_info) OVERRIDE;

  // CefAccessibilityHandler methods.
  void OnAccessibilityTreeChange(CefRefPtr<CefValue> value) OVERRIDE;
  void OnAccessibilityLocationChange(CefRefPtr<CefValue> value) OVERRIDE;

 private:
  // Only accessed on the UI thread.
  // UI 스레드에서만 접근 가능
  OsrDelegate* osr_delegate_;

  // Include the default reference counting implementation.
  // 기본 참조 카운팅 구현 포함
  IMPLEMENT_REFCOUNTING(ClientHandlerOsr);
  DISALLOW_COPY_AND_ASSIGN(ClientHandlerOsr);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_OSR_H_
