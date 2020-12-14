#ifndef    COLOR_UTILS_HH
# define   COLOR_UTILS_HH

# include "olcPixelGameEngine.h"

namespace tdef {

  olc::Pixel
  colorGradient(const olc::Pixel& low,
                const olc::Pixel& high,
                float ratio,
                int alpha) noexcept;
}

std::string
toString(const olc::Pixel& p) noexcept;

# include "ColorUtils.hxx"

#endif    /* COLOR_UTILS_HH */
