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

#include "colmap/feature/types.h"

namespace colmap {

// Convert feature keypoints to vector of points.
// 将特征关键点转换为点的向量。
std::vector<Eigen::Vector2d> FeatureKeypointsToPointsVector(
    const FeatureKeypoints& keypoints);

// L2-normalize feature descriptor, where each row represents one feature.
// 对特征描述子进行 L2 归一化，其中每一行代表一个特征。
void L2NormalizeFeatureDescriptors(FeatureDescriptorsFloat* descriptors);

// L1-Root-normalize feature descriptors, where each row represents one feature.
// See "Three things everyone should know to improve object retrieval",
// Relja Arandjelovic and Andrew Zisserman, CVPR 2012.
// 对特征描述子进行 L1-Root 归一化，其中每一行代表一个特征。
// 参考论文："Three things everyone should know to improve object retrieval",
// 作者：Relja Arandjelovic 和 Andrew Zisserman，CVPR 2012。
void L1RootNormalizeFeatureDescriptors(FeatureDescriptorsFloat* descriptors);

// Convert normalized floating point feature descriptor to unsigned byte
// representation by linear scaling from range [0, 0.5] to [0, 255]. Truncation
// to a maximum value of 0.5 is used to avoid precision loss and follows the
// common practice of representing SIFT vectors.
// 将归一化后的浮点型特征描述子通过线性缩放转换为无符号字节表示：
// 将数值范围从 [0, 0.5] 映射到 [0, 255]。
// 为了避免精度损失，数值会被截断至最大值 0.5，
// 这一做法符合 SIFT 特征向量表示的通用规范。
FeatureDescriptors FeatureDescriptorsToUnsignedByte(
    const Eigen::Ref<const FeatureDescriptorsFloat>& descriptors);

// Extract the descriptors corresponding to the largest-scale features.
// 提取对应于最大尺度特征的描述子。
void ExtractTopScaleFeatures(FeatureKeypoints* keypoints,
                             FeatureDescriptors* descriptors,
                             size_t num_features);

}  // namespace colmap
