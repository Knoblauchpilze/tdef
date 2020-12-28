#ifndef    COLOR_UTILS_HH
# define   COLOR_UTILS_HH

# include "olcEngine.hh"

namespace tdef {

  olc::Pixel
  colorGradient(const olc::Pixel& low,
                const olc::Pixel& high,
                float ratio,
                int alpha) noexcept;

  /**
   * @brief - Convert the input color expressed in `RGB`
   *          color space to a color in `HSL` space.
   *          Note that some precision is lost when the
   *          output channels are converted to integer
   *          values.
   * @param rgb - the input color to convert.
   * @return - the equivalent in `HSL` color space.
   */
  olc::Pixel
  RGBToHSL(const olc::Pixel& rgb) noexcept;

  /**
   * @brief - Convert the input color expressed in `HSL`
   *          color space to a color in `RGB` space.
   *          Note that some precision is lost when the
   *          output channels are converted to integer
   *          values.
   * @param hsl - the input color to convert.
   * @return - the equivalent in `RGB` color space.
   */
  olc::Pixel
  HSLToRGB(const olc::Pixel& hsl) noexcept;

  /**
   * @brief - Modulate the lightness of the input color
   *          by the specified factor. We convert the
   *          color into `HSL` space to change the `L`
   *          component and then revert to `RGB` space.
   * @param in - the input color to modulate.
   * @param factor - a value indicating the change in
   *                 lightness to perform. A value
   *                 larger than `1` will brighten the
   *                 color and a value in the range `0`
   *                 to `1` will darken it. Any negative
   *                 value will be clamped to `0`.
   * @return - the modulated color.
   */
  olc::Pixel
  modulate(const olc::Pixel& in, float factor) noexcept;

  namespace alpha {

    static constexpr int Opaque = 255;
    static constexpr int AlmostOpaque = 192;
    static constexpr int SemiOpaque = 128;
    static constexpr int AlmostTransparent = 64;
    static constexpr int Transparent = 0;

  }

}

std::string
toString(const olc::Pixel& p) noexcept;

# include "ColorUtils.hxx"

#endif    /* COLOR_UTILS_HH */
