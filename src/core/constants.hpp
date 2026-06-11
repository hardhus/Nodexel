#pragma once

#include "imgui.h"

namespace Nodexel::Constants {
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr const char *WINDOW_TITLE = "Nodexel Engine v0.1.0";

constexpr float LAYOUT_PREVIEW_RATIO = 0.32f;
constexpr float LAYOUT_CONTROL_PANEL_RATIO = 0.17f;

const ImVec2 DEFAULT_INPUT_POS(100.0f, 150.0f);
const ImVec2 DEFAULT_OUTPUT_POS(700.0f, 200.0f);
constexpr float NEW_NODE_OFFSET = 35.0f;

constexpr float PREVIEW_THUMB_SIZE = 256.0f;
} // namespace Nodexel::Constants
