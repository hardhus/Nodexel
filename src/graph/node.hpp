#pragma once

#include "core/types.hpp"
#include <string>
#include <vector>

class Node {
public:
  int id;
  std::string title;
  Nodexel::Types::NodeType type;
  std::vector<Nodexel::Types::Pin> inputs;
  std::vector<Nodexel::Types::Pin> outputs;

  Node(int id, const std::string &title, Nodexel::Types::NodeType type)
      : id(id), title(title), type(type) {}

  virtual ~Node() = default;
  virtual void draw() = 0;
};
