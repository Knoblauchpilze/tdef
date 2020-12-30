
# include "Tower.hh"
# include "Mob.hh"
# include "Locator.hh"

namespace tdef {

  Tower::Tower(const TProps& props,
               const towers::Data& desc):
    Block(props, "tower"),

    m_type(props.type),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_attackCost(props.attackCost),

    m_range(props.range),
    m_attack(props.attack),

    m_processes(desc)
  {
    // TODO: Handle upgrade.
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
    towers::PickData pd;
    pd.pos = m_pos;
    pd.range = m_range;

    MobShPtr m = m_processes.pickMob(info, pd);
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

    towers::DamageData dd;
    dd.attack = m_attack;

    if (m_processes.damage(info, m, dd)) {
      return;
    }

    log("Killed mob at " + m->getPos().toString() + ", earned " + std::to_string(m->getBounty()) + " coin(s)");

    info.gold += m->getBounty();

    // Mark this mob for deletion.
    info.removeMob(m.get());
  }

  void
  Tower::pause(const utils::TimeStamp& /*t*/) {}

  void
  Tower::resume(const utils::TimeStamp& /*t*/) {}

}
