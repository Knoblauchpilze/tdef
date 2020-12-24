
# include "Tower.hh"
# include "Mob.hh"
# include "Locator.hh"

namespace tdef {

  Tower::Tower(const TProps& props):
    Block(props, "tower"),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_attackCost(props.attackCost),

    m_range(props.range),
    m_attack(props.attack)
  {
    setService("tower");
  }

  void
  Tower::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // Check whether we can attack.
    if (m_energy < m_attackCost) {
      return;
    }

    // Find the closest mob.
    MobShPtr m = info.frustum->getClosestMob(m_pos, m_range, nullptr);
    if (m == nullptr) {
      // No mobs are visible, nothing to do.
      return;
    }

    log(
      "Hitting mob at " + m->getPos().toString() +
      " for " + std::to_string(m_attack) + " damage" +
      " (health: " + std::to_string(m->getHealthRatio()) + ")"
    );

    // Hit the mob with a devastating attack.
    m_energy -= m_attackCost;
    if (m->damage(m_attack)) {
      return;
    }

    log("Killed mob at " + m->getPos().toString());

    // Mark this mob for deletion.
    // TODO: Handle this.
  }

  void
  Tower::pause(const utils::TimeStamp& /*t*/) {}

  void
  Tower::resume(const utils::TimeStamp& /*t*/) {}

}
