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

    // Only save the destination and not the target:
    // this will make the project only reach the last
    // position of the target when it is restored but
    // it is enough for our purposes.
    out << m_dest.x() << " " << m_dest.y() << " ";

    // We won't save the tower to the file: indeed it
    // would require to be able to then relink in some
    // way the re-loaded tower to this projectile. As
    // of now we don't really have a way to do it. Not
    // doing it means not being able to credit some XP
    // to the tower if the projectile kills some mobs.
    // We consider that it is a small price to pay for
    // the convenience to not bother relinking towers
    // and projectiles.
    out << m_speed << " ";
    out << m_damage << " ";
    out << m_aoeRadius << " ";
    out << m_accuracy << " ";
    out << m_freezePercent << " ";
    out << m_freezeSpeed << " ";
    out << m_stunProb << " ";
    out << m_critProb << " ";
    out << m_critMultiplier << " ";
    out << utils::toMilliseconds(m_freezeDuration) << " ";
    out << utils::toMilliseconds(m_stunDuration) << " ";
    out << utils::toMilliseconds(m_poisonDuration) << " ";

    verbose("Saved projectile at " + m_pos.toString());

    return out;
  }

  inline
  std::istream&
  Projectile::operator>>(std::istream& in) {
    WorldElement::operator>>(in);

    // Restore only the destination as the target
    // has not been saved.
    m_target = nullptr;
    in >> m_dest.x();
    in >> m_dest.y();

    // As explained in the serialization operation
    // we don't save the attached tower: so we will
    // just assign a null tower.
    m_tower = nullptr;
    in >> m_speed;
    in >> m_damage;
    in >> m_aoeRadius;
    in >> m_accuracy;
    in >> m_freezePercent;
    in >> m_freezeSpeed;
    in >> m_stunProb;
    in >> m_critProb;
    in >> m_critMultiplier;
    float d;
    in >> d;
    m_freezeDuration = utils::toMilliseconds(d);
    in >> d;
    m_stunDuration = utils::toMilliseconds(d);
    in >> d;
    m_poisonDuration = utils::toMilliseconds(d);

    verbose("Restored projectile at " + m_pos.toString());

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

  inline
  void
  Projectile::updateTrackedDestination() {
    // Update the destination from the position of the
    // target if any is dedined.
    if (m_target != nullptr) {
      m_dest = m_target->getPos();
    }
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
