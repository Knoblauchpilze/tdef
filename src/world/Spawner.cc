
# include "Spawner.hh"

namespace tdef {

  Spawner::Spawner(const SProps& props):
    Block(props, "spawner"),

    m_spawnRadius(std::max(props.spawnRadius, 0.0f)),

    m_stock(props.reserve),
    m_threshold(props.threshold),
    m_refill(props.refill)
  {
    setService("spawner");
  }

  void
  Spawner::step(StepInfo& info) {
    // Check whether the spawner is allowed to spawn
    // a new entity at this frame.
    refill(info.elapsed * m_refill, false);

    // Check for spawning conditions.
    if (m_stock < m_threshold) {
      return;
    }

    // Spawn a new entity and prepare it.
    m_stock -= m_threshold;

    MobShPtr mob = spawn(info);
    if (mob == nullptr) {
      log("Spawner generated null entity, discarding it");
      return;
    }

    info.spawnMob(mob);
  }

  MobShPtr
  Spawner::spawn(StepInfo& info) noexcept {
    // Spawn the entity within `radius` of the spawner,
    // using the provided rng to pick a point. Don't
    // forget to add the position of the spawner itself.
    float r = info.rng.rndFloat(0, m_spawnRadius * m_spawnRadius);
    float theta = info.rng.rndAngle();

    float x = std::sqrt(r) * std::cos(theta);
    float y = std::sqrt(r) * std::sin(theta);

    x += (m_pos.x() + 0.5f);
    y += (m_pos.y() + 0.5f);

    // Create the mob and return it.
    Mob::MProps props = Mob::newProps(utils::Point2f(x, y), getOwner());

    return std::make_shared<Mob>(props);
  }

}
