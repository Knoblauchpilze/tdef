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

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.1f;

    pp.range = 3.0f;
    pp.attack = 0.2f;
    pp.attackCost = 0.55f;

    return pp;
  }

}

#endif    /* TOWER_HXX */