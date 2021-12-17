// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

// updated 2021-12-16

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_IMAGE_CACHE_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_IMAGE_CACHE_H_
#pragma once

#include <map>
#include <vector>

#include "include/base/cef_bind.h"
#include "include/base/cef_ref_counted.h"
#include "include/cef_image.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace client {

// Simple image caching implementation.
// 간단한 이미지 캐싱 구현
class ImageCache
    : public base::RefCountedThreadSafe<ImageCache, CefDeleteOnUIThread> {
 public:
  ImageCache();

  // Image representation at a specific scale factor.
  // 특정 스케일 요소에서 이미지 표현
  struct ImageRep {
    ImageRep(const std::string& path, float scale_factor);

    // Full file system path.
    std::string path_;

    // Image scale factor (usually 1.0f or 2.0f).
    float scale_factor_;
  };
  typedef std::vector<ImageRep> ImageRepSet;

  // Unique image that may have multiple representations.
  // 여러번 표시되는 유일한 이미지
  struct ImageInfo {
    ImageInfo(const std::string& id,
              const ImageRepSet& reps,
              bool internal,
              bool force_reload);

    // Helper for returning an empty image.
    // 빈 이미지를 반환하는 헬퍼
    static ImageInfo Empty();

    // Helpers for creating common representations.
    // 공통 표시를 생성하는 헬퍼
    static ImageInfo Create1x(const std::string& id,
                              const std::string& path_1x,
                              bool internal);
    static ImageInfo Create2x(const std::string& id,
                              const std::string& path_1x,
                              const std::string& path_2x,
                              bool internal);
    static ImageInfo Create2x(const std::string& id);

    // Image unique ID.
    std::string id_;

    // Image representations to load.
    ImageRepSet reps_;

    // True if the image is internal (loaded via LoadBinaryResource).
    bool internal_;

    // True to force reload.
    bool force_reload_;
  };
  typedef std::vector<ImageInfo> ImageInfoSet;

  typedef std::vector<CefRefPtr<CefImage>> ImageSet;

  typedef base::Callback<void(const ImageSet& /*images*/)> LoadImagesCallback;

  // Loads the images represented by |image_info|. Executes |callback|
  // either synchronously or asychronously on the UI thread after completion.
  // |image_info|에 의해 표시되는 이미지를 로드합니다.
  // 완료되면 UI 스레드에서 동기/비동기 모두 |callback|이 수행됩니다.
  void LoadImages(const ImageInfoSet& image_info,
                  const LoadImagesCallback& callback);

  // Returns an image that has already been cached. Must be called on the
  // UI thread.
  // 미리 캐시된 이미지를 반환합니다.
  // UI 스레드에서 호출돼야만 합니다.
  CefRefPtr<CefImage> GetCachedImage(const std::string& image_id);

 private:
  // Only allow deletion via scoped_refptr.
  // 오직 scoped_refptr에 의한 삭제를 허용합니다.
  friend struct CefDeleteOnThread<TID_UI>;
  friend class base::RefCountedThreadSafe<ImageCache, CefDeleteOnUIThread>;

  ~ImageCache();

  enum ImageType {
    TYPE_NONE,
    TYPE_PNG,
    TYPE_JPEG,
  };

  static ImageType GetImageType(const std::string& path);

  struct ImageContent;
  typedef std::vector<ImageContent> ImageContentSet;

  // Load missing image contents on the FILE thread.
  // FILE 스레드에서 누락된 이미지 컨텐츠를 로드합니다.
  void LoadMissing(const ImageInfoSet& image_info,
                   const ImageSet& images,
                   const LoadImagesCallback& callback);
  static bool LoadImageContents(const ImageInfo& info, ImageContent* content);
  static bool LoadImageContents(const std::string& path,
                                bool internal,
                                ImageType* type,
                                std::string* contents);

  // Create missing CefImage representations on the UI thread.
  // UI 스레드에 누락된 CefImage 표현을 생성합니다.
  void UpdateCache(const ImageInfoSet& image_info,
                   const ImageContentSet& contents,
                   const LoadImagesCallback& callback);
  static CefRefPtr<CefImage> CreateImage(const std::string& image_id,
                                         const ImageContent& content);

  // Map image ID to image representation. Only accessed on the UI thread.
  // 이미지 표현을 위한 이미지 ID 맵. UI 스레드에서만 접근됩니다.
  typedef std::map<std::string, CefRefPtr<CefImage>> ImageMap;
  ImageMap image_map_;
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_IMAGE_CACHE_H_
