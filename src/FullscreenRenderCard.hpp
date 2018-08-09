#pragma once

#include "MainApp.hpp"

class FullscreenRenderCard {
public:
  FullscreenRenderCard(const std::string &buffname, float alpha);
  ~FullscreenRenderCard();
  
  CardMaker* render_card = NULL;
  NodePath* render_node = NULL;
  Texture* render_tex = NULL;
private:
};
