#pragma once

#include "graph/node.hpp"

class PixelateNode : public Node {
public:
  int blockSize = 8;

  PixelateNode(int id, int inputPinId, int outputPinId);
  ~PixelateNode() override = default;

  void draw() override;
};
