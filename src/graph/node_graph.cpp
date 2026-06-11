#include "core/constants.hpp"
#include "graph/nodes/color_quantize_node.hpp"
#include "graph/nodes/input_node.hpp"
#include "graph/nodes/output_node.hpp"
#include "graph/nodes/pixelate_node.hpp"
#include "imgui.h"
#include "imnodes.h"
#include "node_graph.hpp"
#include <algorithm>
#include <iostream>

NodeGraph::NodeGraph() {
  m_pipeline_manager = std::make_unique<Nodexel::Pipeline::PipelineManager>();
}

void NodeGraph::addInputNode(const std::string &filePath, float posX,
                             float posY) {
  int node_id = getNextId();
  int pin_id = getNextId();

  auto inputNode = std::make_shared<InputImageNode>(node_id, filePath, pin_id);
  m_nodes.push_back(inputNode);

  activeInputNodeId = node_id;

  ImNodes::SetNodeEditorSpacePos(node_id, ImVec2(posX, posY));
}

void NodeGraph::addNode(Nodexel::Types::NodeType type, float posX, float posY) {
  int node_id = getNextId();

  if (type == Nodexel::Types::NodeType::OutputImage) {
    int pin_id = getNextId();
    auto outputNode = std::make_shared<OutputImageNode>(node_id, pin_id);
    m_nodes.push_back(outputNode);
    activeOutputNodeId = node_id;
  }

  ImNodes::SetNodeEditorSpacePos(node_id, ImVec2(posX, posY));
}

void NodeGraph::initDefaultLayout() {
  std::cout << "[INFO] NodeGraph::initDefaultLayout() executed. Creating data "
               "objects."
            << std::endl;

  int input_id = getNextId();
  int input_pin = getNextId();
  m_nodes.push_back(
      std::make_shared<InputImageNode>(input_id, "No file loaded", input_pin));
  activeInputNodeId = input_id;

  int output_id = getNextId();
  int output_pin = getNextId();
  m_nodes.push_back(std::make_shared<OutputImageNode>(output_id, output_pin));
  activeOutputNodeId = output_id;
}

ImTextureID NodeGraph::getLatestInputTextureId() {
  for (auto &node : m_nodes) {
    if (node->id == activeInputNodeId &&
        node->type == Nodexel::Types::NodeType::InputImage) {
      auto inputNode = std::static_pointer_cast<InputImageNode>(node);
      if (inputNode && inputNode->texture && inputNode->texture->isValid()) {
        return inputNode->texture->getTextureId();
      }
    }
  }
  return 0;
}

ImTextureID NodeGraph::getLatestOutputTextureId() {
  if (m_pipeline_manager && m_pipeline_manager->getOutputTexture()) {
    auto outTex = m_pipeline_manager->getOutputTexture();
    if (outTex->isValid()) {
      return outTex->getTextureId();
    }
  }
  return 0;
}

int NodeGraph::getLatestInputWidth() {
  for (auto &node : m_nodes) {
    if (node->id == activeInputNodeId &&
        node->type == Nodexel::Types::NodeType::InputImage) {
      auto inputNode = std::static_pointer_cast<InputImageNode>(node);
      if (inputNode && inputNode->texture)
        return inputNode->texture->getWidth();
    }
  }
  return 512;
}

int NodeGraph::getLatestInputHeight() {
  for (auto &node : m_nodes) {
    if (node->id == activeInputNodeId &&
        node->type == Nodexel::Types::NodeType::InputImage) {
      auto inputNode = std::static_pointer_cast<InputImageNode>(node);
      if (inputNode && inputNode->texture)
        return inputNode->texture->getHeight();
    }
  }
  return 512;
}

