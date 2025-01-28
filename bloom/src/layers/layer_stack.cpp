#include "layer_stack.hpp"
#include <bloom_header.hpp>

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
    ++m_layerInsert;
    layer->OnAttach();
  }

  void LayerStack::PushOverlay(Layer *overlay) {
    m_layers.emplace_back(overlay);
    overlay->OnAttach();
  }

  void LayerStack::PopLayer(Layer *layer) {
    layer->OnDetach();
    const auto i = std::ranges::find(m_layers, layer);
    delete layer;
    if (i != m_layers.end()) {
      m_layers.erase(i);
      --m_layerInsert;
    }
  }

  void LayerStack::PopOverlay(Layer *overlay) {
    overlay->OnDetach();
    if (const auto i = std::ranges::find(m_layers, overlay); i != m_layers.end())
      m_layers.erase(i);
  }

}
