#include "imgui.h"
#include "imnodes.h"
#include "output_node.hpp"

OutputImageNode::OutputImageNode(int id, int inputPinId)
    : Node(id, "Output Image", Nodexel::Types::NodeType::OutputImage) {
  inputs.push_back(
      {inputPinId, "Final Result", Nodexel::Types::PinType::Input, id});
}

void OutputImageNode::draw() {
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::Text("%s", title.c_str());
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(inputs[0].id);
  ImGui::Text("%s", inputs[0].name.c_str());
  ImNodes::EndInputAttribute();

  ImNodes::EndNode();
}
