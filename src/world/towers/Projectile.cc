
# include "Projectile.hh"
# include <maths_utils/LocationUtils.hh>

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
  Projectile::step(StepInfo& info) {
    // Check whether the projectile has arrived to its target.
    // Note that we will try to reach the target even in case
    // it is dead so that we can handle the aoe damage.
    float d = utils::d(getPos(), m_target->getPos());

    if (d > sk_arrived) {
      // Move to reach the target.
      float dx = m_target->getPos().x() - getPos().x();
      float dy = m_target->getPos().y() - getPos().y();

      dx /= d;
      dy /= d;

      m_pos.x() += dx * m_speed * info.elapsed;
      m_pos.y() += dy * m_speed * info.elapsed;

      return;
    }

    // TODO: Handling damage.
    log("Arrived");

    markForDeletion(true);
  }

}
