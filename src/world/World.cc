
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/TimeUtils.hh>
# include "Spawner.hh"
# include "Wall.hh"
# include "Portal.hh"
# include "Tower.hh"
# include "TowerFactory.hh"
# include "MobFactory.hh"
# include "SpawnerFactory.hh"

namespace tdef {

  World::World(int seed):
    utils::CoreObject("world"),

    m_rng(seed),

    m_blocks(),
    m_mobs(),
    m_projectiles(),

    m_loc(nullptr),

    onGoldEarned()
  {
    setService("world");

    generate();
    initialize();
  }

  World::World(const std::string& file):
    utils::CoreObject("world"),

    m_rng(),

    m_blocks(),
    m_mobs(),
    m_projectiles(),

    m_loc(nullptr),

    onGoldEarned()
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
    initialize();
  }

  void
  World::step(float tDelta) {
    StepInfo si{
      m_rng,                          // rng

      utils::now(),                   // moment
      tDelta,                         // elapsed

      m_loc,                          // frustum

      std::vector<MobShPtr>(),        // mSpawned
      std::vector<ProjectileShPtr>(), // pSpawned

      0.0f,                           // gold
    };

    // Make elements evolve.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->step(si);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->step(si);
    }

    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      m_projectiles[id]->step(si);
    }

    // Process influences.
    for (unsigned id = 0u ; id < si.mSpawned.size() ; ++id) {
      m_mobs.push_back(si.mSpawned[id]);
    }

    for (unsigned id = 0u ; id < si.pSpawned.size() ; ++id) {
      m_projectiles.push_back(si.pSpawned[id]);
    }

    // Remove elements marked for deletion.
    forceDelete();

    // Handle cases where some gold was earned.
    if (si.gold > 0.0f) {
      onGoldEarned.safeEmit("gold earned signal", si.gold);
    }
  }

  void
  World::pause(float /*tDelta*/) {
    // Pause each element.
    utils::TimeStamp t = utils::now();

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->pause(t);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->pause(t);
    }

    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      m_projectiles[id]->pause(t);
    }
  }

  void
  World::resume(float /*tDelta*/) {
    // Resume each element.
    utils::TimeStamp t = utils::now();

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->resume(t);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->resume(t);
    }

    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      m_projectiles[id]->resume(t);
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
    onWorldUpdate();
  }

  void
  World::forceDelete() {
    // First remove blocks.
    std::size_t bs = m_blocks.size();
    std::size_t ms = m_mobs.size();
    std::size_t ps = m_projectiles.size();

    m_blocks.erase(
      std::remove_if(
        m_blocks.begin(),
        m_blocks.end(),
        [](BlockShPtr block){
          return block->isDeleted();
        }
      ),
      m_blocks.end()
    );

    // Then remove mobs.
    m_mobs.erase(
      std::remove_if(
        m_mobs.begin(),
        m_mobs.end(),
        [](MobShPtr mob){
          return mob->isDeleted();
        }
      ),
      m_mobs.end()
    );

    // And finish with projectiles.
    m_projectiles.erase(
      std::remove_if(
        m_projectiles.begin(),
        m_projectiles.end(),
        [](ProjectileShPtr proj){
          return proj->isDeleted();
        }
      ),
      m_projectiles.end()
    );

    // Update remaining elements if anything has
    // been removed.
    if (bs != m_blocks.size() ||
        ms != m_mobs.size() ||
        ps != m_projectiles.size())
    {
      onWorldUpdate();
    }
  }

  void
  World::reset(const std::string& file) {
    // Clear all registered elements.
    m_blocks.clear();
    m_mobs.clear();
    m_projectiles.clear();

    // Regenerate the world.
    if (file.empty()) {
      generate();
    }
    else {
      loadFromFile(file);
    }
  }

  void
  World::save(const std::string& file) const {
    // Open the file.
    std::ofstream out(file.c_str());

    if (!out.good()) {
      error(
        "Failed to save world to \"" + file + "\"",
        "Failed to open file"
      );
    }

    // Indicate that a valid rng was saved for this
    // world and save the corresponding resource.
    out << true;
    out << m_rng;

    int count = 0;

    // Save towers.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Tower>(m_blocks[id]) != nullptr) {
        ++count;
      }
    }
    out << count;

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Tower>(m_blocks[id]) != nullptr) {
        out << *m_blocks[id];
      }
    }

    // Save portals.
    count = 0;
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Portal>(m_blocks[id]) != nullptr) {
        ++count;
      }
    }
    out << count;

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Portal>(m_blocks[id]) != nullptr) {
        out << *m_blocks[id];
      }
    }

    // Save spawners.
    count = 0;
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Spawner>(m_blocks[id]) != nullptr) {
        ++count;
      }
    }
    out << count;

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Spawner>(m_blocks[id]) != nullptr) {
        out << *m_blocks[id];
      }
    }

    // Save walls.
    count = 0;
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Wall>(m_blocks[id]) != nullptr) {
        ++count;
      }
    }
    out << count;

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Wall>(m_blocks[id]) != nullptr) {
        out << *m_blocks[id];
      }
    }

    // Save mobs.
    out << m_mobs.size();
    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      out << *m_mobs[id];
    }

    // Save projectiles.
    out << m_projectiles.size();
    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      out << *m_projectiles[id];
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
        spawners::Level lvl = spawners::Level::Normal;

        log("Generated " + spawners::toString(lvl) + " spawner at " + p.toString(), utils::Level::Verbose);

        Spawner::SProps pp = spawners::generateProps(p, lvl);
        pp.difficulty = lvl;

        SpawnerShPtr b = std::make_shared<Spawner>(pp);
        m_blocks.push_back(b);
        --id;

        used.insert(key);
      }
    }

    id = sk_walls;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        log("Generated wall at " + p.toString(), utils::Level::Verbose);

        WallShPtr b = std::make_shared<Wall>(Wall::newProps(p));
        m_blocks.push_back(b);
        --id;

        used.insert(key);
      }
    }

    id = sk_portals;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        Portal::PProps pp = Portal::newProps(p);
        log(
          "Generated portal at " + p.toString() + " with " +
          std::to_string(pp.lives) + " live(s)",
          utils::Level::Verbose
        );

        PortalShPtr b = std::make_shared<Portal>(pp);
        m_blocks.push_back(b);
        --id;

        used.insert(key);
      }
    }

    id = sk_towers;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f) + 0.5f;
      p.y() = m_rng.rndInt(0.0f, h - 1.0f) + 0.5f;

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      if (used.count(key) == 0) {
        Tower::TProps pp;

        if (id % 4 == 0) {
          pp = towers::generateProps(towers::Type::Multishot, p);
        }
        else if (id % 4 == 1) {
          pp = towers::generateProps(towers::Type::Sniper, p);
        }
        else if (id % 4 == 2) {
          pp = towers::generateProps(towers::Type::Freezing, p);
        }
        else {
          pp = towers::generateProps(towers::Type::Venom, p);
        }

        log("Generated tower " + towers::toString(pp.type) + " at " + p.toString(), utils::Level::Verbose);

        TowerShPtr b = std::make_shared<Tower>(pp);
        m_blocks.push_back(b);
        --id;

        used.insert(key);
      }
    }

    id = sk_mobs;
    while (id > 0) {
      p.x() = m_rng.rndInt(0.0f, w - 1.0f);
      p.y() = m_rng.rndInt(0.0f, h - 1.0f);

      key = static_cast<int>(p.y() * w) + static_cast<int>(p.x());

      Mob::MProps pp;

      if (used.count(key) == 0) {
        if (id % 4 == 0) {
          pp = mobs::generateProps(mobs::Type::Regular, p);
        }
        else if(id % 4 == 1) {
          pp = mobs::generateProps(mobs::Type::Fast, p);
        }
        else if(id % 4 == 2) {
          pp = mobs::generateProps(mobs::Type::Strong, p);
        }
        else {
          pp = mobs::generateProps(mobs::Type::Heli, p);
        }

        MobShPtr m = std::make_shared<Mob>(pp);
        m_mobs.push_back(m);

        log(
          "Generated mob " + mobs::toString(pp.type) +
          " at " + p.toString() +
          " with radius " + std::to_string(pp.radius),
          utils::Level::Verbose
        );

        --id;

        used.insert(key);
      }
    }
  }

  void
  World::initialize() {
    m_loc = std::make_shared<Locator>(m_blocks, m_mobs, m_projectiles);
  }

  void
  World::loadFromFile(const std::string& file) {
    // Open the file.
    std::ifstream in(file.c_str());

    if (!in.good()) {
      error(
        "Failed to load world from \"" + file + "\"",
        "Failed to open file"
      );
    }

    // First thing is to determine whether a valid
    // rng has been saved.
    log("Loading rng engine");

    bool rngDefined;
    in >> rngDefined;
    if (rngDefined) {
      in >> m_rng;
    }

    int count = 0;

    // Load towers if any.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      TowerShPtr e = std::make_shared<Tower>(Tower::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load portals.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      PortalShPtr e = std::make_shared<Portal>(Portal::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load spawners.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      SpawnerShPtr e = std::make_shared<Spawner>(Spawner::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load walls.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      WallShPtr e = std::make_shared<Wall>(Wall::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load mobs if any.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      MobShPtr e = std::make_shared<Mob>(Mob::newProps(utils::Point2f()));
      in >> *e;

      m_mobs.push_back(e);
    }

    // Load projectiles.
    in >> count;
    for (int id = 0 ; id < count ; ++id) {
      ProjectileShPtr e = std::make_shared<Projectile>(
        Projectile::newProps(utils::Point2f()),
        nullptr,
        nullptr
      );
      in >> *e;

      m_projectiles.push_back(e);
    }
  }

  void
  World::onWorldUpdate() {
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      m_blocks[id]->worldUpdate(m_loc);
    }

    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      m_mobs[id]->worldUpdate(m_loc);
    }

    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      m_projectiles[id]->worldUpdate(m_loc);
    }
  }

}
