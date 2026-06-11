#pragma once

#include "pipeline/image_buffer.hpp"
#include <cmath>

namespace Nodexel::Pipeline::Algorithms {

inline void colorQuantize(ImageBuffer &buffer, int colorCount) {
  if (colorCount < 2)
    return;

  int totalPixels = buffer.width * buffer.height * 4;
  auto &pixels = buffer.pixels;
  float step = 255.0f / static_cast<float>(colorCount - 1);

  for (int i = 0; i < totalPixels; i += 4) {
    pixels[i] = static_cast<uint8_t>(std::round(pixels[i] / step) * step);
    pixels[i + 1] =
        static_cast<uint8_t>(std::round(pixels[i + 1] / step) * step);
    pixels[i + 2] =
        static_cast<uint8_t>(std::round(pixels[i + 2] / step) * step);
  }
}

} // namespace Nodexel::Pipeline::Algorithms
