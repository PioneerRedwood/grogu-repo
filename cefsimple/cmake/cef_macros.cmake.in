# Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.
# 저작권

# FindCEF.cmake에 의해 로드돼야 함
# Must be loaded via FindCEF.cmake.
if(NOT DEFINED _CEF_ROOT_EXPLICIT)
  message(FATAL_ERROR "Use find_package(CEF) to load this file.")
endif()


#
# Shared macros.
# 공유 매크로
#

# 현재 CEF 설정을 출력
# Print the current CEF configuration.
macro(PRINT_CEF_CONFIG)
  message(STATUS "*** CEF CONFIGURATION SETTINGS ***")
  message(STATUS "Generator:                    ${CMAKE_GENERATOR}")
  message(STATUS "Platform:                     ${CMAKE_SYSTEM_NAME}")
  message(STATUS "Project architecture:         ${PROJECT_ARCH}")

  if(GEN_NINJA OR GEN_MAKEFILES)
    message(STATUS "Build type:                   ${CMAKE_BUILD_TYPE}")
  endif()

  message(STATUS "Binary distribution root:     ${_CEF_ROOT}")

  if(OS_MAC)
    message(STATUS "Base SDK:                     ${CMAKE_OSX_SYSROOT}")
    message(STATUS "Target SDK:                   ${CEF_TARGET_SDK}")
  endif()

  if(OS_WINDOWS)
    message(STATUS "Visual Studio ATL support:    ${USE_ATL}")
  endif()

  message(STATUS "CEF sandbox:                  ${USE_SANDBOX}")

  set(_libraries ${CEF_STANDARD_LIBS})
  if(OS_WINDOWS AND USE_SANDBOX)
    list(APPEND _libraries ${CEF_SANDBOX_STANDARD_LIBS})
  endif()
  message(STATUS "Standard libraries:           ${_libraries}")

  message(STATUS "Compile defines:              ${CEF_COMPILER_DEFINES}")
  message(STATUS "Compile defines (Debug):      ${CEF_COMPILER_DEFINES_DEBUG}")
  message(STATUS "Compile defines (Release):    ${CEF_COMPILER_DEFINES_RELEASE}")
  message(STATUS "C compile flags:              ${CEF_COMPILER_FLAGS} ${CEF_C_COMPILER_FLAGS}")
  message(STATUS "C compile flags (Debug):      ${CEF_COMPILER_FLAGS_DEBUG} ${CEF_C_COMPILER_FLAGS_DEBUG}")
  message(STATUS "C compile flags (Release):    ${CEF_COMPILER_FLAGS_RELEASE} ${CEF_C_COMPILER_FLAGS_RELEASE}")
  message(STATUS "C++ compile flags:            ${CEF_COMPILER_FLAGS} ${CEF_CXX_COMPILER_FLAGS}")
  message(STATUS "C++ compile flags (Debug):    ${CEF_COMPILER_FLAGS_DEBUG} ${CEF_CXX_COMPILER_FLAGS_DEBUG}")
  message(STATUS "C++ compile flags (Release):  ${CEF_COMPILER_FLAGS_RELEASE} ${CEF_CXX_COMPILER_FLAGS_RELEASE}")
  message(STATUS "Exe link flags:               ${CEF_LINKER_FLAGS} ${CEF_EXE_LINKER_FLAGS}")
  message(STATUS "Exe link flags (Debug):       ${CEF_LINKER_FLAGS_DEBUG} ${CEF_EXE_LINKER_FLAGS_DEBUG}")
  message(STATUS "Exe link flags (Release):     ${CEF_LINKER_FLAGS_RELEASE} ${CEF_EXE_LINKER_FLAGS_RELEASE}")
  message(STATUS "Shared link flags:            ${CEF_LINKER_FLAGS} ${CEF_SHARED_LINKER_FLAGS}")
  message(STATUS "Shared link flags (Debug):    ${CEF_LINKER_FLAGS_DEBUG} ${CEF_SHARED_LINKER_FLAGS_DEBUG}")
  message(STATUS "Shared link flags (Release):  ${CEF_LINKER_FLAGS_RELEASE} ${CEF_SHARED_LINKER_FLAGS_RELEASE}")

  if(OS_LINUX OR OS_WINDOWS)
    message(STATUS "CEF Binary files:             ${CEF_BINARY_FILES}")
    message(STATUS "CEF Resource files:           ${CEF_RESOURCE_FILES}")
  endif()
