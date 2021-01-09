
# include "Projectile.hh"

namespace tdef {

  Projectile::Projectile(const PProps& props,
                         MobShPtr mob):
    WorldElement(props, "projectile"),

    m_target(mob),

    m_speed(props.speed),

    m_damage(props.damage),
    m_aoeRadius(props.aoeRadius),
    m_aoeDamage(props.aoeDamage),
    m_accuracy(props.accuracy),

    m_freezePercent(props.freezePercent),
    m_freezeSpeed(props.freezeSpeed),
    m_freezeDuration(props.freezeDuration),

    m_poison(props.poison),
    m_poisonDuration(props.poisonDuration)
  {
    setService("projectile");
  }

  void
  Projectile::step(StepInfo& /*info*/) {
    // TODO: Handle this.
  }

  void
  Projectile::pause(const utils::TimeStamp& /*t*/) {
    // TODO: Handle this.
  }

  void
  Projectile::resume(const utils::TimeStamp& /*t*/) {
    // TODO: Handle this.
  }

}
