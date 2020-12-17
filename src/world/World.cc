
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/TimeUtils.hh>

namespace tdef {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_portal(0.0f, 0.0f),
    m_spawners(),
    m_mobs()
  {
    setService("world");

    generate();
  }

  World::World(int seed,
               const std::string& file):
    utils::CoreObject("world"),

    m_w(0),
    m_h(0),

    m_rng(seed),

    m_portal(0.0f, 0.0f),
    m_spawners(),
    m_mobs()
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
  }

  void
  World::step(float tDelta) {
    StepInfo si{
      m_rng,                   // rng

      utils::now(),            // moment
      tDelta,                  // elapsed

      std::vector<MobShPtr>(), // mSpawned
      std::vector<Mob*>(),     // mRemoved
    };

    // Make elements evolve.
    for (unsigned id = 0u ; id < m_spawners.size() ; ++id) {
      m_spawners[id]->step(si);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->step(si);
    }

    // Process influences.
    for (unsigned id = 0u ; id < si.mSpawned.size() ; ++id) {
      m_mobs.push_back(si.mSpawned[id]);
    }

    for (unsigned id = 0u ; id < si.mRemoved.size() ; ++id) {
      auto toRm = std::find_if(
        m_mobs.cbegin(),
        m_mobs.cend(),
        [&si, &id](const MobShPtr& m) {
          return m != nullptr && m.get() == si.mRemoved[id];
        }
      );
      if (toRm != m_mobs.end()) {
        m_mobs.erase(toRm);
      }
    }
  }

  void
  World::generate() {
    // TODO: Restore this.
  }

  void
  World::loadFromFile(const std::string& file) {
    // Open the file.
    std::ifstream in(file.c_str());

    if (!in.good()) {
      error(
        std::string("Unable to load level from file \"") + file + "\"",
        std::string("No such file")
      );
    }

    // Read the dimensions of the world.
    loadDimensions(in);

    error(
      "Loading from file is not implemented",
      "COME BACK LATER"
    );
  }

}
