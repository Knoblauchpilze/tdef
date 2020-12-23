#ifndef    TOPVIEW_FRAME_HXX
# define   TOPVIEW_FRAME_HXX

# include "TopViewFrame.hh"

namespace tdef {

  inline
  TopViewFrame::TopViewFrame(const Viewport& cvp,
                             const Viewport& pvp,
                             const olc::vi2d& tileSize):
    CoordinateFrame(cvp, pvp, tileSize)
  {}

  inline
  Viewport
  TopViewFrame::cellsViewport() const noexcept {
    // We know the position of the origin point
    // in pixels. We also know the size of each
    // cell in pixels.
    // From there we can convert the position
    // `(0, 0)` in pixels to tiles.
    olc::vi2d tl = pixelCoordsToTiles(olc::vi2d(0, 0));
    olc::vi2d br = pixelCoordsToTiles(olc::vi2d(m_pViewport.dims.x, m_pViewport.dims.y));

    Viewport out;
    out.p = tl;
    // The `+1` comes from the fact that the
    // `br.x - tl.x` accounts for the number
    // of tiles in difference but does not
    // count the actual `tl.x` tile.
    out.dims = olc::vf2d(br.x - tl.x + 1, br.y - tl.y + 1);

    return out;
  }

  inline
  olc::vf2d
  TopViewFrame::tileCoordsToPixels(float x,
                                   float y,
                                   const RelativePosition& loc,
                                   float radius) const noexcept
  {
    // Offset the input coordinates based on the
    // current position of the cell's viewport.
    x -= m_cViewport.p.x;
    y -= m_cViewport.p.y;

    // Convert to top view coordinates: we just
    // need to scale by the tile size.
    olc::vf2d tp(
      m_pViewport.p.x + x * m_tScaled.x,
      m_pViewport.p.y + y * m_tScaled.y
    );

    // Account for the relative position of the
    // tile compared to the input position.
    switch (loc) {
      case RelativePosition::CenterTop:
        // Offset by a full tile in height and
        // half a tile in width.
        tp.x -= radius * tileSize().x / 2.0f;
        tp.y -= radius * tileSize().y;
        break;
      case RelativePosition::Center:
        tp.x -= radius * tileSize().x / 2.0f;
        tp.y -= radius * tileSize().y / 2.0f;
      case RelativePosition::BottomRight:
        // This is the default case.
      default:
        // Nothing to do.
        break;
    }

    return tp;
  }

  inline
  olc::vi2d
  TopViewFrame::pixelCoordsToTiles(const olc::vi2d& pixels, olc::vf2d* intraTile) const noexcept {
    // The conversion to float value is necessary in the
    // case of negative values where for example coords
    // `(-0.5, -0.5)` should be interpreted as belonging
    // to the cell `(-1, -1)`.
    float pox = pixels.x - m_pViewport.p.x;
    float poy = pixels.y - m_pViewport.p.y;

    int tx = static_cast<int>(std::floor(pox / m_tScaled.x));
    int ty = static_cast<int>(std::floor(poy / m_tScaled.y));

    olc::vi2d rt(tx, ty);

    // The viewport should be handled after we first
    // computed coordinates in the world's frame: it
    // is what is meant by the cells' offset.
    rt.x += m_cViewport.p.x;
    rt.y += m_cViewport.p.y;

    // Compute the intra-tile offset by converting
    // the tile position to pixels and comparing the
    // input value.
    olc::vf2d tlp = tileCoordsToPixels(rt.x, rt.y);

    if (intraTile != nullptr) {
      intraTile->x = pixels.x - tlp.x;
      intraTile->y = pixels.y - tlp.y;

      // Convert the intra tile to value in the range
      // `[0; 1]`.
      intraTile->x /= m_tScaled.x;
      intraTile->y /= m_tScaled.y;
    }

    return rt;
  }

}

#endif    /* TOPVIEW_FRAME_HXX */
