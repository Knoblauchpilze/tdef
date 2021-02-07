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

  inline
  void
  GameState::setWorldFile(const std::string& file) {
    log("Setting world file to \"" + file + "\"");
    m_worldFile = file;
  }

}

#endif    /* GAME_STATE_HXX */
