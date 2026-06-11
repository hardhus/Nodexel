#pragma once

#include "pipeline/image_buffer.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

namespace Nodexel::Pipeline::Algorithms {

inline void rescaleNearestNeighbor(ImageBuffer &buffer, int targetWidth,
                                   int targetHeight) {
  if (targetWidth <= 0 || targetHeight <= 0)
    return;
  if (targetWidth == buffer.width && targetHeight == buffer.height)
    return;

  int oldW = buffer.width;
  int oldH = buffer.height;
  auto &oldPixels = buffer.pixels;

  std::vector<uint8_t> newPixels(targetWidth * targetHeight * 4);

  float scaleX = static_cast<float>(oldW) / static_cast<float>(targetWidth);
  float scaleY = static_cast<float>(oldH) / static_cast<float>(targetHeight);

  for (int y = 0; y < targetHeight; ++y) {
    int srcY = std::min(static_cast<int>(std::floor(y * scaleY)), oldH - 1);

    for (int x = 0; x < targetWidth; ++x) {
      int srcX = std::min(static_cast<int>(std::floor(x * scaleX)), oldW - 1);

      int srcIdx = (srcY * oldW + srcX) * 4;
      int destIdx = (y * targetWidth + x) * 4;

      newPixels[destIdx] = oldPixels[srcIdx];
      newPixels[destIdx + 1] = oldPixels[srcIdx + 1];
      newPixels[destIdx + 2] = oldPixels[srcIdx + 2];
      newPixels[destIdx + 3] = oldPixels[srcIdx + 3];
    }
  }

  buffer.pixels = std::move(newPixels);
  buffer.width = targetWidth;
  buffer.height = targetHeight;
}

} // namespace Nodexel::Pipeline::Algorithms
