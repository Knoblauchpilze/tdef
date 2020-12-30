#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"

namespace tdef {

  inline
  Mob::MProps
  Mob::newProps(const utils::Point2f& p,
                const mobs::Type& type,
                const utils::Uuid& owner) noexcept
  {
    MProps pp;
    assignProps(pp, p, owner);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.01f;

    pp.speed = 1.0f;
    pp.arrival = 0.0001f;

    pp.bounty = 1.0f;

    pp.type = type;

    return pp;
  }

  inline
  const mobs::Type&
  Mob::getType() const noexcept {
    return m_type;
  }

  inline
  const path::Path&
  Mob::getPath() const noexcept {
    return m_path;
  }

  inline
  float
  Mob::getBounty() const noexcept {
    return m_bounty;
  }

  inline
  bool
  Mob::isEnRoute() const noexcept {
    return m_path.enRoute(m_rArrival);
  }

  inline
  void
  Mob::init(StepInfo& /*info*/) {}

  inline
  void
  Mob::destroy(StepInfo& /*info*/) {}

}

#endif    /* MOB_HXX */
