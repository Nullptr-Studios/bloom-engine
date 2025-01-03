/**
 * @file resource_loader.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 30/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <fstream>
#include <future>
#include <string>

namespace bloom {

class ResourceLoader {
public:
  static std::future<std::string> LoadFileAsync(const std::string& path);

private:
  static std::string LoadFile(const std::string& path);

};

}
