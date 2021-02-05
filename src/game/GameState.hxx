#ifndef    GAME_STATE_HXX
# define   GAME_STATE_HXX

# include "GameState.hh"

namespace tdef {

  inline
  const game::Screen&
  GameState::getScreen() const noexcept {
    return m_screen;
  }

  inline
  const std::string&
  GameState::getWorldFile() const noexcept {
    return m_worldFile;
  }

}

#endif    /* GAME_STATE_HXX */
