// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
#pragma once

#include "include/cef_base.h"
#include "tests/shared/browser/client_app_browser.h"

namespace client {
namespace browser {

// Create the browser delegate. Called from client_app_delegates_browser.cc.
// 브라우저 델리게이트를 생성합니다. client_app_delegates_browser.cc 파일로부터 호출됩니다.
void CreateDelegates(ClientAppBrowser::DelegateSet& delegates);

}  // namespace browser
}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_CLIENT_BROWSER_H_
