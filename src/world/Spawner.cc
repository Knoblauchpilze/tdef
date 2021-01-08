
# include "Spawner.hh"
# include "MobFactory.hh"

namespace tdef {

  Spawner::Spawner(const SProps& props):
    Block(props, "spawner"),

    m_distribution(props.mobs),

    m_spawnRadius(std::max(props.spawnRadius, 0.0f)),
    m_minWaveSize(props.minWaveSize),
    m_maxWaveSize(std::max(props.maxWaveSize, 1)),

    m_stock(props.reserve),
    m_threshold(props.threshold),
    m_refill(props.refill)
  {
    setService("spawner");

    if (m_distribution.empty()) {
      error(
        "Invalid distribution provided to spawner",
        "No element in distribution"
      );
    }
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

    // Generate the size of the wave.
    int size = info.rng.rndInt(m_minWaveSize, m_maxWaveSize);
    for (int id = 0u ; id < size ; ++id) {
      info.spawnMob(mob);
    }
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

    // Randomize the type of the mob based on the
    // distribution associated to this spawner and
    // return the mob created from this type.
    float prob = info.rng.rndFloat(0.0f, 1.0f);
    unsigned id = 0u;
    float cumProb = 0.0f;
    while (id < m_distribution.size() && cumProb + m_distribution[id].prob < prob) {
      cumProb += m_distribution[id].prob;
      ++id;
    }

    id = std::min(static_cast<unsigned>(m_distribution.size() - 1u), id);

    Mob::MProps props = mobs::generateProps(m_distribution[id].mob, utils::Point2f(x, y));
    return std::make_shared<Mob>(props);
  }

}
