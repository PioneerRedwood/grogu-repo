// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_BYTES_WRITE_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_BYTES_WRITE_HANDLER_H_
#pragma once

#include "include/base/cef_lock.h"
#include "include/cef_stream.h"

namespace client {
// 바이트 스트림의 래퍼 인터페이스를 구현해 사용이 쉽게 만든것으로 추정
// 내부적으로 base::Autolock(cef_lock.h에 있습니다.)를 사용하고 있습니다.
class BytesWriteHandler : public CefWriteHandler {
 public:
  explicit BytesWriteHandler(size_t grow);
  ~BytesWriteHandler();

  size_t Write(const void* ptr, size_t size, size_t n) OVERRIDE;
  int Seek(int64 offset, int whence) OVERRIDE;
  int64 Tell() OVERRIDE;
  int Flush() OVERRIDE;
  bool MayBlock() OVERRIDE { return false; }

  void* GetData() { return data_; }
  int64 GetDataSize() { return offset_; }

 private:
  size_t Grow(size_t size);

  size_t grow_;
  void* data_;
  int64 datasize_;
  int64 offset_;

  base::Lock lock_;

  IMPLEMENT_REFCOUNTING(BytesWriteHandler);
  DISALLOW_COPY_AND_ASSIGN(BytesWriteHandler);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_BYTES_WRITE_HANDLER_H_
