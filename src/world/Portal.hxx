#ifndef    PORTAL_HXX
# define   PORTAL_HXX

# include "Portal.hh"

namespace tdef {

  inline
  Portal::PProps
  Portal::newProps(const utils::Point2f& p,
                   const utils::Uuid& owner) noexcept
  {
    PProps pp;
    assignProps(pp, p, owner);

    pp.lives = 15;

    return pp;
  }

  inline
  float
  Portal::getLives() const noexcept {
    return m_lives;
  }

  inline
  std::ostream&
  Portal::operator<<(std::ostream& out) const {
    Block::operator<<(out);
    out << m_lives;

    log("Saved portal at " + m_pos.toString(), utils::Level::Verbose);

    return out;
  }

  inline
  std::istream&
  Portal::operator>>(std::istream& in) {
    Block::operator>>(in);
    in >> m_lives;

    log("Restored portal at " + m_pos.toString(), utils::Level::Verbose);

    return in;
  }

}

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Portal& p) noexcept {
  p << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Portal& p) noexcept {
  p >> in;
  return in;
}

#endif    /* PORTAL_HXX */
