#pragma once

#include "imgui.h"
#include <string>

namespace Nodexel::Types {

enum class PinType { Input, Output };

enum class NodeType {
  InputImage,
  Pixelate,
  ColorQuantize,
  Rescale,
  OutputImage
};

struct Pin {
  int id;
  std::string name;
  PinType type;
  int parentNodeId;
};

struct Link {
  int id;
  int fromPinId;
  int toPinId;
};

struct NodeTransform {
  ImVec2 position;
  bool needsSync = true;
};
} // namespace Nodexel::Types
