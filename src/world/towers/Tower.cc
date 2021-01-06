
# include "Tower.hh"
# include "Mob.hh"
# include "Locator.hh"
# include <maths_utils/LocationUtils.hh>

namespace tdef {

  Tower::Tower(const TProps& props,
               const towers::Data& desc):
    Block(props, "tower"),

    m_type(props.type),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_attackCost(props.attackCost),

    m_minRange(props.minRange),
    m_maxRange(props.maxRange),
    m_shootAngle(props.shootAngle),

    m_attack(fromProps(props)),
    m_processes(desc),

    m_target(nullptr)
  {
    setService("tower");
  }

  void
  Tower::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // Pick and align with the target.
    if (!pickAndAlignWithTarget(info) || m_target == nullptr) {
      // Can't do anything: we didn't find a
      // target or we are not aligned with it
      // yet.
      return;
    }

    // Check whether we can attack.
    if (m_energy < m_attackCost) {
      return;
    }

    log(
      "Hitting mob " + std::to_string(static_cast<int>(m_target->getType())) +
      " at " + m_target->getPos().toString() +
      " for " + std::to_string(m_attack.damage) + " damage" +
      " (health: " + std::to_string(m_target->getHealth()) + ")"
    );

    // Hit the mob with a devastating attack.
    m_energy -= m_attackCost;

    towers::DamageData dd;
    dd.damage = m_attack.damage;

    dd.aoeRadius = m_attack.aoeRadius;
    dd.aoeDamage = m_attack.aoeDamage;

    dd.accuracy = m_attack.accuracy;

    dd.speed = m_attack.speed;
    dd.sDuration = m_attack.sDuration;

    dd.poison = m_attack.poison;
    dd.pDuration = m_attack.pDuration;

    if (m_processes.damage(info, m_target, dd)) {
      return;
    }

    log("Killed mob at " + m_target->getPos().toString() + ", earned " + std::to_string(m_target->getBounty()) + " coin(s)");

    info.gold += m_target->getBounty();

    // Mark this mob for deletion.
    info.removeMob(m_target.get());

    // Clear the target.
    m_target.reset();
  }

  bool
  Tower::pickAndAlignWithTarget(StepInfo& info) {
    // Check whether a target is already defined.
    // If this is the case we will make sure that
    // the distance is still less than the range
    // of the tower (so that we can shoot it).
    if (m_target != nullptr) {
      float d = utils::d(m_target->getPos(), getPos());
      
      if (d < m_minRange || d > m_maxRange) {
        m_target.reset();
      }

      if (m_target->getHealthRatio() <= 0.0f) {
        m_target.reset();
      }
    }

    // The above section might have reset the
    // target: in this case we want to find a
    // new one so that we can still spend the
    // frame trying to face it.
    if (m_target == nullptr) {
      // Find the closest mob.
      towers::PickData pd;
      pd.pos = m_pos;
      pd.minRange = m_minRange;
      pd.maxRange = m_maxRange;

      m_target = m_processes.pickMob(info, pd);

      if (m_target == nullptr) {
        // No mobs are visible, nothing to do.
        return false;
      }
    }

    // We now know that a target *is* set and
    // that it lies within the range of the
    // tower. We will try to align with it:
    // this means rotating the tower until we
    // are pointing right at the target.
    float dx = m_target->getPos().x() - getPos().x();
    float dy = m_target->getPos().y() - getPos().y();

    float theta = std::atan2(dy, dx);

    // Rotate to close the gap between the
    // current orientation and the desired
    // one. We cannot rotate more than the
    // maximum rotation speed.
    float dif = theta - m_orientation;

    float achieved = std::min(dif, m_rotationSpeed * info.elapsed);

    m_orientation += achieved;

    // The rotation part has been handled, which
    // means that we are now facing the target as
    // best as we can for this frame. In order to
    // shot at it we need to determine whether it
    // lies within the firing cone.
    return std::abs(m_orientation - theta) <= m_shootAngle;
  }

}
