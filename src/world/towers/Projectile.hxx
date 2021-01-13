#ifndef    PROJECTILE_HXX
# define   PROJECTILE_HXX

# include "Projectile.hh"

namespace tdef {

  inline
  Projectile::PProps
  Projectile::newProps(const utils::Point2f& p,
                       const utils::Uuid& owner) noexcept
  {
    PProps pp;
    assignProps(pp, p, owner);

    pp.speed = 1.0f;

    pp.damage = 1.0f;
    pp.aoeRadius = 0.0f;

    pp.accuracy = 1.0f;

    pp.freezePercent = 0.0f;
    pp.freezeSpeed = 0.0f;
    pp.freezeDuration = utils::Duration::zero();

    pp.poison = 0.0f;
    pp.poisonDuration = utils::Duration::zero();

    return pp;
  }

  inline
  void
  Projectile::init(StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  void
  Projectile::pause(const utils::TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Projectile::resume(const utils::TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Projectile::destroy(StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  void
  Projectile::worldUpdate(LocatorShPtr /*loc*/) {
    // Nothing to do.
  }

}

#endif    /* PROJECTILE_HXX */
