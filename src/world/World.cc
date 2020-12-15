
# include "World.hh"
# include <algorithm>
# include <unordered_set>
# include <core_utils/TimeUtils.hh>

namespace tdef {

  World::World(int seed, int width, int height):
    utils::CoreObject("world"),

    m_w(width),
    m_h(height),

    m_rng(seed)
  {
    setService("world");

    generate();
  }

  World::World(int seed,
               const std::string& file):
    utils::CoreObject("world"),

    m_w(0),
    m_h(0),

    m_rng(seed)
  {
    // Check dimensions.
    setService("world");

    loadFromFile(file);
  }

  void
  World::step(float /*tDelta*/) {
    // TODO: Handle this.
  }

  void
  World::generate() {
    // TODO: Handle this.
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
  }

}