endmacro()

# 소스 리스트에 플랫폼 특정 소스들을 추가
# Append platform specific sources to a list of sources.
macro(APPEND_PLATFORM_SOURCES name_of_list)
  if(OS_LINUX AND ${name_of_list}_LINUX)
    list(APPEND ${name_of_list} ${${name_of_list}_LINUX})
  endif()
  if(OS_POSIX AND ${name_of_list}_POSIX)
    list(APPEND ${name_of_list} ${${name_of_list}_POSIX})
  endif()
  if(OS_WINDOWS AND ${name_of_list}_WINDOWS)
    list(APPEND ${name_of_list} ${${name_of_list}_WINDOWS})
  endif()
  if(OS_MAC AND ${name_of_list}_MAC)
    list(APPEND ${name_of_list} ${${name_of_list}_MAC})
  endif()
endmacro()

# 플랫폼과 제너레이터에 기반한 타겟 출력 디렉토리 지정
# Determine the target output directory based on platform and generator.
macro(SET_CEF_TARGET_OUT_DIR)
  if(GEN_NINJA OR GEN_MAKEFILES)
    # 기본적으로 Ninja와 Make 빌드는 서브 디렉토리를 생성하지 않음
    # By default Ninja and Make builds don't create a subdirectory named after
    # the configuration.
    set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

    # 출력 디렉토리( 실행파일, 라이브러리들) 해당하는 디렉토리에 맞게
    # Output binaries (executables, libraries) to the correct directory.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
  else()
    set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")
  endif()
endmacro()

# 하나의 디렉토리에서 다른 곳으로 파일들을 복사, 상대 경로로 지정된 파일들은 유지
# 소스 |파일들|의 경로 컴포넌트는 삭제됨
# Copy a list of files from one directory to another. Relative files paths are maintained.
# The path component of the source |file_list| will be removed.
macro(COPY_FILES target file_list source_dir target_dir)
  foreach(FILENAME ${file_list})
    set(source_file ${source_dir}/${FILENAME})
    get_filename_component(target_name ${FILENAME} NAME)
    set(target_file ${target_dir}/${target_name})

    string(FIND ${source_file} "$<CONFIGURATION>" _pos)
    if(NOT ${_pos} EQUAL -1)
      # 실제 설정 디렉토리를 테스트해야 함
      # Must test with an actual configuration directory.
      string(REPLACE "$<CONFIGURATION>" "Release" existing_source_file ${source_file})
      if(NOT EXISTS ${existing_source_file})
        string(REPLACE "$<CONFIGURATION>" "Debug" existing_source_file ${source_file})
      endif()
    else()
      set(existing_source_file ${source_file})
    endif()

    if(IS_DIRECTORY ${existing_source_file})
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_file}" "${target_file}"
        VERBATIM
        )
    else()
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_file}" "${target_file}"
        VERBATIM
        )
    endif()
  endforeach()
endmacro()


#
# Linux macros.
# 리눅스 매크로
#

if(OS_LINUX)

# pkg-config를 사용해 리눅스 라이브러리를 찾고 컴파일러와 링커 변수를 업데이트
# Use pkg-config to find Linux libraries and update compiler/linker variables.
macro(FIND_LINUX_LIBRARIES libraries)
  # Read pkg-config info into variables.
  execute_process(COMMAND pkg-config --cflags ${libraries} OUTPUT_VARIABLE FLL_CFLAGS)
  execute_process(COMMAND pkg-config --libs-only-L --libs-only-other ${libraries} OUTPUT_VARIABLE FLL_LDFLAGS)
  execute_process(COMMAND pkg-config --libs-only-l ${libraries} OUTPUT_VARIABLE FLL_LIBS)

  # 선행 후행 공백 제거
  # Strip leading and trailing whitepspace.
  STRING(STRIP "${FLL_CFLAGS}"  FLL_CFLAGS)
  STRING(STRIP "${FLL_LDFLAGS}" FLL_LDFLAGS)
  STRING(STRIP "${FLL_LIBS}"    FLL_LIBS)

  # 리스트로 변환
  # Convert to a list.
  separate_arguments(FLL_CFLAGS)
  separate_arguments(FLL_LDFLAGS)
  separate_arguments(FLL_LIBS)

  # 빌드 변수 업데이트
  # Update build variables.
  list(APPEND CEF_C_COMPILER_FLAGS    ${FLL_CFLAGS})
  list(APPEND CEF_CXX_COMPILER_FLAGS  ${FLL_CFLAGS})
  list(APPEND CEF_EXE_LINKER_FLAGS    ${FLL_LDFLAGS})
  list(APPEND CEF_SHARED_LINKER_FLAGS ${FLL_LDFLAGS})
  list(APPEND CEF_STANDARD_LIBS       ${FLL_LIBS})
