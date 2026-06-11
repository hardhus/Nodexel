#pragma once

#include "imgui.h"
#include <string>

namespace Nodexel::Pipeline {

class Texture {
public:
  Texture() = default;
  ~Texture();

  bool loadFromFile(const std::string &filePath);
  bool loadFromMemory(const unsigned char *pixels, int width, int height);

  void release();

  ImTextureID getTextureId() const {
    return (ImTextureID)(uintptr_t)m_texture_id;
  }

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
  bool isValid() const { return m_is_valid; }

private:
  unsigned int m_texture_id = 0;
  int m_width = 0;
  int m_height = 0;
  bool m_is_valid = false;
};

} // namespace Nodexel::Pipeline
