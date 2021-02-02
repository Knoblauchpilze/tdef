#ifndef    WALL_HXX
# define   WALL_HXX

# include "Wall.hxx"

namespace tdef {

  inline
  Wall::WProps
  Wall::newProps(const utils::Point2f& p,
                 const utils::Uuid& owner) noexcept
  {
    WProps pp;
    assignProps(pp, p, owner);

    pp.height = 1.0f;

    return pp;
  }

  inline
  float
  Wall::getHeight() const noexcept {
    return m_height;
  }

  inline
  std::ostream&
  Wall::operator<<(std::ostream& out) const {
    Block::operator<<(out);
    out << m_height;

    log("Saved wall at " + m_pos.toString(), utils::Level::Verbose);

    return out;
  }

  inline
  std::istream&
  Wall::operator>>(std::istream& in) {
    Block::operator>>(in);
    in >> m_height;

    log("Restored wall at " + m_pos.toString(), utils::Level::Verbose);

    return in;
  }

}

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Wall& w) noexcept {
  w << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Wall& w) noexcept {
  w >> in;
  return in;
}

#endif    /* Wall_HXX */
