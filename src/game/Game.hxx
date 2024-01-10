#ifndef    GAME_HXX
# define   GAME_HXX

# include "Game.hh"

namespace tdef {

  inline
  Game::~Game() {
    // Disconnect the gold earned slot.
    if (m_statusDisplay.goldEarnedSlot >= 0) {
      m_world->onGoldEarned.disconnect(m_statusDisplay.goldEarnedSlot);
    }
  }

  inline
  void
  Game::setTowerType(const towers::Type& type) {
    // Prevent actions to happen in case the UI is disabled.
    if (m_state.disabled) {
      verbose("Ignoring tower type as game is disabled");
      return;
    }

    info("Set tower type to " + towers::toString(type));
    m_state.tType = std::make_shared<towers::Type>(type);
    m_state.wallBuilding = false;
  }

  inline
  void
  Game::allowWallBuilding() {
    // Prevent actions to happen in case the UI is disabled.
    if (m_state.disabled) {
      return;
    }

    m_state.tType.reset();
    m_state.wallBuilding = true;
  }

  inline
  void
  Game::terminate() noexcept {
    info("Game has been terminated");
    m_state.terminated = true;
  }

  inline
  bool
  Game::terminated() const noexcept {
    return m_state.terminated;
  }

  inline
  void
  Game::pause() {
    // Do nothing in case the game is already paused.
    if (m_state.paused) {
      return;
    }

    m_world->pause();
    m_state.paused = true;
  }

  inline
  void
  Game::resume() {
    // Do nothing in case the game is already running.
    if (!m_state.paused) {
      return;
    }

    m_world->resume();
    m_state.paused = false;
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
  world::Projectile
  Game::projectile(int id) const noexcept {
    return m_loc->projectile(id);
  }

  inline
  void
  Game::updateGold(float earned) {
    m_state.gold += earned;
  }

}

#endif    /* GAME_HXX */
