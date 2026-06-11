#pragma once

#include "image_buffer.hpp"
#include "texture.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace Nodexel::Pipeline {

using FilterStep = std::function<void(ImageBuffer &)>;

class PipelineManager {
public:
  PipelineManager();
  ~PipelineManager();

  void triggerProcess(const std::string &sourcePath,
                      const std::vector<FilterStep> &pipelineSteps);

  void updateTextures();
  std::shared_ptr<Texture> getOutputTexture() { return m_output_texture; }
  bool isProcessing() const { return m_is_processing.load(); }

private:
  std::shared_ptr<Texture> m_output_texture;
  std::thread m_worker_thread;
  std::mutex m_mutex;
  std::atomic<bool> m_is_processing{false};
  std::atomic<bool> m_new_result_ready{false};

  ImageBuffer m_processed_result;
};
} // namespace Nodexel::Pipeline
