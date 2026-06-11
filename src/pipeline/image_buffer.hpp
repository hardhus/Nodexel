#pragma once

#include <cstdint>
#include <vector>

namespace Nodexel::Pipeline {

struct ImageBuffer {
  std::vector<uint8_t> pixels;
  int width = 0;
  int height = 0;
  int channels = 4;

  bool isValid() const { return !pixels.empty() && width > 0 && height > 0; }
  void clear() {
    pixels.clear();
    width = 0;
    height = 0;
  }
};

} // namespace Nodexel::Pipeline
