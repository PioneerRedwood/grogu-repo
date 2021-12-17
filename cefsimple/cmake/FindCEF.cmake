# Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.
# 저작권

#
# This file is the CEF CMake configuration entry point and should be loaded
# using `find_package(CEF REQUIRED)`. See the top-level CMakeLists.txt file
# included with the CEF binary distribution for usage information.
#
# 해당 파일은 CEF CMake 설정 진입점이며 'find_package(CEF 요구됨)'을 통해 로드돼야 합니다.
# 사용 정보를 위해 CEF 이진 배포가 포함된 상위-레벨의 CMakeLists.txt 파일을 참고하십시오.
#

# CEF 이진 배포 파일을 루트 디렉토리에서 찾기
# Find the CEF binary distribution root directory.
set(_CEF_ROOT "")
if(CEF_ROOT AND IS_DIRECTORY "${CEF_ROOT}")
  set(_CEF_ROOT "${CEF_ROOT}")
  set(_CEF_ROOT_EXPLICIT 1)
else()
  set(_ENV_CEF_ROOT "")
  if(DEFINED ENV{CEF_ROOT})
    file(TO_CMAKE_PATH "$ENV{CEF_ROOT}" _ENV_CEF_ROOT)
  endif()
  if(_ENV_CEF_ROOT AND IS_DIRECTORY "${_ENV_CEF_ROOT}")
    set(_CEF_ROOT "${_ENV_CEF_ROOT}")
    set(_CEF_ROOT_EXPLICIT 1)
  endif()
  unset(_ENV_CEF_ROOT)
endif()

# 만약 _CEF_ROOT_EXPLICIT가 명시적으로 정의돼있지 않다면 치명적 오류!
if(NOT DEFINED _CEF_ROOT_EXPLICIT)
  message(FATAL_ERROR "Must specify a CEF_ROOT value via CMake or environment variable.")
endif()

# _CEF_ROOT/cmake를 찾을 수 없는 경우
if(NOT IS_DIRECTORY "${_CEF_ROOT}/cmake")
  message(FATAL_ERROR "No CMake bootstrap found for CEF binary distribution at: ${CEF_ROOT}.")
endif()

# CEF 이진 배포로부터 추가적인 cmake 파일을 실행
# Execute additional cmake files from the CEF binary distribution.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${_CEF_ROOT}/cmake")
include("cef_variables")
include("cef_macros")
