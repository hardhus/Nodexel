#include "editor_layout.hpp"
#include "imgui.h"
#include "imnodes.h"

EditorLayout::EditorLayout() {}

void EditorLayout::render() {
  renderControlPanel();
  renderNodeGraph();
  renderPreviewPanel();
}

void EditorLayout::renderControlPanel() {
  ImGui::Begin("Nodexel Control Panel");
  ImGui::Text("System: Working");
  ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Separator();

  if (ImGui::Button("Reprocess Image")) {
    // async trigger
  }
  ImGui::End();
}

void EditorLayout::renderNodeGraph() {
  ImGui::Begin("Node Graph Editor");
  ImNodes::BeginNodeEditor();

  ImNodes::BeginNode(1);
  ImNodes::BeginNodeTitleBar();
  ImGui::Text("Input Image");
  ImNodes::EndNodeTitleBar();
  ImNodes::BeginOutputAttribute(2);
  ImGui::Text("Image Data");
  ImNodes::EndOutputAttribute();
  ImNodes::EndNode();

  ImNodes::BeginNode(3);
  ImNodes::BeginNodeTitleBar();
  ImGui::Text("Pixelate Filter");
  ImNodes::EndNodeTitleBar();
  ImNodes::BeginInputAttribute(4);
  ImGui::Text("In");
  ImNodes::EndInputAttribute();
  ImNodes::BeginOutputAttribute(5);
  ImGui::Text("Out");
  ImNodes::EndOutputAttribute();
  ImNodes::EndNode();

  ImNodes::EndNodeEditor();
  ImGui::End();
}

void EditorLayout::renderPreviewPanel() {
  ImGui::Begin("Image Preview");

  ImGui::Columns(2, "PreviewColumns");

  ImGui::Text("Original Image");
  ImGui::Dummy(ImVec2(200, 200));

  ImGui::NextColumn();

  ImGui::Text("Pixel Art Result");
  ImGui::Dummy(ImVec2(200, 200));

  ImGui::Columns(1);
  ImGui::End();
}
