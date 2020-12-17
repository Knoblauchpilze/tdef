
# include "Mob.hh"

namespace tdef {

  Mob::Mob(const utils::Point2f& pos,
           float energy,
           float maxEnergy,
           float refill,
           const utils::Uuid& owner):
    WorldElement(pos, 1.0f, 1.0f, "mob", owner),

    m_energy(energy),
    m_maxEnergy(maxEnergy),
    m_energyRefill(refill)
  {
    setService("mob");
  }

  void
  Mob::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // TODO: Handle behavior.
  }

  void
  Mob::pause(const utils::TimeStamp& /*t*/) {}

  void
  Mob::resume(const utils::TimeStamp& /*t*/) {}

}
