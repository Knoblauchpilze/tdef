
# include "Mob.hh"
# include "Portal.hh"
# include "Locator.hh"

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

    // In case we're already following a path, go
    // there.
    if (isEnRoute()) {
      m_path.advance(m_speed, info.elapsed, m_rArrival);
      m_pos = m_path.cur;

      return;
    }

    // Otherwise check where is the closest portal.
    BlockShPtr b = info.frustum->getClosestBlock(m_pos, world::BlockType::Portal, -1.0f, nullptr);
    PortalShPtr p = std::dynamic_pointer_cast<Portal>(b);
    if (p == nullptr) {
      // No blocks are visible, nothing to do.
      return;
    }

    utils::Point2f bp = b->getPos();

    float d = utils::d(bp, m_pos);
    if (d < m_rArrival) {
      // We arrived at the portal, hit it.
      // TODO: Handle this.
      return;
    }

    log("Found portal at " + bp.toString() + " (d: " + std::to_string(d) + ")");

    // Generate a path to the portal and go there.
    path::Path np = path::newPath(m_pos);
    if (!np.generatePathTo(info, bp, true)) {
      log("Failed to generate path to " + bp.toString());
      return;
    }

    std::swap(m_path, np);
  }

  void
  Mob::pause(const utils::TimeStamp& /*t*/) {}

  void
  Mob::resume(const utils::TimeStamp& /*t*/) {}

}
