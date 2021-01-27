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

}

#endif    /* WORLD_HXX */
