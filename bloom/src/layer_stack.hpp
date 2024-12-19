/**
 * @file layer_stack.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 19/12/2024
 *
 * @brief This file defines the LayerStack class
 */

#pragma once
#include "layer.hpp"
#include <bloom_header.hpp>

namespace bloom {

/**
 * @class LayerStack
 * @brief Manages a stack of layers and overlays
 *
 * The @c LayerStack class is responsible for maintaining a stack of layers and overlays,
 * providing methods to push and pop layers and overlays, and iterating over the stack.
 */
class LayerStack {
public:
  LayerStack();
  ~LayerStack();

  /**
   * @brief Pushes a layer onto the stack
   * @param layer Pointer to the layer to be pushed
   */
  void PushLayer(Layer* layer);
  /**
   * @brief Pushes an overlay onto the stack
   * @param overlay Pointer to the overlay to be pushed
   */
  void PushOverlay(Layer* overlay);

  /**
   * @brief Pops a layer from the stack
   * @param layer Pointer to the layer to be popped
   */
  void PopLayer(Layer* layer);
  /**
   * @brief Pops an overlay from the stack
   * @param overlay Pointer to the overlay to be popped
   */
  void PopOverlay(Layer* overlay);

  /**
   * @brief Returns an iterator to the beginning of the layer stack
   * @return Iterator to the beginning of the layer stack
   */
  std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
  /**
   * @brief Returns an iterator to the end of the layer stack
   * @return Iterator to the end of the layer stack
   */
  std::vector<Layer*>::iterator end() { return m_layers.end(); }

private:
  std::vector<Layer*> m_layers; ///< Vector of pointers to layers.
  std::vector<Layer*>::iterator m_layerInsert; ///< Iterator to the layer insertion point.
};

}
