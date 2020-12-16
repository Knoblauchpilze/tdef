
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

    m_regions()
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

    m_regions()
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
  }

  world::State
  World::cell(int x, int y, bool& valid) const noexcept {
    valid = false;

    if (x < 0 || x >= m_w) {
      return world::State::Empty;
    }
    if (y < 0 || y >= m_h) {
      return world::State::Empty;
    }

    valid = true;

    // TODO: Assume a single region.
    return m_regions[0].cells[y * sk_regionSize + x];
  }

  void
  World::step(float /*tDelta*/) {
    // TODO: Handle this.
  }

  void
  World::generate() {
    // Generate a single block.
    Region r;
    r.x = 0;
    r.y = 0;

    // Generate random colonies and deposits.
    static constexpr int sk_colonies = 10;
    static constexpr int sk_deposits = 5;

    int id = sk_colonies;
    while (id > 0) {
      int x = m_rng.rndInt(0, sk_regionSize - 1);
      int y = m_rng.rndInt(0, sk_regionSize - 1);

      if (r.cells[y * sk_regionSize + x] == world::State::Empty) {
        r.cells[y * sk_regionSize + x] = world::State::Colony;

        --id;
      }
    }

    id = sk_deposits;
    while (id > 0) {
      int x = m_rng.rndInt(0, sk_regionSize - 1);
      int y = m_rng.rndInt(0, sk_regionSize - 1);

      if (r.cells[y * sk_regionSize + x] == world::State::Empty) {
        r.cells[y * sk_regionSize + x] = world::State::Deposit;

        --id;
      }
    }


    // Generate random workers and warriors.
    static constexpr int sk_workers = 3;
    static constexpr int sk_warriors = 2;

    id = sk_workers;
    while (id > 0) {
      int x = m_rng.rndInt(0, sk_regionSize - 1);
      int y = m_rng.rndInt(0, sk_regionSize - 1);

      if (r.cells[y * sk_regionSize + x] == world::State::Empty) {
        r.cells[y * sk_regionSize + x] = world::State::Worker;

        --id;
      }
    }

    id = sk_warriors;
    while (id > 0) {
      int x = m_rng.rndInt(0, sk_regionSize - 1);
      int y = m_rng.rndInt(0, sk_regionSize - 1);

      if (r.cells[y * sk_regionSize + x] == world::State::Empty) {
        r.cells[y * sk_regionSize + x] = world::State::Warrior;

        --id;
      }
    }

    m_regions.push_back(r);
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
