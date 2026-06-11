#pragma once

#include "graph/node.hpp"
#include "pipeline/texture.hpp"
#include <memory>
#include <string>

class InputImageNode : public Node {
public:
  std::string filePath;
  std::unique_ptr<Nodexel::Pipeline::Texture> texture;

  InputImageNode(int id, const std::string &path, int outputPinId);
  ~InputImageNode() override = default;

  void draw() override;
};
