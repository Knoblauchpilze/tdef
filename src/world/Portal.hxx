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

}

#endif    /* PORTAL_HXX */
