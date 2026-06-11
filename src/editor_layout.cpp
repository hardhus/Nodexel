#include "editor_layout.hpp"
#include "graph/node_graph.hpp"
#include "imgui.h"
#include "imnodes.h"
#include <memory>

EditorLayout::EditorLayout() { m_node_graph = std::make_unique<NodeGraph>(); }
EditorLayout::~EditorLayout() = default;

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
    m_node_graph->updatePipeline();
  }
  ImGui::End();
}

void EditorLayout::renderNodeGraph() {
  ImGui::Begin("Node Graph Editor");

  if (m_node_graph)
    m_node_graph->drawGraph();

  ImGui::End();
}

void handleImageInteractionAndDraw(ImTextureID texId, float &zoom, ImVec2 &pan,
                                   int origWidth, int origHeight) {
  ImVec2 availRegion = ImGui::GetContentRegionAvail();
  if (availRegion.x <= 0.0f || availRegion.y <= 0.0f)
    return;

  float ratioX = availRegion.x / (float)origWidth;
  float ratioY = availRegion.y / (float)origHeight;
  float baseScale = std::min(ratioX, ratioY) * 0.85f;

  float finalWidth = (float)origWidth * baseScale * zoom;
  float finalHeight = (float)origHeight * baseScale * zoom;
  ImVec2 size = ImVec2(finalWidth, finalHeight);

  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImVec2 centerPos;
  centerPos.x = pos.x + (availRegion.x - size.x) * 0.5f + pan.x;
  centerPos.y = pos.y + (availRegion.y - size.y) * 0.5f + pan.y;

  ImVec2 screenMax = ImVec2(pos.x + availRegion.x, pos.y + availRegion.y);

  bool isContentHovered = ImGui::IsMouseHoveringRect(pos, screenMax);

  if (isContentHovered && ImGui::GetIO().MouseWheel != 0.0f) {
    zoom += ImGui::GetIO().MouseWheel * 0.1f;
    zoom = std::max(0.1f, std::min(zoom, 10.0f));
  }

  if (isContentHovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    ImVec2 delta = ImGui::GetIO().MouseDelta;
    pan.x += delta.x;
    pan.y += delta.y;
  }

  if (isContentHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
    zoom = 1.0f;
    pan = ImVec2(0.0f, 0.0f);
  }

  ImGui::SetCursorScreenPos(centerPos);
  ImGui::Image(texId, size);
}

void EditorLayout::renderPreviewPanel() {
  ImGui::Begin("Image Preview");

  if (m_node_graph) {
    ImGui::Checkbox("Sync Viewport Zoom/Pan", &m_node_graph->viewSynced);
    ImGui::SameLine();
    if (ImGui::Button("Reset View")) {
      m_node_graph->previewZoom = 1.0f;
      m_node_graph->previewPan = ImVec2(0.0f, 0.0f);
      m_node_graph->outputZoom = 1.0f;
      m_node_graph->outputPan = ImVec2(0.0f, 0.0f);
    }

    if (m_node_graph->isPipelineProcessing()) {
      ImGui::SameLine();
      if ((int)(ImGui::GetTime() * 3.0f) % 2 == 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.0f, 1.0f),
                           "[ PROCESSING... ]");
      } else {
        ImGui::TextDisabled("[ PROCESSING... ]");
      }
    }
  }
  ImGui::Separator();

  float totalWidth = ImGui::GetContentRegionAvail().x;
  ImGui::Text("Original Image");
  ImGui::SameLine(totalWidth * 0.5f + 10.0f);
  ImGui::Text("Pixel Art Result");
  ImGui::Separator();

  ImGui::Columns(2, "PreviewColumns", false);

  ImTextureID inputTexId = 0;
  int imgW = 512;
  int imgH = 512;

  if (m_node_graph) {
    inputTexId = m_node_graph->getLatestInputTextureId();
    imgW = m_node_graph->getLatestInputWidth();
    imgH = m_node_graph->getLatestInputHeight();
  }

  if (inputTexId) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImGui::PushClipRect(pos, ImVec2(pos.x + avail.x, pos.y + avail.y), true);
    handleImageInteractionAndDraw(inputTexId, m_node_graph->previewZoom,
                                  m_node_graph->previewPan, imgW, imgH);
    ImGui::PopClipRect();
  } else {
    ImGui::Dummy(ImVec2(200, 200));
    ImGui::TextDisabled("(Drag & Drop an image into the graph)");
  }

  if (m_node_graph && m_node_graph->viewSynced) {
    m_node_graph->outputZoom = m_node_graph->previewZoom;
    m_node_graph->outputPan = m_node_graph->previewPan;
  }

  ImGui::NextColumn();

  if (m_node_graph) {
    ImTextureID outputTexId = m_node_graph->getLatestOutputTextureId();
    if (outputTexId) {
      ImVec2 pos = ImGui::GetCursorScreenPos();
      ImVec2 avail = ImGui::GetContentRegionAvail();
      ImGui::PushClipRect(pos, ImVec2(pos.x + avail.x, pos.y + avail.y), true);
      handleImageInteractionAndDraw(outputTexId, m_node_graph->outputZoom,
                                    m_node_graph->outputPan, imgW, imgH);
      ImGui::PopClipRect();
    } else {
      ImGui::Dummy(ImVec2(200, 200));
      ImGui::TextDisabled(
          "(Connect Input Image to Output Image Node to Preview)");
    }
  }

  ImGui::Columns(1);
  ImGui::End();
}
