
# include "Projectile.hh"
# include <maths_utils/LocationUtils.hh>
# include <maths_utils/ComparisonUtils.hh>
# include "Locator.hh"
# include "Tower.hh"

namespace tdef {

  Projectile::Projectile(const PProps& props,
                         Tower* tower,
                         MobShPtr mob):
    WorldElement(props, "projectile"),

    m_target(mob),
    m_dest(),
    m_tower(tower),

    m_speed(props.speed),

    m_damage(props.damage),
    m_aoeRadius(props.aoeRadius),
    m_accuracy(props.accuracy),

    m_freezePercent(props.freezePercent),
    m_freezeSpeed(props.freezeSpeed),

    m_stunProb(utils::clamp(props.stunProb, 0.0f, 1.0f)),
    m_critProb(utils::clamp(props.critProb, 0.0f, 1.0f)),
    m_critMultiplier(props.critMultiplier),

    m_freezeDuration(props.freezeDuration),
    m_stunDuration(props.stunDuration),
    m_poisonDuration(props.poisonDuration)
  {
    setService("projectile");
  }

  void
  Projectile::step(StepInfo& info) {
    // Check whether the projectile has arrived to its target.
    // Note that we will try to reach the target even in case
    // it is dead so that we can handle the aoe damage.
    updateTrackedDestination();
    float dst = utils::d(getPos(), m_dest);

    if (dst > sk_arrived) {
      // Move to reach the target.
      float dx = m_dest.x() - getPos().x();
      float dy = m_dest.y() - getPos().y();

      dx /= dst;
      dy /= dst;

      // At best we can move of `m_speed * info.elapsed` in a
      // single frame. However, the distance required might
      // be smaller than that, in which case we don't want to
      // overshoot our target. Note that the `dx/y` can be
      // negative so the `s` (for "scaling factor") takes
      // that into account.
      // It measures how we should scale the maximum distance
      // we can travel to not overshoot the target.
      float covered = m_speed * info.elapsed;
      float s = std::min(dst / covered, 1.0f);

      m_pos.x() += s * dx * covered;
      m_pos.y() += s * dy * covered;

      return;
    }

    // Get all the mobs that are within the `aoe` radius
    // at the moment of the hit.
    std::vector<MobShPtr> wounded;
    if (m_aoeRadius > 0.0f) {
      // Note that we don't explicitely append the target
      // to the wounded list as we assume it will also
      // be found by this function anyway.
      wounded = info.frustum->getVisibleMobs(m_dest, m_aoeRadius, nullptr);
    }
    else {
      // In case there's no aoe, at least consider the
      // target if it is defined.
      if (m_target == nullptr) {
        // No target and no aoe, nothing to do.
        return;
      }

      wounded.push_back(m_target);
    }

    // Give a chance to critical hits.
    float damage = m_damage;
    if (m_critProb > 0.0f && info.rng.rndFloat(0.0f, 1.0f) < m_critProb) {
      damage = m_damage * m_critMultiplier;
    }

    // Apply damage to all the mobs.
    mobs::Damage d;
    d.hit = damage;

    d.accuracy = m_accuracy;

    d.speed = m_freezePercent;
    d.sDecraseSpeed = m_freezeSpeed;
    d.stunProb = m_stunProb;

    d.fDuration = m_freezeDuration;
    d.sDuration = m_stunDuration;
    d.pDuration = m_poisonDuration;

    for (unsigned id = 0; id < wounded.size() ; ++id) {
      // The damage depends on the distance to the
      // center of the projectile. For the case of
      // the target we will apply the maximum dmg
      // and for the other mob we will apply damage
      // based on the distance.
      if (wounded[id] == m_target) {
        d.hit = damage;
      }
      else {
        float far = utils::d(m_dest, wounded[id]->getPos());
        d.hit = std::max(0.0f, damage * far / m_aoeRadius);
      }

      // In case the target is already dead, do not
      // process it. This can happen as we allow a
      // projectile to keep targetting a dead target
      // for the aoe.
      if (wounded[id]->isDeleted()) {
        log("Mob " + mobs::toString(wounded[id]->getType()) + " is already deleted");
        continue;
      }

      bool alive = wounded[id]->hit(info, d);
      if (!alive) {
        log(
          "Killed " + mobs::toString(wounded[id]->getType()) +
          " at " + wounded[id]->getPos().toString() +
          ", earned " + std::to_string(wounded[id]->getBounty()) + " coin(s)" +
          " (deleted: " + std::to_string(wounded[id]->isDead()) + ")"
        );

        info.gold += wounded[id]->getBounty();

        // Propagate the experience gain.
        if (m_tower != nullptr && !m_tower->isDeleted()) {
          m_tower->gainExp(wounded[id]->getExpReward());
        }

        continue;
      }

      log(
        "Damaging " + mobs::toString(wounded[id]->getType()) +
        " at " + std::to_string(utils::d(m_dest, wounded[id]->getPos())) +
        " (target: " + std::to_string(wounded[id] == m_target) + ")" +
        " with " + std::to_string(d.hit) + " damage" +
        ", health: " + std::to_string(wounded[id]->getHealth())
      );
    }

    // The projectile is now obsolete.
    markForDeletion(true);
  }

}
