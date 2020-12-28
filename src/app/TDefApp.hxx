#ifndef    TDEF_APP_HXX
# define   TDEF_APP_HXX

# include "TDefApp.hh"
# include "Action.hh"

namespace tdef {

  inline
  void
  TDefApp::loadResources() {
    sprites::Pack p;

    p.file = "data/img/towers.png";
    p.sSize = olc::vi2d(64, 64);
    p.layout = olc::vi2d(4, 1);
    m_tPackID = m_packs->registerPack(p);

    p.file = "data/img/walls.png";
    p.sSize = olc::vi2d(64, 64);
    p.layout = olc::vi2d(1, 1);
    m_wPackID = m_packs->registerPack(p);
  }

  inline
  void
  TDefApp::cleanResources() {
    if (m_packs != nullptr) {
      m_packs.reset();
    }
  }

  inline
  void
  TDefApp::cleanMenuResources() {
    if (m_sMenu != nullptr) {
      m_sMenu.reset();
    }
    if (m_tMenu != nullptr) {
      m_tMenu.reset();
    }
    if (m_uMenu != nullptr) {
      m_uMenu.reset();
    }
  }

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
  TDefApp::onInputs(const controls::State& c) {
    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    if (m_sMenu != nullptr) {
      m_sMenu->processUserInput(c, actions);
    }
    if (m_tMenu != nullptr) {
      m_tMenu->processUserInput(c, actions);
    }
    if (m_uMenu != nullptr) {
      m_uMenu->processUserInput(c, actions);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_world);
    }
  }

  inline
  void
  TDefApp::drawSprite(const SpriteDesc& t, const CoordinateFrame& cf) {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);

    m_packs->draw(this, t.sprite, p, t.radius * cf.tileScale());
  }

  inline
  void
  TDefApp::drawRect(const SpriteDesc& t,
                    const CoordinateFrame& cf)
  {
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);
    FillRectDecal(p, t.radius * cf.tileSize(), t.sprite.tint);
  }

  inline
  void
  TDefApp::drawHealthBar(const SpriteDesc& t,
                         float ratio,
                         const CoordinateFrame& cf,
                         const Orientation& o)
  {
    // The health bar goes from `green` for `healthy`
    // to `red` for `almost-dead`. The `empty` part
    // of the health bar will be darkened to make it
    // stand out.
    olc::Pixel hbc = colorGradient(olc::RED, olc::GREEN, ratio, ALPHA_ALMOST_OPAQUE);
    olc::Pixel bc(hbc.r / 2, hbc.g / 2, hbc.b / 2, hbc.a);

    // The combined length of the healthbar will be
    // 7/10th of the size of the tile and 1/10th of
    // the size on the perpendicular direction.
    // The offset will also be ~1/10th of the size
    // of a tile.
    float hbBRatio = 0.7f;
    float hbSRatio = 0.1f;
    float hbOffset = 0.12f;

    olc::vf2d s = t.radius * cf.tileSize();
    olc::vf2d p = cf.tileCoordsToPixels(t.x, t.y, t.loc, t.radius);

    if (o == Orientation::Horizontal) {
      // We interpret the big ratio as a width and
      // the small one as a height. The offset is
      // applied on height.
      olc::vf2d hp = olc::vf2d(p.x + (1.0f - hbBRatio) * s.x / 2.0f, p.y - s.y * hbOffset);
      olc::vf2d hs = olc::vf2d(s.x * hbBRatio * ratio, s.y * hbSRatio);
      FillRectDecal(hp, hs, hbc);

      hp = olc::vf2d(p.x + (1.0f - hbBRatio) * s.x / 2.0f + s.x * hbBRatio * ratio, p.y - s.y * hbOffset);
      hs = olc::vf2d(s.x * hbBRatio * (1.0f - ratio), s.y * hbSRatio);
      FillRectDecal(hp, hs, bc);
    }

    if (o == Orientation::Vertical) {
      // We interpret the big ration as a height
      // and the small one as a width. The offset
      // is applied on width. As we display the
      // healthbar with the `full` part in the
      // bottom of the tile, we will first draw
      // the `empty` part (as it is on top of
      // the `full` part) to simplify the process
      // to compute positions.

      // `Empty` part first.
      olc::vf2d hp(p.x + s.x * hbOffset, p.y + (1.0f - hbBRatio) * s.y / 2.0f);
      olc::vf2d hs(s.x * hbSRatio, s.y * hbBRatio * (1.0f - ratio));
      FillRectDecal(hp, hs, bc);

      // `Full` part.
      hp = olc::vf2d(p.x + s.x * hbOffset, p.y + (1.0f - hbBRatio) * s.y / 2.0f + s.y * hbBRatio * (1.0f - ratio));
      hs = olc::vf2d(s.x * hbSRatio, s.y * hbBRatio * ratio);
      FillRectDecal(hp, hs, hbc);
    }
  }

}

#endif    /* PGE_APP_HXX */
