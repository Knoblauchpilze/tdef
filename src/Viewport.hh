#ifndef    VIEWPORT_HH
# define   VIEWPORT_HH

# include "olcPixelGameEngine.h"

namespace tdef {

  /**
   * @brief - Defines a viewport from its top left
   *          corner and the associated dimensions.
   */
  struct Viewport {
    olc::vf2d p;
    olc::vf2d dims;
  };

}

#endif    /* VIEWPORT_HH */
