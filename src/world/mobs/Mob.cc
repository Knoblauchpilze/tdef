
# include "Mob.hh"
# include "Portal.hh"
# include "Locator.hh"

namespace tdef {

  Mob::Mob(const MProps& props):
    WorldElement(props, mobs::toString(props.type)),

    m_type(props.type),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_rArrival(props.arrival),
    m_speed(props.speed),
    m_path(path::newPath(m_pos)),

    m_bounty(props.bounty),
    m_cost(props.lives),

    m_defense(fromProps(props))
  {
    setService("mob");
  }

  bool
  Mob::damage(const mobs::Damage& d) {
    // TODO: Handle the rest of the effect.
    bool alive = WorldElement::damage(d.hit);
    
    if (!alive) {
      markForDeletion(true);
    }
    
    return alive;
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
      // We arrived at the portal, hit it and then
      // mark the mob for deletion.
      p->breach();
      markForDeletion(true);

      log(
        "Mob made it through (health: " + std::to_string(getHealth()) + ")" +
        ", lives: " + std::to_string(p->getLives())
      );

      return;
    }

    log("Found portal at " + bp.toString() + " (d: " + std::to_string(d) + ")");

    // Generate a path to the portal and go there.
    path::Path np = path::newPath(m_pos);
    if (!np.generatePathTo(info.frustum, bp, true)) {
      log("Failed to generate path to " + bp.toString(), utils::Level::Error);
      return;
    }

    std::swap(m_path, np);
  }

  void
  Mob::worldUpdate(LocatorShPtr loc) {
    // We need to recompute the path to the target if
    // a valid path is assigned.
    if (!isEnRoute()) {
      return;
    }

    utils::Point2f e = m_path.target();

    // Generate a path to the portal and go there.
    path::Path np = path::newPath(m_pos);
    if (!np.generatePathTo(loc, e, true)) {
      log("Failed to generate path to " + e.toString(), utils::Level::Error);
      return;
    }

    log("Generated new path to " + e.toString());

    std::swap(m_path, np);
  }

}
