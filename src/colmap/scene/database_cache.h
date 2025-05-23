// Copyright (c), ETH Zurich and UNC Chapel Hill.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of ETH Zurich and UNC Chapel Hill nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "colmap/geometry/gps.h"
#include "colmap/scene/camera.h"
#include "colmap/scene/correspondence_graph.h"
#include "colmap/scene/database.h"
#include "colmap/scene/image.h"
#include "colmap/sensor/models.h"
#include "colmap/util/eigen_alignment.h"
#include "colmap/util/types.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Eigen/Core>

namespace colmap {

// A class that caches the contents of the database in memory, used to quickly
// create new reconstruction instances when multiple models are reconstructed.
// 一个将数据库内容缓存在内存中的类，
// 用于在重建多个模型时快速创建新的重建实例。
class DatabaseCache {
 public:
  DatabaseCache();

  // Load cameras, images, features, and matches from database.
  //
  // @param database              Source database from which to load data.
  // @param min_num_matches       Only load image pairs with a minimum number
  //                              of matches.
  // @param ignore_watermarks     Whether to ignore watermark image pairs.
  // @param image_names           Whether to use only load the data for a subset
  //                              of the images. All images are used if empty.
  /**
   * 从数据库中加载相机、图像、特征和匹配信息。
   *  @param database              要加载数据的源数据库。
   * @param min_num_matches       仅加载匹配点数不少于该值的图像对。
   * @param ignore_watermarks     是否忽略带有水印的图像对。
   * @param image_names           是否只加载部分图像的数据；如果为空，则加载所有图像。
  */
  static std::shared_ptr<DatabaseCache> Create(
      const Database& database,
      size_t min_num_matches,
      bool ignore_watermarks,
      const std::unordered_set<std::string>& image_names);

  // Get number of objects.
  inline size_t NumCameras() const;
  inline size_t NumImages() const;
  inline size_t NumPosePriors() const;

  // Add objects.
  void AddCamera(struct Camera camera);
  void AddImage(class Image image);
  void AddPosePrior(image_t image_id, struct PosePrior pose_prior);

  // Get specific objects.
  inline struct Camera& Camera(camera_t camera_id);
  inline const struct Camera& Camera(camera_t camera_id) const;
  inline class Image& Image(image_t image_id);
  inline const class Image& Image(image_t image_id) const;
  inline struct PosePrior& PosePrior(image_t image_id);
  inline const struct PosePrior& PosePrior(image_t image_id) const;

  // Get all objects.
  inline const std::unordered_map<camera_t, struct Camera>& Cameras() const;
  inline const std::unordered_map<image_t, class Image>& Images() const;
  inline const std::unordered_map<image_t, struct PosePrior>& PosePriors()
      const;

  // Check whether specific object exists.
  inline bool ExistsCamera(camera_t camera_id) const;
  inline bool ExistsImage(image_t image_id) const;
  inline bool ExistsPosePrior(image_t image_id) const;

  // Get reference to const correspondence graph.
  inline std::shared_ptr<const class CorrespondenceGraph> CorrespondenceGraph()
      const;

  // Find specific image by name. Note that this uses linear search.
  const class Image* FindImageWithName(const std::string& name) const;

  // Setup PosePriors for PosePriorBundleAdjustment
  bool SetupPosePriors();

 private:
  std::shared_ptr<class CorrespondenceGraph> correspondence_graph_;

  std::unordered_map<camera_t, struct Camera> cameras_;
  std::unordered_map<image_t, class Image> images_;
  std::unordered_map<image_t, struct PosePrior> pose_priors_;
};

////////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////////

size_t DatabaseCache::NumCameras() const { return cameras_.size(); }
size_t DatabaseCache::NumImages() const { return images_.size(); }
size_t DatabaseCache::NumPosePriors() const { return pose_priors_.size(); }

struct Camera& DatabaseCache::Camera(const camera_t camera_id) {
  return cameras_.at(camera_id);
}

const struct Camera& DatabaseCache::Camera(const camera_t camera_id) const {
  return cameras_.at(camera_id);
}

class Image& DatabaseCache::Image(const image_t image_id) {
  return images_.at(image_id);
}

const class Image& DatabaseCache::Image(const image_t image_id) const {
  return images_.at(image_id);
}

struct PosePrior& DatabaseCache::PosePrior(image_t image_id) {
  return pose_priors_.at(image_id);
}

const struct PosePrior& DatabaseCache::PosePrior(image_t image_id) const {
  return pose_priors_.at(image_id);
}

const std::unordered_map<camera_t, struct Camera>& DatabaseCache::Cameras()
    const {
  return cameras_;
}

const std::unordered_map<image_t, class Image>& DatabaseCache::Images() const {
  return images_;
}

const std::unordered_map<image_t, struct PosePrior>& DatabaseCache::PosePriors()
    const {
  return pose_priors_;
}

bool DatabaseCache::ExistsCamera(const camera_t camera_id) const {
  return cameras_.find(camera_id) != cameras_.end();
}

bool DatabaseCache::ExistsImage(const image_t image_id) const {
  return images_.find(image_id) != images_.end();
}

bool DatabaseCache::ExistsPosePrior(const image_t image_id) const {
  return pose_priors_.find(image_id) != pose_priors_.end();
}

std::shared_ptr<const class CorrespondenceGraph>
DatabaseCache::CorrespondenceGraph() const {
  return correspondence_graph_;
}

}  // namespace colmap
