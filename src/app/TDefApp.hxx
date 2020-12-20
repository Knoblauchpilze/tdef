#ifndef    TDEF_APP_HXX
# define   TDEF_APP_HXX

# include "TDefApp.hh"

namespace tdef {

  inline
  void
  TDefApp::loadResources() {}

  inline
  void
  TDefApp::loadMenuResources() {}

  inline
  void
  TDefApp::cleanResources() {}

  inline
  void
  TDefApp::onStep(float elapsed) {
    if (m_world != nullptr) {
      m_world->step(elapsed);
    }
  }

  inline
  void
  TDefApp::onPause(float elapsed) {
    if (m_world != nullptr) {
      m_world->pause(elapsed);
    }
  }

  inline
  void
  TDefApp::onResume(float elapsed) {
    if (m_world != nullptr) {
      m_world->resume(elapsed);
    }
  }

  inline
  void
  TDefApp::drawSprite(const SpriteDesc& t, const CoordinateFrame& cf) {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);
    FillRectDecal(p, t.radius * cf.tileSize(), t.color);
  }

}

#endif    /* PGE_APP_HXX */
