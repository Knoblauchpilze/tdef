#ifndef    TOWER_HXX
# define   TOWER_HXX

# include "Tower.hh"

namespace tdef {

  inline
  Tower::TProps
  Tower::newProps(const utils::Point2f& p,
                  const utils::Uuid& owner) noexcept
  {
    TProps pp;
    assignProps(pp, p, owner);

    return pp;
  }

}

#endif    /* TOWER_HXX */
