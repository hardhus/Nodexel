#pragma once

#include "editor_layout.hpp"
#include <GLFW/glfw3.h>
#include <memory>

class Application {
public:
  Application();
  ~Application();

  bool init();
  void run();
  void shutdown();

private:
  GLFWwindow *m_window = nullptr;
  std::unique_ptr<EditorLayout> m_layout;

  void initImGui();
};
