#pragma once

#include "graph/node_graph.hpp"
#include <memory>

class EditorLayout {
public:
  EditorLayout();
  ~EditorLayout();

  void render();
  NodeGraph *getNodeGraph() { return m_node_graph.get(); }

private:
  std::unique_ptr<NodeGraph> m_node_graph;

  void renderControlPanel();
  void renderNodeGraph();
  void renderPreviewPanel();
};
