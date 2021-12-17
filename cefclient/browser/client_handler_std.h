// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_STD_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_STD_H_
#pragma once

#include "tests/cefclient/browser/client_handler.h"

namespace client {

// Client handler implementation for windowed browsers. There will only ever be
// one browser per handler instance.
// 창이 있는 브라우저에 대한 클라이언트 핸들러 구현
class ClientHandlerStd : public ClientHandler {
 public:
  ClientHandlerStd(Delegate* delegate, const std::string& startup_url);

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandlerStd);
  DISALLOW_COPY_AND_ASSIGN(ClientHandlerStd);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_HANDLER_STD_H_
