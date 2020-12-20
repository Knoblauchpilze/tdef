#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"

namespace tdef {

  inline
  Mob::MProps
  Mob::newProps(const utils::Point2f& p,
                const utils::Uuid& owner) noexcept
  {
    Mob::MProps pp;

    pp.pos = p;
    pp.radius = 1.0f;

    pp.health = 1.0f;

    pp.owner = owner;

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.01f;

    pp.speed = 1.0f;
    pp.arrival = 0.0001f;
    
    return pp;
  }

  inline
  const path::Path&
  Mob::getPath() const noexcept {
    return m_path;
  }

  inline
  bool
  Mob::isEnRoute() const noexcept {
    return m_path.enRoute(m_rArrival);
  }

}

#endif    /* MOB_HXX */
