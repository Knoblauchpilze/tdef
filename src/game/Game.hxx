#ifndef    GAME_HXX
# define   GAME_HXX

# include "Game.hh"

namespace tdef {

  inline
  int
  Game::w() const noexcept {
    return m_world->w();
  }

  inline
  int
  Game::h() const noexcept {
    return m_world->h();
  }

  inline
  void
  Game::setTowerType(const towers::Type& type) {
    m_tType = std::make_shared<towers::Type>(type);
    m_wallBuilding = false;
  }

  inline
  void
  Game::allowWallBuilding() {
    m_tType.reset();
    m_wallBuilding = true;
  }

  inline
  std::vector<world::ItemEntry>
  Game::getVisible(float xMin,
                   float yMin,
                   float xMax,
                   float yMax,
                   const world::ItemType* type,
                   const world::Filter* filters,
                   world::Sort sort) const noexcept
  {
    return m_loc->getVisible(xMin, yMin, xMax, yMax, type, filters, sort);
  }

  inline
  world::Block
  Game::block(int id) const noexcept {
    return m_loc->block(id);
  }

  inline
  world::Mob
  Game::mob(int id) const noexcept {
    return m_loc->mob(id);
  }

  inline
  void
  Game::step(float tDelta) {
    m_world->step(tDelta);

    // TODO: Include update of the menus here.
  }

  inline
  void
  Game::pause(float tDelta) {
    m_world->pause(tDelta);
  }

  inline
  void
  Game::resume(float tDelta) {
    m_world->resume(tDelta);
  }

}

#endif    /* GAME_HXX */
