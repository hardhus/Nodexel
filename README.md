# 🚀 Nodexel

A high-performance, asynchronous node-based engine designed for real-time retro pixel art generation, palette mapping, and creative shading effects. Built with C++17 and OpenGL3, Nodexel completely decouples heavy image manipulation from the UI thread to ensure an uninterrupted, hardware-accelerated creative workflow.

---

## 🛠️ Core Features

* **Asynchronous Processing Pipeline:** Process massive high-resolution imagery on background threads (`std::thread`) using thread-safe double buffering (`std::mutex`). The UI never drops frames, stays responsive at 60+ FPS even during heavy calculations.
* **Topological Graph Traversal:** Features an automated reverse-dependency tracer starting from the output image node back to the root source, dynamically gathering parameter states across active connections.
* **Ergonomic Node & Link Mechanics:** Seamless graph interactions managed via bounded priority hotkeys (`A` to add, `X` to safely disconnect/delete with confirmation modals, and `Shift + X` for instant cascading pruning of orphaned connections).
* **Sharp Pixel Fidelity Viewport:** Enforces strict window boundaries using isolated viewport clipping (`ImGui::PushClipRect`) and crisp nearest-neighbor texture filtering (`GL_NEAREST`) to prevent pixel blur during high zooms.

---

## 🗺️ Roadmap & Features Matrix

### 📦 Core Engine & Framework Upgrades
- [x] Multithreaded Asynchronous Pipeline Engine (`PipelineManager`)
- [x] Multi-window Docking Layout (Control Panel, Node Editor, Previews)
- [x] Native OS Drag & Drop Image Injection Interface
- [x] Safe Cascading Link Pruning on Node Destruction
- [ ] **Global Undo/Redo System:** Command-pattern based `Ctrl + Z` and `Ctrl + Y` history management for graph modifications, parameters, and link states.
- [ ] **Node Output Cache:** Memory-mapped sub-graph caching to skip reprocessing for nodes whose ancestors haven't changed.
- [ ] **Texture Export Engine:** Dedicated background worker to serialize and export high-fidelity pixel art directly to local disk (PNG/BMP).

### 🧪 Pixel Shading & Effects Nodes
- [x] **Pixelate Node:** Real-time block-size sampling adjustment slider.
- [x] **Color Quantize Node:** Color depth reduction matrix for retro bit-depth simulation.
- [x] **Nearest Neighbor Rescale Node:** Precision sprite resizing tool with aspect ratio locks and pixel/percentage mapping.
- [ ] **Dithering Shader Node:** Ordered Bayer matrices and error-diffusion (Floyd-Steinberg) dithering for authentic retro texturing.
- [ ] **Outline & Edge Detection Node:** Sobel/Scharr filter mapping to apply crisp contours onto pixelized shapes.
- [ ] **Palettization Node:** Custom palette loading support (.GPL or Hex files) to force images into classic retro hardware constraints (e.g., GameBoy, NES, Pico-8).
- [ ] **Procedural Noise Input Node:** Custom Perlin, Simplex, and Cellular noise generators to synthesize patterns entirely inside the node tree.
- [ ] **CRT / Scanline Post-Processing Node:** Post-process CRT monitor warp, scanline emulation, and phosphor bleed simulation.

---

## 🚀 Quick Start

### Prerequisites
Ensure you have a modern C++17 compliant compiler (MinGW-w64 on Windows, GCC/Clang on Linux) and CMake installed.

### Build and Run with `just`
The project ships with a `justfile` for automated workflow management:

```bash
# Initialize CMake and pull external dependencies
just setup

# Build the project using CMake
just build

# Execute the compiled binary
just run
