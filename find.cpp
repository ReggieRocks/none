// Simple "find" for LinuxDir.txt using child/sibling links.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr int kIndentWidth = 5;

struct Node {
  std::string name;
  std::string path;
  bool is_dir = false;
  Node* parent = nullptr;
  Node* first_child = nullptr;
  Node* last_child = nullptr;
  Node* next_sibling = nullptr;
  Node* prev_sibling = nullptr;
};

std::string BaseName(const std::string& path) {
  if (path.empty()) {
    return "";
  }
  std::string trimmed = path;
  if (trimmed.size() > 1 && trimmed.back() == '/') {
    trimmed.pop_back();
  }
  const std::size_t pos = trimmed.find_last_of('/');
  if (pos == std::string::npos) {
    return trimmed;
  }
  if (pos + 1 >= trimmed.size()) {
    return "";
  }
  return trimmed.substr(pos + 1);
}

std::string ParentPath(const std::string& path) {
  if (path.empty()) {
    return "";
  }
  const std::size_t pos = path.find_last_of('/');
  if (pos == std::string::npos || pos == 0) {
    return "";
  }
  return path.substr(0, pos);
}

std::string JoinPath(const std::string& parent, const std::string& child) {
  if (parent.empty()) {
    return child;
  }
  if (!parent.empty() && parent.back() == '/') {
    return parent + child;
  }
  return parent + "/" + child;
}

std::string NormalizePath(std::string path) {
  while (!path.empty() && path.front() == '/') {
    path.erase(path.begin());
  }
  while (!path.empty() && path.back() == '/') {
    path.pop_back();
  }
  return path;
}

bool MatchesTarget(const std::string& path, const std::string& target) {
  if (target.empty()) {
    return true;
  }
  std::string normalized = NormalizePath(path);
  if (normalized.size() < target.size()) {
    return false;
  }
  if (normalized.compare(normalized.size() - target.size(), target.size(), target) != 0) {
    return false;
  }
  if (normalized.size() == target.size()) {
    return true;
  }
  return normalized[normalized.size() - target.size() - 1] == '/';
}

class DirectoryTree {
 public:
  Node* EnsureNode(const std::string& path, const std::string& name_hint, bool mark_dir) {
    auto it = path_index_.find(path);
    if (it != path_index_.end()) {
      Node* node = it->second;
      if (mark_dir) {
        node->is_dir = true;
      }
      if (node->name.empty() && !name_hint.empty()) {
        node->name = name_hint;
      }
      return node;
    }
    auto node = std::make_unique<Node>();
    node->name = name_hint;
    node->path = path;
    node->is_dir = mark_dir;
    Node* node_ptr = node.get();
    nodes_.push_back(std::move(node));
    path_index_[path] = node_ptr;
    return node_ptr;
  }

  void AttachChild(Node* parent, Node* child) {
    if (!parent || !child) {
      return;
    }
    if (child->parent == parent) {
      return;
    }
    if (child->parent != nullptr) {
      return;
    }
    child->parent = parent;
    if (!parent->first_child) {
      parent->first_child = child;
      parent->last_child = child;
      return;
    }
    parent->last_child->next_sibling = child;
    child->prev_sibling = parent->last_child;
    parent->last_child = child;
  }

  void LinkToParent(Node* node) {
    if (!node || node->path.empty()) {
      return;
    }
    std::string parent_path = ParentPath(node->path);
    if (parent_path.empty()) {
      return;
    }
    Node* parent = EnsureNode(parent_path, BaseName(parent_path), true);
    AttachChild(parent, node);
  }

  std::vector<Node*> Roots() const {
    std::vector<Node*> roots;
    for (const auto& entry : nodes_) {
      if (entry->parent == nullptr) {
        roots.push_back(entry.get());
      }
    }
    std::sort(roots.begin(), roots.end(),
              [](const Node* left, const Node* right) { return left->path < right->path; });
    return roots;
  }

  const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes_; }

 private:
  std::vector<std::unique_ptr<Node>> nodes_;
  std::unordered_map<std::string, Node*> path_index_;
};

void PrintTree(Node* node, int depth, bool show_full_path) {
  if (!node) {
    return;
  }
  if (show_full_path) {
    std::string output = node->path;
    if (!output.empty() && output.back() != '/') {
      output.push_back('/');
    }
    std::cout << output << "\n";
  } else {
    std::cout << std::string(depth * kIndentWidth, ' ') << node->name << "\n";
  }

  for (Node* child = node->first_child; child != nullptr; child = child->next_sibling) {
    PrintTree(child, depth + 1, false);
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  std::ifstream input("LinuxDir.txt");
  if (!input) {
    std::cerr << "Error: LinuxDir.txt not found in the current directory.\n";
    return 1;
  }

  DirectoryTree tree;
  std::string line;
  Node* current_dir = nullptr;
  std::string current_path;

  while (std::getline(input, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty()) {
      current_dir = nullptr;
      current_path.clear();
      continue;
    }
    if (line.back() == ':') {
      current_path = line.substr(0, line.size() - 1);
      current_dir = tree.EnsureNode(current_path, BaseName(current_path), true);
      tree.LinkToParent(current_dir);
      continue;
    }
    if (!current_dir) {
      continue;
    }
    std::string child_path = JoinPath(current_path, line);
    Node* child = tree.EnsureNode(child_path, line, false);
    tree.AttachChild(current_dir, child);
  }

  std::string target;
  if (argc >= 2) {
    target = argv[1];
  }
  target = NormalizePath(target);

  std::vector<Node*> matches;
  if (target.empty()) {
    matches = tree.Roots();
  } else {
    for (const auto& entry : tree.Nodes()) {
      if (MatchesTarget(entry->path, target)) {
        matches.push_back(entry.get());
      }
    }
    std::sort(matches.begin(), matches.end(),
              [](const Node* left, const Node* right) { return left->path < right->path; });
  }

  if (matches.empty()) {
    std::cerr << "Directory not found: " << (target.empty() ? "<root>" : target) << "\n";
    return 0;
  }

  for (std::size_t i = 0; i < matches.size(); ++i) {
    PrintTree(matches[i], 0, true);
    if (i + 1 < matches.size()) {
      std::cout << "\n";
    }
  }

  return 0;
}
