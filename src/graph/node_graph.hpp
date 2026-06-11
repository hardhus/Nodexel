#pragma once

#include "core/types.hpp"
#include "graph/node.hpp"
#include "pipeline/pipeline_manager.hpp"
#include <memory>
#include <string>
#include <vector>

class NodeGraph {
public:
  NodeGraph();
  ~NodeGraph() = default;

  void addInputNode(const std::string &filePath, float posX, float posY);
  void addNode(Nodexel::Types::NodeType type, float posX, float posY);
  void initDefaultLayout();
  void drawGraph();

  ImTextureID getLatestInputTextureId();
  ImTextureID getLatestOutputTextureId();

  int getLatestInputWidth();
  int getLatestInputHeight();

  void updatePipeline();

  bool isPipelineProcessing() {
    return m_pipeline_manager && m_pipeline_manager->isProcessing();
  }

  float previewZoom = 1.0f;
  ImVec2 previewPan{0.0f, 0.0f};
  float outputZoom = 1.0f;
  ImVec2 outputPan{0.0f, 0.0f};
  bool viewSynced = false;

  int activeInputNodeId = 0;
  int activeOutputNodeId = 0;
  int currentlySelectedNodeId = 0;

private:
  std::vector<std::shared_ptr<Node>> m_nodes;
  std::vector<Nodexel::Types::Link> m_links;

  std::unique_ptr<Nodexel::Pipeline::PipelineManager> m_pipeline_manager;

  int m_current_id = 1;
  int getNextId() { return m_current_id++; }

  bool m_open_add_menu = false;
  bool m_open_delete_modal = false;
  bool m_open_link_delete_modal = false;
  int m_node_to_delete = 0;
  int m_link_to_delete = 0;
  ImVec2 m_menu_spawn_pos{0.0f, 0.0f};

  void handleContextMenu();
  void handleKeyboardShortcuts();
  void handleDeleteModal();
};
