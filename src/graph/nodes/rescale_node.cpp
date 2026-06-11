#include "imgui.h"
#include "imnodes.h"
#include "rescale_node.hpp"
#include <algorithm>

RescaleNode::RescaleNode(int id, int inputPinId, int outputPinId)
    : Node(id, "Nearest Neighbor Rescale", Nodexel::Types::NodeType::Rescale) {

  inputs.push_back(
      {inputPinId, "Input Image", Nodexel::Types::PinType::Input, id});
  outputs.push_back(
      {outputPinId, "Output Image", Nodexel::Types::PinType::Output, id});
}

void RescaleNode::draw() {
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::Text("%s", title.c_str());
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginInputAttribute(inputs[0].id);
  ImGui::Text("%s", inputs[0].name.c_str());
  ImNodes::EndInputAttribute();

  ImGui::Spacing();
  ImGui::PushItemWidth(140.0f);

  ImGui::Checkbox("Keep Ratio", &keepRatio);

  const char *modes[] = {"Percentage (%)", "Absolute (px)"};
  ImGui::Combo("Mode", &inputMode, modes, 2);

  ImGui::Separator();

  if (inputMode == 0) {
    if (ImGui::SliderInt("Scale", &percentage, 10, 400, "%d%%")) {
      if (keepRatio) {
        customWidth = std::max(
            1, static_cast<int>(currentSrcWidth * (percentage / 100.0f)));
        customHeight = std::max(
            1, static_cast<int>(currentSrcHeight * (percentage / 100.0f)));
      }
    }
  } else {
    if (ImGui::InputInt("Width", &customWidth)) {
      customWidth = std::max(1, customWidth);
      if (keepRatio && currentSrcWidth > 0) {
        float aspect = static_cast<float>(currentSrcHeight) / currentSrcWidth;
        customHeight = std::max(1, static_cast<int>(customWidth * aspect));
      }
    }

    if (ImGui::InputInt("Height", &customHeight)) {
      customHeight = std::max(1, customHeight);
      if (keepRatio && currentSrcHeight > 0) {
        float aspect = static_cast<float>(currentSrcWidth) / currentSrcHeight;
        customWidth = std::max(1, static_cast<int>(customHeight * aspect));
      }
    }
  }

  ImGui::PopItemWidth();
  ImGui::Spacing();

  ImNodes::BeginOutputAttribute(outputs[0].id);
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50.0f);
  ImGui::Text("%s", outputs[0].name.c_str());
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
}
