#pragma once

class EditorLayout {
public:
  EditorLayout();
  ~EditorLayout() = default;

  void render();

private:
  void renderControlPanel();
  void renderNodeGraph();
  void renderPreviewPanel();
};
