
# include "Projectile.hh"
# include <maths_utils/LocationUtils.hh>
# include "Locator.hh"

namespace tdef {

  Projectile::Projectile(const PProps& props,
                         MobShPtr mob):
    WorldElement(props, "projectile"),

    m_target(mob),

    m_speed(props.speed),

    m_damage(props.damage),
    m_aoeRadius(props.aoeRadius),
    m_accuracy(props.accuracy),

    m_freezePercent(props.freezePercent),
    m_freezeSpeed(props.freezeSpeed),
    m_freezeDuration(props.freezeDuration),

    m_poisonDuration(props.poisonDuration)
  {
    setService("projectile");
  }

  void
  Projectile::step(StepInfo& info) {
    // Check whether the projectile has arrived to its target.
    // Note that we will try to reach the target even in case
    // it is dead so that we can handle the aoe damage.
    const utils::Point2f& t = m_target->getPos();
    float dst = utils::d(getPos(), t);

    if (dst > sk_arrived) {
      // Move to reach the target.
      float dx = t.x() - getPos().x();
      float dy = t.y() - getPos().y();

      dx /= dst;
      dy /= dst;

      m_pos.x() += dx * m_speed * info.elapsed;
      m_pos.y() += dy * m_speed * info.elapsed;

      return;
    }

    // Get all the mobs that are within the `aoe` radius
    // at the moment of the hit.
    std::vector<MobShPtr> wounded;

    if (m_aoeRadius > 0.0f) {
      wounded = info.frustum->getVisibleMobs(t, m_aoeRadius, nullptr);
    }

    wounded.push_back(m_target);

    // Apply damage to all the mobs.
    mobs::Damage d;
    d.hit = m_damage;

    d.speed = m_freezePercent;
    d.sDecraseSpeed = m_freezeSpeed;
    d.sDuration = m_freezeDuration;

    d.pDuration = m_poisonDuration;

    for (unsigned id = 0; id < wounded.size() ; ++id) {
      // The damage depends on the distance to the
      // center of the projectile. For the case of
      // the target we will apply the maximum dmg
      // and for the other mob we will apply damage
      // based on the distance.
      if (wounded[id] == m_target) {
        d.hit = m_damage;
      }
      else {
        float far = utils::d(t, wounded[id]->getPos());
        d.hit = std::max(0.0f, m_damage * far / m_aoeRadius);
      }

      log(
        "Damaging " + mobs::toString(wounded[id]->getType()) +
        " at " + std::to_string(utils::d(t, wounded[id]->getPos())) +
        " (target: " + std::to_string(wounded[id] == m_target) + ")" +
        " with " + std::to_string(d.hit) + " damage" +
        ", health: " + std::to_string(wounded[id]->getHealth())
      );
      wounded[id]->damage(d);
    }

    // The projectile is now obsolete.
    markForDeletion(true);
  }

}
