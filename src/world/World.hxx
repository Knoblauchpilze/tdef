#ifndef    WORLD_HXX
# define   WORLD_HXX

# include "World.hh"

namespace tdef {

  inline
  World::~World() {}

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

}

#endif    /* WORLD_HXX */
