#include "stb_image.h"

#include "texture.hpp"

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include <iostream>

namespace Nodexel::Pipeline {

Texture::~Texture() { release(); }

bool Texture::loadFromFile(const std::string &filePath) {
  release();

  stbi_set_flip_vertically_on_load(false);

  int channels = 0;
  unsigned char *data =
      stbi_load(filePath.c_str(), &m_width, &m_height, &channels, 4);

  if (!data) {
    std::cout << "[ERROR] Failed to load image: " << filePath
              << " Reason: " << stbi_failure_reason() << std::endl;
    return false;
  }

  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);

  m_is_valid = true;
  std::cout << "[INFO] Successfully loaded texture to GPU. Size: " << m_width
            << "x" << m_height << std::endl;
  return true;
}

void Texture::release() {
  if (m_texture_id != 0) {
    glDeleteTextures(1, &m_texture_id);
    m_texture_id = 0;
  }
  m_width = 0;
  m_height = 0;
  m_is_valid = false;
}

bool Texture::loadFromMemory(const unsigned char *pixels, int width,
                             int height) {
  release();
  m_width = width;
  m_height = height;

  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  m_is_valid = true;
  return true;
}
} // namespace Nodexel::Pipeline
