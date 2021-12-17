// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "common/scheme_test_common.h"
// #include "tests/shared/common/client_app.h" // 해당 디렉토리는 cefsimple/tests를 참고하십시오

namespace client {

// static
void ClientApp::RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {
  scheme_test::RegisterCustomSchemes(registrar);
}

}  // namespace client
