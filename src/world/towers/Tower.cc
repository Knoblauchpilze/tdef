
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

    m_minRange(props.minRange),
    m_maxRange(props.maxRange),
    m_attack(fromProps(props)),

    m_processes(desc)
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
    towers::PickData pd;
    pd.pos = m_pos;
    pd.minRange = m_minRange;
    pd.maxRange = m_maxRange;

    MobShPtr m = m_processes.pickMob(info, pd);
    if (m == nullptr) {
      // No mobs are visible, nothing to do.
      return;
    }

    log(
      "Hitting mob at " + m->getPos().toString() +
      " for " + std::to_string(m_attack.damage) + " damage" +
      " (health: " + std::to_string(m->getHealthRatio()) + ")"
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

    if (m_processes.damage(info, m, dd)) {
      return;
    }

    log("Killed mob at " + m->getPos().toString() + ", earned " + std::to_string(m->getBounty()) + " coin(s)");

    info.gold += m->getBounty();

    // Mark this mob for deletion.
    info.removeMob(m.get());
  }

}
