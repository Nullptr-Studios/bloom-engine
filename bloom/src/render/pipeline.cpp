#include "pipeline.hpp"
#include <fstream>

namespace bloom::render {

Pipeline::Pipeline(const std::string &vertPath, const std::string &fragPath) { CreatePipeline(vertPath, fragPath); }

std::vector<char> Pipeline::ReadFile(const std::string &path) { 
	std::ifstream file{path, std::ios::ate | std::ios::binary};
  
  // Check if we can open the file
  if (!file.is_open()) 
    //BLOOM_ERROR("Failed to open file {0}", path);

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.close();
  return buffer;
}

void Pipeline::CreatePipeline(const std::string &vertPath, const std::string &fragPath) {
  auto vertShader = ReadFile(vertPath);
  auto fragShader = ReadFile(fragPath);

  BLOOM_LOG("Vertex Shader Size: {0}", vertShader.size());
  BLOOM_LOG("Fragment Shader Size: {0}", fragShader.size());
}

}