endmacro()

# 지정된 실행파일의 SUID 권한 설정
# Set SUID permissions on the specified executable.
macro(SET_LINUX_SUID_PERMISSIONS target executable)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "*** Run the following command manually to set SUID permissions ***"
    COMMAND ${CMAKE_COMMAND} -E echo "EXE=\"${executable}\" && sudo -- chown root:root $EXE && sudo -- chmod 4755 $EXE"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    VERBATIM
    )
endmacro()

endif(OS_LINUX)


#
# Mac OS X macros.
# Mac OS 매크로
#

if(OS_MAC)

# 수동으로 실행하고 리소스 파일을 복사
# Manually process and copy over resource files.
macro(COPY_MAC_RESOURCES resource_list prefix_list target source_dir app_path)
  foreach(FILENAME ${resource_list})
    # 하나 이상 접두사를 소스 경로에서 삭제
    # Remove one or more prefixes from the source paths.
    set(TARGET_FILENAME "${FILENAME}")
    foreach(PREFIX ${prefix_list})
      string(REGEX REPLACE "^.*${PREFIX}" "" TARGET_FILENAME ${TARGET_FILENAME})
    endforeach()

    # 절대 경로 소스와 타겟 경로를 지정
    # Determine the absolute source and target paths.
    set(TARGET_PATH "${app_path}/Contents/Resources/${TARGET_FILENAME}")
    if(IS_ABSOLUTE ${FILENAME})
      set(SOURCE_PATH ${FILENAME})
    else()
      set(SOURCE_PATH "${source_dir}/${FILENAME}")
    endif()

    if(${FILENAME} MATCHES ".xib$")
      # 타겟 파일 확장자 변경
      # Change the target file extension.
      string(REGEX REPLACE ".xib$" ".nib" TARGET_PATH ${TARGET_PATH})

      get_filename_component(TARGET_DIRECTORY ${TARGET_PATH} PATH)
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        # 타겟 디렉토리 생성
        # Create the target directory.
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_DIRECTORY}"
        # XIB 파일을 NIB로 컴파일
        # Compile the XIB file to a NIB.
        COMMAND /usr/bin/ibtool --output-format binary1 --compile "${TARGET_PATH}" "${SOURCE_PATH}"
        VERBATIM
        )
    elseif(NOT ${TARGET_FILENAME} STREQUAL "Info.plist")
      # 그대로 파일 복사
      # Copy the file as-is.
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE_PATH}" "${TARGET_PATH}"
        VERBATIM
        )
    endif()
  endforeach()
endmacro()

endif(OS_MAC)


#
# Windows macros.
# 윈도우 매크로
#

if(OS_WINDOWS)

# 실행파일 타겟을 위해 커스텀 매니페스트 파일 생성
# Add custom manifest files to an executable target.
macro(ADD_WINDOWS_MANIFEST manifest_path target extension)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND "mt.exe" -nologo
            -manifest \"${manifest_path}/${target}.${extension}.manifest\" \"${manifest_path}/compatibility.manifest\"
            -outputresource:"${CEF_TARGET_OUT_DIR}/${target}.${extension}"\;\#1
    COMMENT "Adding manifest..."
    )
endmacro()

endif(OS_WINDOWS)


#
# Target configuration macros.
# 타겟 설정 매크로
#

# 실행파일 타겟에 명시된 라이브러리로 링크되어 사용할 수 있도록 논리적 타겟을 추가
# Add a logical target that can be used to link the specified libraries into an
# executable target.
macro(ADD_LOGICAL_TARGET target debug_lib release_lib)
  add_library(${target} ${CEF_LIBTYPE} IMPORTED)
  set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION "${release_lib}"
    IMPORTED_LOCATION_DEBUG "${debug_lib}"
    IMPORTED_LOCATION_RELEASE "${release_lib}"
    )
endmacro()

