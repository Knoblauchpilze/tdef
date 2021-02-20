
# include "Spawner.hh"
# include "MobFactory.hh"
# include "SpawnerFactory.hh"

namespace tdef {

  Spawner::Spawner(const SProps& props):
    Block(props, "spawner"),

    m_distribution(props.mobs),

    m_spawnRadius(std::max(props.spawnRadius, 0.0f)),

    m_stock(props.reserve),
    m_threshold(props.threshold),
    m_refill(props.refill),

    m_exp(0),
    m_difficulty(props.difficulty),
    m_processes(spawners::generateData(m_difficulty))
  {
    setService("spawner");

    if (m_distribution.empty()) {
      error(
        "Invalid distribution provided to spawner",
        "No element in distribution"
      );
    }
  }

  std::istream&
  Spawner::operator>>(std::istream& in) {
    Block::operator>>(in);

    // Distribution.
    unsigned count;
    in >> count;
    for (unsigned id = 0u ; id < count ; ++id) {
      float prob;
      in >> prob;
      int mt;
      in >> mt;

      spawners::DistItem it;
      it.prob = prob;
      it.mob = static_cast<mobs::Type>(mt);

      m_distribution.push_back(it);
    }

    in >> m_spawnRadius;
    in >> m_stock;
    in >> m_threshold;
    in >> m_refill;
    in >> m_exp;
    int dif;
    in >> dif;
    m_difficulty = static_cast<spawners::Level>(dif);
    // Generate processes based on the level.
    m_processes = spawners::generateData(m_difficulty);

    log("Restored spawner at " + m_pos.toString(), utils::Level::Verbose);

    return in;
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

    m_stock -= m_threshold;
    ++m_exp;

    // Generate a new wave.
    generateWave(info);
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
    props.health = m_processes.health(info, m_exp);
    props.bounty = m_processes.bounty(info, props.health);

    log(
      "Spawning " + mobs::toString(props.type) + " at " +
      props.pos.toString() + " with " +
      std::to_string(props.health) + " health and worth " +
      std::to_string(props.bounty) + " gold",
      utils::Level::Verbose
    );

    return std::make_shared<Mob>(props);
  }

  void
  Spawner::generateWave(StepInfo& info) {
    int size = m_processes.wave(info, m_exp);

    for (int id = 0u ; id < size ; ++id) {
    // Spawn a new entity and prepare it.
      MobShPtr mob = spawn(info);
      if (mob == nullptr) {
        log("Spawner generated null entity, discarding it");
        continue;
      }

      info.spawnMob(mob);
    }
  }

}
