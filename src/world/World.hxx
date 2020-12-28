#ifndef    WORLD_HXX
# define   WORLD_HXX

# include "World.hh"

namespace tdef {

  inline
  World::~World() {}

  inline
  unsigned
  World::w() const noexcept {
    return m_w;
  }

  inline
  unsigned
  World::h() const noexcept {
    return m_h;
  }

  inline
  LocatorShPtr
  World::locator() const noexcept {
    return m_loc;
  }

  inline
  void
  World::pause(float /*tDelta*/) {
    // Nothing to do for now.
  }

  inline
  void
  World::resume(float /*tDelta*/) {
    // Nothing to do for now.
  }

  inline
  void
  World::setTowerType(const towers::Type& type) {
    m_tType = std::make_shared<towers::Type>(type);
  }

  inline
  void
  World::loadDimensions(std::ifstream& in) {
    // Read each dimension: should be on separate lines.
    in >> m_w;
    if (in.eof()) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions")
      );
    }
    in >> m_h;

    if (m_w <= 0 || m_h <= 0) {
      error(
        std::string("Unable to load level from file"),
        std::string("Invalid dimensions ") + std::to_string(m_w) +
        "x" + std::to_string(m_h)
      );
    }
  }

}

#endif    /* WORLD_HXX */