void NodeGraph::drawGraph() {
  if (m_pipeline_manager) {
    m_pipeline_manager->updateTextures();
  }

  ImNodes::BeginNodeEditor();

  static bool positions_initialized = false;
  if (!positions_initialized && m_nodes.size() >= 2) {
    positions_initialized = true;
    std::cout << "[INFO] Context is active. Setting initial space positions."
              << std::endl;
    ImNodes::SetNodeEditorSpacePos(m_nodes[0]->id,
                                   Nodexel::Constants::DEFAULT_INPUT_POS);
    ImNodes::SetNodeEditorSpacePos(m_nodes[1]->id,
                                   Nodexel::Constants::DEFAULT_OUTPUT_POS);
  }

  int current_frame_selected_node = 0;
  for (auto &node : m_nodes) {
    node->draw();
    if (ImNodes::IsNodeSelected(node->id)) {
      current_frame_selected_node = node->id;
      if (node->type == Nodexel::Types::NodeType::InputImage)
        activeInputNodeId = node->id;
      if (node->type == Nodexel::Types::NodeType::OutputImage)
        activeOutputNodeId = node->id;
    }
  }

  if (current_frame_selected_node != 0) {
    currentlySelectedNodeId = current_frame_selected_node;
  }

  for (const auto &link : m_links) {
    ImNodes::Link(link.id, link.fromPinId, link.toPinId);
  }

  if (ImNodes::IsEditorHovered() &&
      ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    m_open_add_menu = true;
    m_menu_spawn_pos = ImGui::GetMousePos();
  }

  ImNodes::EndNodeEditor();

  if (m_open_add_menu) {
    ImGui::OpenPopup("GraphContextMenu");
    m_open_add_menu = false;
  }

  handleContextMenu();
  handleKeyboardShortcuts();
  handleDeleteModal();

  int start_pin, end_pin;
  if (ImNodes::IsLinkCreated(&start_pin, &end_pin)) {
    if (start_pin > 0 && end_pin > 0) {
      bool link_already_exists = false;
      for (const auto &existing_link : m_links) {
        if (existing_link.fromPinId == start_pin &&
            existing_link.toPinId == end_pin) {
          link_already_exists = true;
          break;
        }
      }

      if (!link_already_exists) {
        m_links.push_back({getNextId(), start_pin, end_pin});
        std::cout << "[INFO] Valid link created between Pin " << start_pin
                  << " and Pin " << end_pin << std::endl;
        updatePipeline();
      }
    }
  }
}

void NodeGraph::handleContextMenu() {
  if (ImGui::BeginPopup("GraphContextMenu")) {
    if (ImGui::MenuItem("Add Input Image Node")) {
      addInputNode("No file loaded", m_menu_spawn_pos.x, m_menu_spawn_pos.y);
    }

    if (ImGui::MenuItem("Add Pixelate Filter")) {
      int node_id = getNextId();
      int in_pin = getNextId();
      int out_pin = getNextId();
      m_nodes.push_back(
          std::make_shared<PixelateNode>(node_id, in_pin, out_pin));
      ImNodes::SetNodeEditorSpacePos(node_id, m_menu_spawn_pos);
    }

    if (ImGui::MenuItem("Add Color Quantize Filter")) {
      int node_id = getNextId();
      int in_pin = getNextId();
      int out_pin = getNextId();
      m_nodes.push_back(
          std::make_shared<ColorQuantizeNode>(node_id, in_pin, out_pin));
      ImNodes::SetNodeEditorSpacePos(node_id, m_menu_spawn_pos);
    }

    if (ImGui::MenuItem("Add Output Image Node")) {
      addNode(Nodexel::Types::NodeType::OutputImage, m_menu_spawn_pos.x,
              m_menu_spawn_pos.y);
    }
    ImGui::EndPopup();
  }
}

void NodeGraph::handleKeyboardShortcuts() {
  if (!ImGui::IsAnyItemActive()) {

    if (ImGui::IsKeyPressed(ImGuiKey_A)) {
      m_open_add_menu = true;
      m_menu_spawn_pos = ImGui::GetMousePos();
    }

    if (ImNodes::NumSelectedLinks() > 0) {
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) ||
          ImGui::IsKeyPressed(ImGuiKey_X)) {
        std::vector<int> selected_links(ImNodes::NumSelectedLinks());
        ImNodes::GetSelectedLinks(selected_links.data());
        int target_link = selected_links[0];

        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) ||
            ImGui::IsKeyDown(ImGuiKey_RightShift)) {
          for (int link_id : selected_links) {
            m_links.erase(
                std::remove_if(m_links.begin(), m_links.end(),
                               [link_id](const Nodexel::Types::Link &link) {
                                 return link.id == link_id;
                               }),
                m_links.end());
          }
          ImNodes::ClearLinkSelection();
          updatePipeline();
        } else {
          m_link_to_delete = target_link;
          m_open_link_delete_modal = true;
        }
      }
    } else if (currentlySelectedNodeId != 0) {
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) ||
          ImGui::IsKeyPressed(ImGuiKey_X)) {

        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) ||
            ImGui::IsKeyDown(ImGuiKey_RightShift)) {

          std::vector<int> targetPinIds;
          for (auto &node : m_nodes) {
            if (node->id == currentlySelectedNodeId) {
              for (const auto &p : node->inputs)
                targetPinIds.push_back(p.id);
              for (const auto &p : node->outputs)
                targetPinIds.push_back(p.id);
              break;
            }
          }

          m_links.erase(
              std::remove_if(
                  m_links.begin(), m_links.end(),
                  [&targetPinIds](const Nodexel::Types::Link &link) {
                    return std::find(targetPinIds.begin(), targetPinIds.end(),
                                     link.fromPinId) != targetPinIds.end() ||
                           std::find(targetPinIds.begin(), targetPinIds.end(),
                                     link.toPinId) != targetPinIds.end();
                  }),
              m_links.end());

          m_nodes.erase(
              std::remove_if(m_nodes.begin(), m_nodes.end(),
                             [this](const std::shared_ptr<Node> &node) {
                               return node->id == currentlySelectedNodeId;
                             }),
              m_nodes.end());

          currentlySelectedNodeId = 0;
          ImNodes::ClearNodeSelection();
          ImNodes::ClearLinkSelection();
          updatePipeline();
        } else {
          m_node_to_delete = currentlySelectedNodeId;
          m_open_delete_modal = true;
        }
      }
    }
  }
}

