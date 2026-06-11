#include "imgui.h"
#include "imnodes.h"
#include "input_node.hpp"
#include "pipeline/texture.hpp"
#include <memory>

InputImageNode::InputImageNode(int id, const std::string &path, int outputPinId)
    : Node(id, "Input Image", Nodexel::Types::NodeType::InputImage),
      filePath(path) {

  outputs.push_back(
      {outputPinId, "Image Data", Nodexel::Types::PinType::Output, id});

  texture = std::make_unique<Nodexel::Pipeline::Texture>();

  if (!filePath.empty() && filePath != "No file loaded") {
    size_t lastSlash = filePath.find_last_of("/\\");
    title = "Input: " + ((lastSlash == std::string::npos)
                             ? filePath
                             : filePath.substr(lastSlash + 1));

    texture->loadFromFile(filePath);
  } else {
    title = "Input: No File";
  }
}

void InputImageNode::draw() {
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::Text("%s", title.c_str());
  ImNodes::EndNodeTitleBar();

  if (ImGui::Button("Browse...")) {
    // TODO: cross-platform
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip(
        "WIP: Please drag and drop images directly into the window for now!");
  }

  ImGui::TextWrapped("Path: %s", filePath.c_str());

  if (texture && texture->isValid()) {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Resolution: %dx%d",
                       texture->getWidth(), texture->getHeight());
  }

  ImNodes::BeginOutputAttribute(outputs[0].id);
  ImGui::Text("%s", outputs[0].name.c_str());
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
}
