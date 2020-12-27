#ifndef    TEXTURE_PACK_HXX
# define   TEXTURE_PACK_HXX

# include "TexturePack.hh"

namespace tdef {

  inline
  TexturePack::~TexturePack() {
    if (m_pack != nullptr) {
      delete m_pack;
      m_pack = nullptr;
    }
  }

  inline
  void
  TexturePack::draw(olc::PixelGameEngine* pge,
                    const sprites::Sprite& s,
                    const olc::vf2d& p,
                    float scale) const
  {
    draw(pge, s, p, olc::vf2d(scale, scale));
  }

  inline
  olc::vi2d
  TexturePack::spriteCoords(const olc::vi2d& coord,
                            int id) const noexcept
  {
    // Compute linear identifier from 2D coordinates.
    int lID = coord.y * m_layout.x + coord.x + id;

    // Go back to 2D coordinates using the layout on
    // the linearized ID and the size of the sprite
    // to obtain a pixels position.
    return olc::vi2d(
      (lID % m_layout.x) * m_sSize.x,
      (lID / m_layout.x) * m_sSize.y
    );
  }

}

#endif    /* TEXTURE_PACK_HXX */
