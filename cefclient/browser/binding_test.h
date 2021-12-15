// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_BINDING_TEST_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_BINDING_TEST_H_
#pragma once

#include "tests/cefclient/browser/test_runner.h"

// updated 2021-12-15 

namespace client {
namespace binding_test {

// Create message handlers. Called from test_runner.cc.
// 메시지 핸들러를 생성합니다. test_runner에서 호출됩니다.
void CreateMessageHandlers(test_runner::MessageHandlerSet& handlers);

}  // namespace binding_test
}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_BINDING_TEST_H_
