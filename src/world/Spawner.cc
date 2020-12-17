
# include "Spawner.hh"

namespace tdef {

  Spawner::Spawner(const utils::Point2f& pos,
                   float threshold,
                   float reserve,
                   float refill,
                   float radius,
                   const utils::Uuid& owner):
    WorldElement(pos, 1.0f, 1.0f, "spawner", owner),

    m_spawnRadius(std::max(radius, 0.0f)),

    m_stock(reserve),
    m_threshold(threshold),
    m_refill(refill)
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
    return std::make_shared<Mob>(
      utils::Point2f(x, y),
      0.5f,
      1.0f,
      0.01f,
      getOwner()
    );
  }

}
