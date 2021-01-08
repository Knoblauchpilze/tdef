#ifndef    BACKGROUND_DESC_HH
# define   BACKGROUND_DESC_HH

# include "olcEngine.hh"

namespace tdef {
  namespace menu {

    /**
     * @brief - Convenience structure defining the background that
     *          can be applied to a menu. We either define a color
     *          that is applied on all the menu or a sprite that
     *          is repeated a certain number of times.
     */
    struct BackgroundDesc {
      olc::Pixel color;
      olc::Pixel hColor;

      bool scale;
    };

    /**
     * @brief - Initializes a background structure defining a color
     *          for the background.
     * @param c - the color to assign to the background.
     * @return - the created background object.
     */
    BackgroundDesc
    newColoredBackground(const olc::Pixel& c) noexcept;

  }
}

# include "BackgroundDesc.hxx"

#endif    /* BACKGROUND_DESC_HH */
