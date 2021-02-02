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

    pp.stunProb = 0.0f;
    pp.critProb = 0.0f;
    pp.critMultiplier = 1.0f;

    pp.freezeDuration = utils::Duration::zero();
    pp.stunDuration = utils::Duration::zero();
    pp.poisonDuration = utils::Duration::zero();

    return pp;
  }

  inline
  std::ostream&
  Projectile::operator<<(std::ostream& out) const {
    WorldElement::operator<<(out);

    // TODO: Handle target and tower.
    // m_target;
    // Tower* m_tower;
    out << m_speed;
    out << m_damage;
    out << m_aoeRadius;
    out << m_accuracy;
    out << m_freezePercent;
    out << m_freezeSpeed;
    out << m_stunProb;
    out << m_critProb;
    out << m_critMultiplier;
    // TODO: Handle durations.
    // utils::Duration m_freezeDuration;
    // utils::Duration m_stunDuration;
    // utils::Duration m_poisonDuration

    log("Saved projectile at " + m_pos.toString(), utils::Level::Verbose);

    return out;
  }

  inline
  std::istream&
  Projectile::operator>>(std::istream& in) {
    WorldElement::operator>>(in);

    // TODO: Handle target and tower.
    // m_target;
    // Tower* m_tower;
    in >> m_speed;
    in >> m_damage;
    in >> m_aoeRadius;
    in >> m_accuracy;
    in >> m_freezePercent;
    in >> m_freezeSpeed;
    in >> m_stunProb;
    in >> m_critProb;
    in >> m_critMultiplier;
    // TODO: Handle durations.
    // utils::Duration m_freezeDuration;
    // utils::Duration m_stunDuration;
    // utils::Duration m_poisonDuration

    log("Restored projectile at " + m_pos.toString(), utils::Level::Verbose);

    return in;
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

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Projectile& p) noexcept {
  p << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Projectile& p) noexcept {
  p >> in;
  return in;
}

#endif    /* PROJECTILE_HXX */
