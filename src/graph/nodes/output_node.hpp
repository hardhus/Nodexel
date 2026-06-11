#pragma once

#include "graph/node.hpp"

class OutputImageNode : public Node {
public:
  OutputImageNode(int id, int inputPinId);
  ~OutputImageNode() override = default;

  void draw() override;
};
