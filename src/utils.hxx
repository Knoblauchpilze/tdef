#ifndef    UTILS_HXX
# define   UTILS_HXX

# include "utils.hh"

template <typename CoordinateType>
inline
std::string
toString(const olc::v2d_generic<CoordinateType>& vec) noexcept {
  return std::string("[x: ") + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + "]";
}

template <typename CoordinateType>
inline
std::ostream&
operator<<(std::ostream& out, const olc::v2d_generic<CoordinateType>& vec) noexcept {
  out << toString(vec);
  return out;
}

template <typename CoordinateType>
inline
std::ostream&
operator<<(const olc::v2d_generic<CoordinateType>& vec, std::ostream& out) noexcept {
  return operator<<(out, vec);
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


#endif    /* UTILS_HXX */
