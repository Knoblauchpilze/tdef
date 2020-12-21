
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/TimeUtils.hh>
# include "Spawner.hh"
# include "Wall.hh"
# include "Portal.hh"
# include "Tower.hh"

namespace tdef {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed),

    m_blocks(),
    m_mobs(),

    m_loc(nullptr)
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

    m_blocks(),
    m_mobs(),

    m_loc(nullptr)
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

      m_loc,                   // frustum

      std::vector<MobShPtr>(), // mSpawned
      std::vector<Mob*>(),     // mRemoved
    };

    // Make elements evolve.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->step(si);
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
    static constexpr int sk_spawners = 3;
    static constexpr int sk_walls = 2;
    static constexpr int sk_portals = 1;
    static constexpr int sk_towers = 4;

    static constexpr int sk_mobs = 2;

    utils::Point2f p;
    int key = 0;
    std::unordered_set<int> used;

    int id = sk_spawners;
    while (id > 0) {
      p.x() = m_rng.rndFloat(0.0f, m_w - 1.0f);
      p.y() = m_rng.rndFloat(0.0f, m_h - 1.0f);

      key = static_cast<int>(p.y() * m_w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        SpawnerShPtr b = std::make_shared<Spawner>(Spawner::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_walls;
    while (id > 0) {
      p.x() = m_rng.rndFloat(0.0f, m_w - 1.0f);
      p.y() = m_rng.rndFloat(0.0f, m_h - 1.0f);

      key = static_cast<int>(p.y() * m_w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        WallShPtr b = std::make_shared<Wall>(Wall::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_portals;
    while (id > 0) {
      p.x() = m_rng.rndFloat(0.0f, m_w - 1.0f);
      p.y() = m_rng.rndFloat(0.0f, m_h - 1.0f);

      key = static_cast<int>(p.y() * m_w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        PortalShPtr b = std::make_shared<Portal>(Portal::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_towers;
    while (id > 0) {
      p.x() = m_rng.rndFloat(0.0f, m_w - 1.0f);
      p.y() = m_rng.rndFloat(0.0f, m_h - 1.0f);

      key = static_cast<int>(p.y() * m_w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        TowerShPtr b = std::make_shared<Tower>(Tower::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }


    id = sk_mobs;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, m_w - 1.0f);
      p.y() = m_rng.rndInt(0.0f, m_h - 1.0f);

      key = static_cast<int>(p.y() * m_w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        Mob::MProps mp = Mob::newProps(p);

        if (id % 2 == 0) {
          mp.radius = 1.5f;
        } else {
          mp.radius = 0.5f;
        }

        MobShPtr m = std::make_shared<Mob>(mp);
        m_mobs.push_back(m);

        log("Generating mob at " + std::to_string(p.x()) + "x" + std::to_string(p.y()) + " with radius " + std::to_string(mp.radius));

        --id;
      }
    }

    m_loc = std::make_shared<Locator>(m_w, m_h, m_blocks, m_mobs);
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
