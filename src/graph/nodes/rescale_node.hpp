#pragma once

#include "graph/node.hpp"

class RescaleNode : public Node {
public:
  bool keepRatio = true;
  int inputMode = 0;

  int percentage = 50;
  int customWidth = 256;
  int customHeight = 256;

  int currentSrcWidth = 512;
  int currentSrcHeight = 512;

  RescaleNode(int id, int inputPinId, int outputPinId);
  ~RescaleNode() override = default;

  void draw() override;
};
