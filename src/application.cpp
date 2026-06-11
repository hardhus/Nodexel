#include "GLFW/glfw3.h"
#include "application.hpp"
#include "editor_layout.hpp"
#include "graph/node_graph.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imnodes.h"
#include <filesystem>
#include <iostream>
#include <memory>

bool g_ini_file_exists = false;

Application::Application() {}
Application::~Application() { shutdown(); }

bool Application::init() {
  std::cout << "[INFO] Application::init() started." << std::endl;

  if (!glfwInit()) {
    std::cout << "[ERROR] glfwInit() failed!" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window =
      glfwCreateWindow(1280, 720, "Nodexel Engine v0.1.0", nullptr, nullptr);
  if (!m_window) {
    std::cout << "[ERROR] glfwCreateWindow() failed to create the window!"
              << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // V-Sync

  g_ini_file_exists = std::filesystem::exists("imgui.ini");
  initImGui();

  m_layout = std::make_unique<EditorLayout>();

  glfwSetWindowUserPointer(m_window, this);

  glfwSetDropCallback(m_window, [](GLFWwindow *window, int count,
                                   const char **paths) {
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app && app->m_layout) {
      NodeGraph *graph = app->m_layout->getNodeGraph();
      if (graph) {
        for (int i = 0; i < count; i++) {
          std::cout << "[INFO] Dragging file path: " << paths[i] << std::endl;
          float offset = i * 35.0f;
          graph->addInputNode(paths[i], 100.0f + offset, 150.0f + offset);
        }
      }
    }
  });
  return true;
}

void Application::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImNodes::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();
  ImNodes::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void Application::run() {
  bool is_first_frame = true;

  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();

    if (is_first_frame) {
      is_first_frame = false;
      std::cout << "[INFO] First frame event triggered. Constructing layouts..."
                << std::endl;

      if (!g_ini_file_exists) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id,
                                      ImGui::GetMainViewport()->Size);

        ImGuiID dock_id_upper;
        ImGuiID dock_id_preview = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Down, 0.32f, nullptr, &dock_id_upper);

        ImGuiID dock_id_node_editor;
        ImGuiID dock_id_control_panel =
            ImGui::DockBuilderSplitNode(dock_id_upper, ImGuiDir_Right, 0.17f,
                                        nullptr, &dock_id_node_editor);

        ImGui::DockBuilderDockWindow("Node Graph Editor", dock_id_node_editor);
        ImGui::DockBuilderDockWindow("Nodexel Control Panel",
                                     dock_id_control_panel);
        ImGui::DockBuilderDockWindow("Image Preview", dock_id_preview);

        ImGui::DockBuilderFinish(dockspace_id);
      }

      if (m_layout && m_layout->getNodeGraph()) {
        m_layout->getNodeGraph()->initDefaultLayout();
      }
    }

    if (m_layout) {
      m_layout->render();
    }

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
  }
}

void Application::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImNodes::DestroyContext();
  ImGui::DestroyContext();

  if (m_window) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
  }
  glfwTerminate();
}
