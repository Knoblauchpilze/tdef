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
    p.layout = olc::vi2d(12, 1);
    m_tPackID = m_packs->registerPack(p);

    p.file = "data/img/mobs.png";
    p.sSize = olc::vi2d(64, 64);
    p.layout = olc::vi2d(11, 1);
    m_mPackID = m_packs->registerPack(p);

    p.file = "data/img/walls.png";
    p.sSize = olc::vi2d(64, 64);
    p.layout = olc::vi2d(1, 1);
    m_wPackID = m_packs->registerPack(p);

    // Assign a specific tint to the regular
    // drawing layer so that we have a built
    // in transparency.
    // We can't do it directly when drawing
    // in the rendering function because as
    // the whole layer will be drawn as one
    // quad in opengl with an opaque alpha,
    // we will lose this info.
    // This means that everything is indeed
    // transparent but that's the only way
    // for now to achieve it.
    setLayerTint(Layer::Draw, olc::Pixel(255, 255, 255, alpha::SemiOpaque));
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
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id].reset();
    }

    m_menus.clear();
  }

  inline
  bool
  TDefApp::onPause(float elapsed) {
    m_game->pause(elapsed);
    m_game->enable(false);
    return m_game->terminated();
  }

  inline
  bool
  TDefApp::onResume(float elapsed) {
    m_game->resume(elapsed);
    m_game->enable(true);
    return m_game->terminated();
  }

  inline
  void
  TDefApp::onInputs(const controls::State& c,
                    const CoordinateFrame& cf)
  {
    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    if (m_state != nullptr) {
      menu::InputHandle ih = m_state->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
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
    olc::Pixel hbc = colorGradient(olc::RED, olc::GREEN, ratio, alpha::AlmostOpaque);
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
