#pragma once

#include "graph/node.hpp"

class ColorQuantizeNode : public Node {
public:
  int colorCount = 8;

  ColorQuantizeNode(int id, int inputPinId, int outputPinId);
  ~ColorQuantizeNode() override = default;

  void draw() override;
};
