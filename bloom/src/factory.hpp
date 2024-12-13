/**
 * @file factory.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "engine.hpp"
#include "object.hpp"

namespace bloom {

class Object;

class BLOOM_API Factory {

public:
  /**
   * Creates an Entity and does all the set-up processes that need to happen
   *
   * @tparam T Class to create an entity from
   * @return Created entity
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  T CreateObject();

private:
  unsigned int n_currentID = 0;
    
};

template<typename T, typename>
T Factory::CreateObject() {
  auto _obj = T(n_currentID);
  n_currentID++;
  return _obj;
}

}