# 공통 타겟 특성 설정. 해당 매크로를 직접적으로 쓰기 대신에
# SET_LIBRARY_TARGET_PROPERTIES() 혹은 SET_EXECUTABLE_TARGET_PROPERTIES()를 사용할 것. 
# Set common target properties. Use SET_LIBRARY_TARGET_PROPERTIES() or
# SET_EXECUTABLE_TARGET_PROPERTIES() instead of calling this macro directly.
macro(SET_COMMON_TARGET_PROPERTIES target)
  # 컴파일 플래그
  # Compile flags.
  target_compile_options(${target} PRIVATE ${CEF_COMPILER_FLAGS} ${CEF_CXX_COMPILER_FLAGS})
  target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:${CEF_COMPILER_FLAGS_DEBUG} ${CEF_CXX_COMPILER_FLAGS_DEBUG}>)
  target_compile_options(${target} PRIVATE $<$<CONFIG:Release>:${CEF_COMPILER_FLAGS_RELEASE} ${CEF_CXX_COMPILER_FLAGS_RELEASE}>)

  # 컴파일 정의
  # Compile definitions.
  target_compile_definitions(${target} PRIVATE ${CEF_COMPILER_DEFINES})
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:${CEF_COMPILER_DEFINES_DEBUG}>)
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Release>:${CEF_COMPILER_DEFINES_RELEASE}>)

  # 포함 디렉토리
  # Include directories.
  target_include_directories(${target} PRIVATE ${CEF_INCLUDE_PATH})

  # 링커 플래그
  # Linker flags.
  if(CEF_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  if(CEF_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  if(CEF_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()

  if(OS_MAC)
    # Set Xcode target properties.
    set_target_properties(${target} PROPERTIES
      XCODE_ATTRIBUTE_ALWAYS_SEARCH_USER_PATHS                    NO
      XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD                 "gnu++11"   # -std=gnu++11
      XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME                     NO          # -fno-objc-link-runtime
      XCODE_ATTRIBUTE_CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS  YES         # -Wobjc-missing-property-synthesis
      XCODE_ATTRIBUTE_COPY_PHASE_STRIP                            NO
      XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING[variant=Release]        YES         # -Wl,-dead_strip
      XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD                     "c99"       # -std=c99
      XCODE_ATTRIBUTE_GCC_CW_ASM_SYNTAX                           NO          # No -fasm-blocks
      XCODE_ATTRIBUTE_GCC_DYNAMIC_NO_PIC                          NO
      XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS                   NO          # -fno-exceptions
      XCODE_ATTRIBUTE_GCC_ENABLE_CPP_RTTI                         NO          # -fno-rtti
      XCODE_ATTRIBUTE_GCC_ENABLE_PASCAL_STRINGS                   NO          # No -mpascal-strings
      XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN              YES         # -fvisibility-inlines-hidden
      XCODE_ATTRIBUTE_GCC_OBJC_CALL_CXX_CDTORS                    YES         # -fobjc-call-cxx-cdtors
      XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN                  YES         # -fvisibility=hidden
      XCODE_ATTRIBUTE_GCC_THREADSAFE_STATICS                      NO          # -fno-threadsafe-statics
      XCODE_ATTRIBUTE_GCC_TREAT_WARNINGS_AS_ERRORS                YES         # -Werror
      XCODE_ATTRIBUTE_GCC_VERSION                                 "com.apple.compilers.llvm.clang.1_0"
      XCODE_ATTRIBUTE_GCC_WARN_ABOUT_MISSING_NEWLINE              YES         # -Wnewline-eof
      XCODE_ATTRIBUTE_USE_HEADERMAP                               NO
      OSX_ARCHITECTURES_DEBUG                                     "${CMAKE_OSX_ARCHITECTURES}"
      OSX_ARCHITECTURES_RELEASE                                   "${CMAKE_OSX_ARCHITECTURES}"
      )
  endif()
endmacro()

# 라이브러리 특정 속성 설정
# Set library-specific properties.
macro(SET_LIBRARY_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})

  # 공유 라이브러리 링커 플래그
  # Shared library linker flags.
  if(CEF_SHARED_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  if(CEF_SHARED_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  if(CEF_SHARED_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()
endmacro()

# 실행파일 특정 속성 설정
# Set executable-specific properties.
macro(SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})

  # 실행파일 링커 플래그
  # Executable linker flags.
  if(CEF_EXE_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  # 디버그 링커
  if(CEF_EXE_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  # 릴리즈 링커
  if(CEF_EXE_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()
endmacro()
