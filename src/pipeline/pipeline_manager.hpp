#pragma once

#include "image_buffer.hpp"
#include "texture.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

namespace Nodexel::Pipeline {

class PipelineManager {
public:
  PipelineManager();
  ~PipelineManager();

  void triggerProcess(const std::string &sourcePath, int pixelblockSize,
                      int quantizeColorCount, bool useQuantize);

  void updateTextures();

  std::shared_ptr<Texture> getOutputTexture() { return m_output_texture; }

private:
  std::shared_ptr<Texture> m_output_texture;

  std::thread m_worker_thread;
  std::mutex m_mutex;
  std::atomic<bool> m_is_processing{false};
  std::atomic<bool> m_new_result_ready{false};

  ImageBuffer m_raw_input;
  ImageBuffer m_processed_result;

  void asyncWorker(std::string path, int blockSize, int colorCount,
                   bool useQuantize);
};

} // namespace Nodexel::Pipeline