void NodeGraph::handleDeleteModal() {
  if (m_open_delete_modal) {
    ImGui::OpenPopup("Delete Node?");
    m_open_delete_modal = false;
  }

  if (ImGui::BeginPopupModal("Delete Node?", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Are you sure you want to delete this node?");
    ImGui::Separator();

    if (ImGui::Button("Yes", ImVec2(120, 0))) {
      m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(),
                                   [this](const std::shared_ptr<Node> &node) {
                                     return node->id == m_node_to_delete;
                                   }),
                    m_nodes.end());
      if (currentlySelectedNodeId == m_node_to_delete)
        currentlySelectedNodeId = 0;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  if (m_open_link_delete_modal) {
    ImGui::OpenPopup("Disconnect Link?");
    m_open_link_delete_modal = false;
  }

  if (ImGui::BeginPopupModal("Disconnect Link?", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Are you sure you want to disconnect this link?");
    ImGui::Separator();

    if (ImGui::Button("Yes", ImVec2(120, 0))) {
      m_links.erase(std::remove_if(m_links.begin(), m_links.end(),
                                   [this](const Nodexel::Types::Link &link) {
                                     return link.id == m_link_to_delete;
                                   }),
                    m_links.end());
      ImNodes::ClearLinkSelection();
      std::cout << "[INFO] Link " << m_link_to_delete
                << " disconnected via safe modal confirmation." << std::endl;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("No", ImVec2(120, 0))) {
      ImNodes::ClearLinkSelection();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void NodeGraph::updatePipeline() {
  std::shared_ptr<Node> activeOutput = nullptr;
  for (auto &node : m_nodes) {
    if (node->id == activeOutputNodeId &&
        node->type == Nodexel::Types::NodeType::OutputImage) {
      activeOutput = node;
      break;
    }
  }
  if (!activeOutput || activeOutput->inputs.empty())
    return;

  std::string sourceFilePath = "";
  int blockSize = 1;
  int colorCount = 8;
  bool useQuantize = false;

  int currentInputPinId = activeOutput->inputs[0].id;
  bool traceCompleted = false;

  for (int step = 0; step < 10; ++step) {
    int foundFromPinId = -1;

    for (const auto &link : m_links) {
      if (link.toPinId == currentInputPinId) {
        foundFromPinId = link.fromPinId;
        break;
      }
    }
    if (foundFromPinId == -1)
      break;

    std::shared_ptr<Node> parentNode = nullptr;
    for (auto &node : m_nodes) {
      for (const auto &outPin : node->outputs) {
        if (outPin.id == foundFromPinId) {
          parentNode = node;
          break;
        }
      }
      if (parentNode)
        break;
    }
    if (!parentNode)
      break;

    if (parentNode->type == Nodexel::Types::NodeType::InputImage) {
      auto inputNode = std::static_pointer_cast<InputImageNode>(parentNode);
      if (inputNode && !inputNode->filePath.empty() &&
          inputNode->filePath != "No file loaded") {
        sourceFilePath = inputNode->filePath;
        traceCompleted = true;
      }
      break;
    } else if (parentNode->type == Nodexel::Types::NodeType::Pixelate) {
      auto pixNode = std::static_pointer_cast<PixelateNode>(parentNode);
      if (pixNode)
        blockSize = pixNode->blockSize;

      if (!parentNode->inputs.empty())
        currentInputPinId = parentNode->inputs[0].id;
    } else if (parentNode->type == Nodexel::Types::NodeType::ColorQuantize) {
      auto quantNode = std::static_pointer_cast<ColorQuantizeNode>(parentNode);
      if (quantNode) {
        colorCount = quantNode->colorCount;
        useQuantize = true;
      }
      if (!parentNode->inputs.empty())
        currentInputPinId = parentNode->inputs[0].id;
    }
  }

  if (traceCompleted && !sourceFilePath.empty()) {
    m_pipeline_manager->triggerProcess(sourceFilePath, blockSize, colorCount,
                                       useQuantize);
  }
}
