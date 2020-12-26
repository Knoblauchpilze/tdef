#ifndef    BACKGROUND_DESC_HXX
# define   BACKGROUND_DESC_HXX

# include "BackgroundDesc.hh"

namespace tdef {
  namespace menu {

    inline
    BackgroundDesc
    newColoredBackground(const olc::Pixel& c) noexcept {
      olc::Pixel hsl = tdef::RGBToHSL(c);

      BackgroundDesc bd;
      bd.color = c;
      bd.hColor = tdef::modulate(c, hsl.b > 127 ? 0.5f : 2.0f);

      return bd;
    }

  }
}

#endif    /* BACKGROUND_DESC_HXX */
