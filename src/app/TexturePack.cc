
# include "TexturePack.hh"

namespace tdef {

  TexturePack::TexturePack(const sprites::Pack& pack):
    utils::CoreObject("pack"),

    m_sSize(pack.sSize),
    m_layout(pack.layout),

    m_pack(nullptr)
  {
    setService("textures");

    load(pack.file);
  }

  void
  TexturePack::draw(olc::PixelGameEngine* pge,
                    const sprites::Sprite& s,
                    const olc::vf2d& p,
                    const olc::vf2d& scale) const
  {
    // TODO: Replace with actual resolution of type.
    olc::vi2d sCoords = spriteCoords(olc::vi2d(s.sprite, 0), s.id);
    pge->DrawPartialDecal(p, m_pack, sCoords, m_sSize, scale, s.tint);
  }

  void
  TexturePack::load(const std::string& file) {
    // Load the file as a sprite and then convert it
    // to a faster `Decal` resource.
    olc::Sprite* spr = new olc::Sprite(file);
    if (spr == nullptr) {
      error(
        "Failed to load texture pack \"" + file + "\"",
        "Loading returned null"
      );
    }

    m_pack = new olc::Decal(spr);
  }

}
