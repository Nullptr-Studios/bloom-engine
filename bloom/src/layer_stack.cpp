#include "layer_stack.hpp"

namespace bloom {

  LayerStack::LayerStack() {
    m_layerInsert = m_layers.begin();
  }

  LayerStack::~LayerStack() {
    for (Layer *layer : m_layers)
      delete layer;
  }

  void LayerStack::PushLayer(Layer *layer) {
    m_layerInsert = m_layers.emplace(m_layerInsert, layer);
  }

  void LayerStack::PushOverlay(Layer *overlay) {
    m_layers.emplace_back(overlay);
  }

  void LayerStack::PopLayer(Layer *layer) {
    auto i = std::find(m_layers.begin(), m_layers.end(), layer);
    if (i != m_layers.end()) {
      m_layers.erase(i);
      --m_layerInsert;
    }
  }

  void LayerStack::PopOverlay(Layer *overlay) {
    auto i = std::find(m_layers.begin(), m_layers.end(), overlay);
    if (i != m_layers.end())
      m_layers.erase(i);
  }

}
