#ifndef    COLOR_UTILS_HXX
# define   COLOR_UTILS_HXX

# include "ColorUtils.hh"

namespace tdef {

  inline
  olc::Pixel
  colorGradient(const olc::Pixel& low,
                const olc::Pixel& high,
                float ratio,
                int alpha) noexcept
  {
    ratio = std::min(std::max(ratio, 0.0f), 1.0f);

    return olc::Pixel(
      static_cast<int>((1.0f - ratio) * low.r + ratio * high.r),
      static_cast<int>((1.0f - ratio) * low.g + ratio * high.g),
      static_cast<int>((1.0f - ratio) * low.b + ratio * high.b),
      alpha
    );
  }
}


inline
std::string
toString(const olc::Pixel& p) noexcept {
  return
    std::string("[r: ") +
    std::to_string(p.r) +
    ", g: " + std::to_string(p.g) +
    ", b: " + std::to_string(p.b) +
    ", a: " + std::to_string(p.a) +
    "]"
  ;
}


#endif    /* COLOR_UTILS_HXX */
