#pragma once

#include "pipeline/image_buffer.hpp"
#include <algorithm>

namespace Nodexel::Pipeline::Algorithms {

inline void pixelate(ImageBuffer &buffer, int blockSize) {
  if (blockSize <= 1)
    return;

  int w = buffer.width;
  int h = buffer.height;
  auto &pixels = buffer.pixels;

  for (int y = 0; y < h; y += blockSize) {
    for (int x = 0; x < w; x += blockSize) {

      int sampleX = std::min(x, w - 1);
      int sampleY = std::min(y, h - 1);
      int baseIdx = (sampleY * w + sampleX) * 4;

      uint8_t r = pixels[baseIdx];
      uint8_t g = pixels[baseIdx + 1];
      uint8_t b = pixels[baseIdx + 2];
      uint8_t a = pixels[baseIdx + 3];

      for (int by = 0; by < blockSize && (y + by) < h; ++by) {
        for (int bx = 0; bx < blockSize && (x + bx) < w; ++bx) {
          int targetIdx = ((y + by) * w + (x + bx)) * 4;
          pixels[targetIdx] = r;
          pixels[targetIdx + 1] = g;
          pixels[targetIdx + 2] = b;
          pixels[targetIdx + 3] = a;
        }
      }
    }
  }
}

} // namespace Nodexel::Pipeline::Algorithms
