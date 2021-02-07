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

      bd.scale = true;

      return bd;
    }

    inline
    void
    updateFromColor(BackgroundDesc& bg,
                    const olc::Pixel& c) noexcept
    {
      // Generate a new background.
      BackgroundDesc nbg = newColoredBackground(c);

      // Restore the other props.
      nbg.scale = bg.scale;

      // Swap values.
      std::swap(bg, nbg);
    }

  }
}

#endif    /* BACKGROUND_DESC_HXX */
