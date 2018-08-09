#include "FullscreenRenderCard.hpp"

FullscreenRenderCard::FullscreenRenderCard(const std::string &buffname, float alpha) {
  render_card = new CardMaker(buffname);
  render_card->set_frame_fullscreen_quad();
  
  render_node = new NodePath(render_card->generate());
  render_node->set_transparency(TransparencyAttrib::M_alpha);
  render_node->set_color(1, 1, 1, alpha);
  render_node->reparent_to(M_A->window->get_render_2d());

  render_tex = new Texture(buffname);
  render_node->set_texture(render_tex);
}

FullscreenRenderCard::~FullscreenRenderCard() {
  render_node->hide();
  render_node->clear();
  render_node->remove_node();
  delete render_node;
}
