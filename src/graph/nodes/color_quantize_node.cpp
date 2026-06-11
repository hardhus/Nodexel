#include "color_quantize_node.hpp"
#include "imgui.h"
#include "imnodes.h"

ColorQuantizeNode::ColorQuantizeNode(int id, int inputPinId, int outputPinId)
    : Node(id, "Color Quantize", Nodexel::Types::NodeType::ColorQuantize) {

  inputs.push_back(
      {inputPinId, "Input Image", Nodexel::Types::PinType::Input, id});
  outputs.push_back(
      {outputPinId, "Output Image", Nodexel::Types::PinType::Output, id});
}

void ColorQuantizeNode::draw() {
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::Text("%s", title.c_str());
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(inputs[0].id);
  ImGui::Text("%s", inputs[0].name.c_str());
  ImNodes::EndInputAttribute();

  ImGui::Spacing();

  ImGui::PushItemWidth(120.0f);
  ImGui::SliderInt("Colors", &colorCount, 2, 32, "%d Colors");
  ImGui::PopItemWidth();

  ImGui::Spacing();

  ImNodes::BeginOutputAttribute(outputs[0].id);
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40.0f);
  ImGui::Text("%s", outputs[0].name.c_str());
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
}
