#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "pipeline_manager.hpp"
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

void PipelineManager::triggerProcess(
    const std::string &sourcePath,
    const std::vector<FilterStep> &pipelineSteps) {
  if (m_is_processing)
    return;

  m_is_processing = true;
  if (m_worker_thread.joinable())
    m_worker_thread.join();

  m_worker_thread = std::thread([this, sourcePath, pipelineSteps]() {
    int w, h, ch;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(sourcePath.c_str(), &w, &h, &ch, 4);

    if (!data) {
      m_is_processing = false;
      return;
    }

    ImageBuffer workerBuffer;
    workerBuffer.pixels.assign(data, data + (w * h * 4));
    workerBuffer.width = w;
    workerBuffer.height = h;
    workerBuffer.channels = 4;
    stbi_image_free(data);

    for (const auto &step : pipelineSteps) {
      step(workerBuffer);
    }

    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_processed_result = std::move(workerBuffer);
    }
    m_new_result_ready = true;
    m_is_processing = false;
  });
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
