#include "resource_loader.hpp"

namespace bloom {

std::future<std::string> ResourceLoader::LoadFileAsync(const std::string &path) {
  return std::async(std::launch::async, LoadFile, path);
}

std::string ResourceLoader::LoadFile(const std::string &path) {
  // Open the file
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    BLOOM_ERROR("Unable to open file: {0}", path);
    return "";
  }

  // Read the file into a string
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return content;
}

}
