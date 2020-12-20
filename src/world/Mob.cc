
# include "Mob.hh"

namespace tdef {

  Mob::Mob(const MProps& props):
    WorldElement(props, "mob"),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_rArrival(props.arrival),
    m_speed(props.speed),
    m_path(path::newPath(m_pos))
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
