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