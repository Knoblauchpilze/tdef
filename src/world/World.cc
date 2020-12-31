
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/TimeUtils.hh>
# include "Spawner.hh"
# include "Wall.hh"
# include "Portal.hh"
# include "Tower.hh"
# include "TowerFactory.hh"

namespace tdef {

  World::World(int seed):
    utils::CoreObject("world"),

    m_rng(seed),

    m_blocks(),
    m_mobs(),

    m_loc(nullptr),

    onGoldEarned()
  {
    setService("world");

    generate();
  }

  World::World(int seed,
               const std::string& file):
    utils::CoreObject("world"),

    m_rng(seed),

    m_blocks(),
    m_mobs(),

    m_loc(nullptr),

    onGoldEarned()
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

      0.0f,                    // gold
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

    // Handle cases where some gold was earned.
    if (si.gold > 0.0f) {
      onGoldEarned.safeEmit("gold earned signal", si.gold);
    }
  }

  void
  World::spawn(BlockShPtr block) {
    if (block == nullptr) {
      log("Failed to spawn null block", utils::Level::Error);
      return;
    }

    m_blocks.push_back(block);

    // Update elements.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->worldUpdate(m_loc);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->worldUpdate(m_loc);
    }
  }

  void
  World::generate() {
    static constexpr int sk_spawners = 3;
    static constexpr int sk_walls = 2;
    static constexpr int sk_portals = 1;
    static constexpr int sk_towers = 4;

    static constexpr int sk_mobs = 1;

    // Assume a certain width and height to
    // prevent collision.
    unsigned w = 10;
    unsigned h = 5;

    utils::Point2f p;
    int key = 0;
    std::unordered_set<int> used;

    int id = sk_spawners;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        log("Spawner at " + p.toString());
        SpawnerShPtr b = std::make_shared<Spawner>(Spawner::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_walls;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        log("Wall at " + p.toString());
        WallShPtr b = std::make_shared<Wall>(Wall::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_portals;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        log("Portal at " + p.toString());
        PortalShPtr b = std::make_shared<Portal>(Portal::newProps(p));
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_towers;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        log("Tower at " + p.toString());

        Tower::TProps pp;
        towers::Data td;

        if (id % 4 == 0) {
          pp = TowerFactory::generateBasicTowerProps(p);
          td = TowerFactory::generateBasicTowerData();
        }
        else if (id % 4 == 1) {
          pp = TowerFactory::generateSniperTowerProps(p);
          td = TowerFactory::generateSniperTowerData();
        }
        else if (id % 4 == 2) {
          pp = TowerFactory::generateSlowTowerProps(p);
          td = TowerFactory::generateSlowTowerData();
        }
        else {
          pp = TowerFactory::generateCannonTowerProps(p);
          td = TowerFactory::generateCannonTowerData();
        }

        TowerShPtr b = std::make_shared<Tower>(pp, td);
        m_blocks.push_back(b);
        --id;
      }
    }

    id = sk_mobs;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f);
      p.y() = m_rng.rndInt(0.0f, h - 1.0f);

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        Mob::MProps mp = Mob::newProps(p);

        if (id % 4 == 0) {
          mp.type = mobs::Type::Regular;
        }
        else if(id % 4 == 1) {
          mp.type = mobs::Type::Fast;
        }
        else if(id % 4 == 2) {
          mp.type = mobs::Type::Strong;
        }
        else {
          mp.type = mobs::Type::Heli;
        }

        MobShPtr m = std::make_shared<Mob>(mp);
        m_mobs.push_back(m);

        log("Generating mob at " + std::to_string(p.x()) + "x" + std::to_string(p.y()) + " with radius " + std::to_string(mp.radius));

        --id;
      }
    }

    m_loc = std::make_shared<Locator>(m_blocks, m_mobs);
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

    error(
      "Loading from file is not implemented",
      "COME BACK LATER"
    );
  }

}
