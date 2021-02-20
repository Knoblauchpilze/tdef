
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

    m_paused(true),

    m_loc(nullptr),

    onGoldEarned()
  {
    setService("world");

    generate(world::Difficulty::Normal);
    initialize();
  }

  void
  World::step(float tDelta) {
    if (m_paused) {
      return;
    }

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
  World::pause() {
    // Pause each element if needed.
    if (m_paused) {
      return;
    }

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

    m_paused = true;
  }

  void
  World::resume() {
    // Resume each element if needed.
    if (!m_paused) {
      return;
    }

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

    m_paused = false;
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
  World::reset(unsigned metadataSize,
               const std::string& file,
               const world::Difficulty& difficulty)
  {
    // Clear all registered elements.
    m_blocks.clear();
    m_mobs.clear();
    m_projectiles.clear();

    // Regenerate the world.
    if (file.empty()) {
      generate(difficulty);
    }
    else {
      loadFromFile(file, metadataSize);
    }

    m_paused = true;
  }

  void
  World::save(const std::string& file) const {
    // Open the file. Note that we assume that
    // the file has already been processed by
    // the game where info about the game state
    // has been saved so we need to append the
    // world's data to the existing content.
    std::ofstream out(file.c_str(), std::ofstream::app);

    if (!out.good()) {
      error(
        "Failed to save world to \"" + file + "\"",
        "Failed to open file"
      );
    }

    log("Saving world to \"" + file + "\"", utils::Level::Info);

    // Indicate that a valid rng was saved for this
    // world and save the corresponding resource.
    out << true << " ";
    out << m_rng << " ";

    int count = 0;

    // Save towers.
    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Tower>(m_blocks[id]) != nullptr) {
        ++count;
      }
    }
    out << count << " ";
    
    log("Saving " + std::to_string(count) + " tower(s)", utils::Level::Verbose);

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
    out << count << " ";
    log("Saving " + std::to_string(count) + " portal(s)", utils::Level::Verbose);

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
    out << count << " ";
    log("Saving " + std::to_string(count) + " spawner(s)", utils::Level::Verbose);

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
    out << count << " ";
    log("Saving " + std::to_string(count) + " wall(s)", utils::Level::Verbose);

    for (unsigned id = 0u ; id < m_blocks.size() ; ++id) {
      if (std::dynamic_pointer_cast<Wall>(m_blocks[id]) != nullptr) {
        out << *m_blocks[id];
      }
    }

    // Save mobs.
    log("Saving " + std::to_string(m_mobs.size()) + " mob(s)", utils::Level::Verbose);

    out << m_mobs.size() << " ";
    for (unsigned id = 0u ; id < m_mobs.size() ; ++id) {
      out << *m_mobs[id];
    }

    // Save projectiles.
    log("Saving " + std::to_string(m_projectiles.size()) + " projectile(s)", utils::Level::Verbose);

    out << m_projectiles.size() << " ";
    for (unsigned id = 0u ; id < m_projectiles.size() ; ++id) {
      out << *m_projectiles[id];
    }
  }

  void
  World::generate(const world::Difficulty& difficulty) {
    // We want to generate spawners, walls and a single portal
    // so that mobs try to access it. The number of portals or
    // walls is dependant on the difficulty.
    // The portal is always at the center of the world. So that
    // we don't put a block on top of another we will maintain
    // an map allowing to keep track of which 'cells' have
    // alreadt been occupied.
    float min = -sk_worldSize / 2.0f;
    float max = sk_worldSize / 2.0f;
    auto keyGen = [&min](const utils::Point2f& p) {
      return static_cast<int>((p.y() - min) * sk_worldSize) + static_cast<int>(p.x() - min);
    };
    std::unordered_set<int> used;

    utils::Point2f p(0.5f, 0.5f);
    PortalShPtr b = std::make_shared<Portal>(Portal::newProps(p));
    log(
      "Generated portal at " + p.toString() + " with " +
      std::to_string(b->getLives()) + " live(s)",
      utils::Level::Verbose
    );

    m_blocks.push_back(b);
    used.insert(keyGen(p));

    // Determine how many spawners and walls we need.
    int spawners = 0;
    int walls = 0;

    switch (difficulty) {
      case world::Difficulty::Easy:
        spawners = 1;
        walls = 50;
        break;
      case world::Difficulty::Hard:
        spawners = 4;
        walls = 10;
        break;
      case world::Difficulty::Normal:
      default:
        spawners = 2;
        walls = 25;
        break;
    }

    // Generate spawners: they will be located on the
    // border of the world so that the mobs have some
    // distance to traverse before reaching the portal.
    int id = spawners;
    spawners::Level lvl = spawners::Level::Normal;

    while (id > 0) {
      p.x() = m_rng.rndInt(min, max) + 0.5f;
      p.y() = m_rng.rndInt(min, max) + 0.5f;

      int key = keyGen(p);

      if (used.count(key) == 0) {
        SpawnerShPtr b = std::make_shared<Spawner>(spawners::generateProps(p, lvl));
        m_blocks.push_back(b);
        used.insert(key);

        log("Generated " + spawners::toString(lvl) + " spawner at " + p.toString(), utils::Level::Debug);

        --id;
      }
    }

    // Generate walls.
    id = walls;
    while (id > 0) {
      p.x() = m_rng.rndInt(min, max) + 0.5f;
      p.y() = m_rng.rndInt(min, max) + 0.5f;

      int key = keyGen(p);

      if (used.count(key) == 0) {
        WallShPtr b = std::make_shared<Wall>(Wall::newProps(p));
        m_blocks.push_back(b);
        used.insert(key);

        log("Generated wall at " + p.toString(), utils::Level::Verbose);

        --id;
      }
    }
  }

  void
  World::initialize() {
    m_loc = std::make_shared<Locator>(m_blocks, m_mobs, m_projectiles);
  }

  void
  World::loadFromFile(const std::string& file,
                      unsigned metadataSize)
  {
    // Open the file.
    std::ifstream in(file.c_str());

    if (!in.good()) {
      error(
        "Failed to load world from \"" + file + "\"",
        "Failed to open file"
      );
    }

    // We want to skip the metadata corresponding to data
    // that might not be relevant to the world. We will
    // just ignore it.
    std::vector<char> dummy(metadataSize);
    in.read(dummy.data(), metadataSize);

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

    log("Loading " + std::to_string(count) + " tower(s)");

    for (int id = 0 ; id < count ; ++id) {
      TowerShPtr e = std::make_shared<Tower>(Tower::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load portals.
    in >> count;

    log("Loading " + std::to_string(count) + " portal(s)");

    for (int id = 0 ; id < count ; ++id) {
      PortalShPtr e = std::make_shared<Portal>(Portal::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load spawners.
    in >> count;

    log("Loading " + std::to_string(count) + " spawner(s)");

    for (int id = 0 ; id < count ; ++id) {
      SpawnerShPtr e = std::make_shared<Spawner>(Spawner::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load walls.
    in >> count;

    log("Loading " + std::to_string(count) + " wall(s)");

    for (int id = 0 ; id < count ; ++id) {
      WallShPtr e = std::make_shared<Wall>(Wall::newProps(utils::Point2f()));
      in >> *e;

      m_blocks.push_back(e);
    }

    // Load mobs if any.
    in >> count;

    log("Loading " + std::to_string(count) + " mob(s)");

    for (int id = 0 ; id < count ; ++id) {
      MobShPtr e = std::make_shared<Mob>(Mob::newProps(utils::Point2f()));
      in >> *e;

      m_mobs.push_back(e);
    }

    // Load projectiles.
    in >> count;

    log("Loading " + std::to_string(count) + " projectile(s)");

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
