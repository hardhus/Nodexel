#include "pipeline_manager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>

namespace Nodexel::Pipeline {

PipelineManager::PipelineManager() {
  m_output_texture = std::make_shared<Texture>();
}

PipelineManager::~PipelineManager() {
  if (m_worker_thread.joinable()) {
    m_worker_thread.join();
  }
}

void PipelineManager::triggerProcess(const std::string &sourcePath,
                                     int blockSize, int colorCount,
                                     bool useQuantize) {
  if (m_is_processing)
    return;

  m_is_processing = true;
  if (m_worker_thread.joinable())
    m_worker_thread.join();

  m_worker_thread = std::thread(&PipelineManager::asyncWorker, this, sourcePath,
                                blockSize, colorCount, useQuantize);
}

void PipelineManager::asyncWorker(std::string path, int blockSize,
                                  int colorCount, bool useQuantize) {
  int w, h, ch;
  stbi_set_flip_vertically_on_load(false);
  unsigned char *data = stbi_load(path.c_str(), &w, &h, &ch, 4);

  if (!data) {
    m_is_processing = false;
    return;
  }

  int totalPixels = w * h * 4;
  std::vector<uint8_t> resultPixels(data, data + totalPixels);
  stbi_image_free(data);

  for (int y = 0; y < h; y += blockSize) {
    for (int x = 0; x < w; x += blockSize) {

      int sampleX = std::min(x, w - 1);
      int sampleY = std::min(y, h - 1);
      int baseIdx = (sampleY * w + sampleX) * 4;

      uint8_t r = resultPixels[baseIdx];
      uint8_t g = resultPixels[baseIdx + 1];
      uint8_t b = resultPixels[baseIdx + 2];
      uint8_t a = resultPixels[baseIdx + 3];

      for (int by = 0; by < blockSize && (y + by) < h; ++by) {
        for (int bx = 0; bx < blockSize && (x + bx) < w; ++bx) {
          int targetIdx = ((y + by) * w + (x + bx)) * 4;
          resultPixels[targetIdx] = r;
          resultPixels[targetIdx + 1] = g;
          resultPixels[targetIdx + 2] = b;
          resultPixels[targetIdx + 3] = a;
        }
      }
    }
  }

  if (useQuantize && colorCount > 0) {
    float step = 255.0f / static_cast<float>(colorCount - 1);
    for (int i = 0; i < totalPixels; i += 4) {
      resultPixels[i] =
          static_cast<uint8_t>(std::round(resultPixels[i] / step) * step);
      resultPixels[i + 1] =
          static_cast<uint8_t>(std::round(resultPixels[i + 1] / step) * step);
      resultPixels[i + 2] =
          static_cast<uint8_t>(std::round(resultPixels[i + 2] / step) * step);
    }
  }

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_processed_result.pixels = std::move(resultPixels);
    m_processed_result.width = w;
    m_processed_result.height = h;
  }

  m_new_result_ready = true;
  m_is_processing = false;
}

void PipelineManager::updateTextures() {
  if (m_new_result_ready) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_processed_result.isValid()) {
      m_output_texture->loadFromMemory(m_processed_result.pixels.data(),
                                       m_processed_result.width,
                                       m_processed_result.height);
    }
    m_new_result_ready = false;
  }
}

} // namespace Nodexel::Pipeline
