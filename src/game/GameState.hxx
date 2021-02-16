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

  inline
  std::string
  GameState::generateSaveFileName() const noexcept {
    // Start from the base directory and base name.
    std::string fn(sk_savedGamesDir);
    fn += "/";
    fn += sk_baseWorldName;

    // Add a suffix corresponding to the number of games
    // currently registered in the directory.
    fn += std::to_string(m_savedGames.saves.size());

    // Append the extension.
    fn += sk_saveGameExtension;

    return fn;
  }

}

#endif    /* GAME_STATE_HXX */
