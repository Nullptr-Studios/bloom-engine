/**
 * \file pipeline.h
 * \author Xein <xgonip@gmail.com>
 * \date 26/11/2024
 *
 * \brief [File description here]
 */

#pragma once

#include <bloom_header.hpp>

namespace bloom::render {

class Pipeline {
public:
  Pipeline(const std::string& vertPath, const std::string& fragPath);

private:
  static std::vector<char> ReadFile(const std::string& path);
  void CreatePipeline(const std::string& vertPath, const std::string& fragPath);

};

}
