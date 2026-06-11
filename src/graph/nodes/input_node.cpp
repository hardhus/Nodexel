#include "imgui.h"
#include "imnodes.h"
#include "input_node.hpp"
#include "pipeline/texture.hpp"
#include "portable-file-dialogs.h"
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
    auto selection = pfd::open_file("Select an image to load", "",
                                    {"Image Files (.png .jpg .jpeg)",
                                     "*.png *.jpg *.jpeg", "All Files", "*"})
                         .result();

    if (!selection.empty()) {
      filePath = selection[0];

      size_t lastSlash = filePath.find_last_of("/\\");
      title = "Input: " + ((lastSlash == std::string::npos)
                               ? filePath
                               : filePath.substr(lastSlash + 1));

      texture->loadFromFile(filePath);
    }
  }

  std::string fileName = "No File Loaded";
  if (!filePath.empty() && filePath != "No file loaded") {
    size_t lastSlash = filePath.find_last_of("/\\");
    fileName = (lastSlash == std::string::npos)
                   ? filePath
                   : filePath.substr(lastSlash + 1);
  }

  ImGui::Text("File: ");
  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "%s", fileName.c_str());

  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(filePath.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }

  if (texture && texture->isValid()) {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Resolution: %dx%d",
                       texture->getWidth(), texture->getHeight());
  }

  ImNodes::BeginOutputAttribute(outputs[0].id);
  ImGui::Text("%s", outputs[0].name.c_str());
  ImNodes::EndOutputAttribute();

  ImNodes::EndNode();
}
