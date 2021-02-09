
# include "GameState.hh"
# include <filesystem>
# include <algorithm>
# include "SimpleMenu.hh"
# include "SimpleAction.hh"
# include "Game.hh"

namespace {

  // Convenience using to shorten the usage of the filesystem
  // data types when loading the saved games list.
  using DirIt = std::filesystem::directory_iterator;

  tdef::MenuShPtr
  generateDefaultScreen(const olc::vi2d& dims,
                        const olc::Pixel& color,
                        const olc::vi2d& menuSize = olc::vi2d(350, 200))
  {
    const olc::vi2d size(std::min(dims.x, menuSize.x), std::min(dims.y, menuSize.y));
    const olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

    tdef::menu::BackgroundDesc bg = tdef::menu::newColoredBackground(color);
    tdef::menu::MenuContentDesc fg = tdef::menu::newTextContent("");

    return std::make_shared<tdef::Menu>(
      pos,
      size,
      "goMenu",
      bg,
      fg,
      tdef::menu::Layout::Vertical,
      // The menus can be highlighted but don't have a
      // selected behavior.
      true,
      false
    );
  }

  tdef::GameMenuShPtr
  generateScreenOption(const std::string& text,
                       const olc::Pixel& bgColor,
                       const olc::Pixel& textColor,
                       const olc::Pixel& textHighlighColor)
  {
    tdef::menu::BackgroundDesc bg = tdef::menu::newColoredBackground(bgColor);
    tdef::menu::MenuContentDesc fg = tdef::menu::newTextContent(
      text,
      textColor,
      tdef::menu::Alignment::Center
    );
    fg.hColor = textHighlighColor;

    tdef::GameMenuShPtr m = std::make_shared<tdef::GameMenu>(
      bg,
      fg,
      [](std::vector<tdef::ActionShPtr>& /*actions*/) {
        // Dummy action.
      }
    );

    // Screen options are not selectable.
    m->setSelectable(false);

    return m;
  }

}

namespace tdef {

  const olc::Pixel GameState::sk_screenBGColor(20, 20, 20, alpha::Opaque);
  const olc::Pixel GameState::sk_menuBGColor(90, 90, 90, alpha::Opaque);
  const olc::Pixel GameState::sk_menuTextColor(180, 180, 180, alpha::Opaque);
  const olc::Pixel GameState::sk_menuTextColorHighlight(30, 30, 30, alpha::Opaque);

  GameState::GameState(const olc::vi2d& dims,
                       const game::Screen& screen):
    utils::CoreObject("state"),

    // Assign a different screen so that we can use the
    // `setScreen` routine to initialize the visibility
    // status of screens.
    m_screen(screen == game::Screen::Home ? game::Screen::Exit : screen),
    m_worldFile(),

    m_homeScreen(nullptr),
    m_loadGameScreen(nullptr),
    m_savedGames(SavedGamesData{
      std::vector<std::string>(),   // saves
      0u,                           // index
      sk_savedGamesPerPage,         // gamesPerPage
      std::vector<GameMenuShPtr>(), // games
      nullptr,                      // previous
      nullptr                       // next
    }),
    m_pauseScreen(nullptr),
    m_gameOverScreen(nullptr)
  {
    setService("game");

    // Generate each screen.
    generateHomeScreen(dims);
    generateLoadGameScreen(dims);
    generatePauseScreen(dims);
    generateGameOverScreen(dims);

    // Assign the desired screen, which will also take care
    // of assigning visibility.
    setScreen(screen);
  }

  void
  GameState::setScreen(const game::Screen& screen) {
    if (m_screen == screen) {
      return;
    }

    // Assign the state.
    m_screen = screen;

    // Update screens' visibility.
    m_homeScreen->setVisible(m_screen == game::Screen::Home);
    m_loadGameScreen->setVisible(m_screen == game::Screen::LoadGame);
    m_pauseScreen->setVisible(m_screen == game::Screen::Pause);
    m_gameOverScreen->setVisible(m_screen == game::Screen::GameOver);
  }

  void
  GameState::render(olc::PixelGameEngine* pge) const {
    m_homeScreen->render(pge);
    m_loadGameScreen->render(pge);
    m_pauseScreen->render(pge);
    m_gameOverScreen->render(pge);
  }

  menu::InputHandle
  GameState::processUserInput(const controls::State& c,
                              std::vector<ActionShPtr>& actions)
  {
    menu::InputHandle res{false, false};

    // Propagate the user input to each screen.
    menu::InputHandle cur = m_homeScreen->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    cur = m_loadGameScreen->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    cur = m_pauseScreen->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    cur = m_gameOverScreen->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    return res;
  }

  void
  GameState::generateHomeScreen(const olc::vi2d& dims) {
    // Generate base menu.
    m_homeScreen = generateDefaultScreen(dims, sk_screenBGColor);

    // Create each option.
    SimpleMenuShPtr m = generateScreenOption("New game", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        // Generate a new game.
        setWorldFile("");
        g.reset(getWorldFile());

        setScreen(game::Screen::Game);
      }
    );
    m_homeScreen->addMenu(m);

    m = generateScreenOption("Load game", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        // Refresh saved games list and display from the
        // first entry.
        refreshSavedGames();
        m_savedGames.index = 0;
        updateSavedGamesDisplay();

        setScreen(game::Screen::LoadGame);
      }
    );
    m_homeScreen->addMenu(m);

    m = generateScreenOption("Quit", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        // Assign the correct screen and terminate the game.
        setScreen(game::Screen::Exit);
        g.terminate();
      }
    );
    m_homeScreen->addMenu(m);
  }

  void
  GameState::generateLoadGameScreen(const olc::vi2d& dims) {
    // Generate base menu: note that as we will be displaying
    // quite a lot of information we want this menu to be a
    // bit larger than the other ones.
    m_loadGameScreen = generateDefaultScreen(dims, sk_screenBGColor, olc::vi2d(350, 400));

    menu::BackgroundDesc bg = menu::newColoredBackground(sk_menuBGColor);
    menu::MenuContentDesc fg = menu::newTextContent("Saved games:", olc::MAGENTA, menu::Alignment::Center);
    MenuShPtr tm = std::make_shared<Menu>(
      olc::vi2d(),
      olc::vi2d(),
      "load_game_title",
      bg,
      fg,
      menu::Layout::Horizontal,
      false,
      false
    );
    m_loadGameScreen->addMenu(tm);

    m_savedGames.previous = generateScreenOption("Previous page", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m_savedGames.previous->setSimpleAction(
      [this](Game& /*g*/) {
        // Move to the previous page if possible.
        if (m_savedGames.index > 0) {
          m_savedGames.index -= std::min(m_savedGames.index, m_savedGames.gamesPerPage);
          updateSavedGamesDisplay();
        }
      }
    );
    m_savedGames.previous->enable(false);
    m_loadGameScreen->addMenu(m_savedGames.previous);

    // Create menus for each line of the saved game screen.
    for (unsigned id = 0u ; id < sk_savedGamesPerPage ; ++id) {
      GameMenuShPtr m = generateScreenOption("", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);

      m->setSimpleAction(
        [this, m](Game& g) {
          // Concatenate the save directory path to the name
          // of the game so that we can readily path it to
          // other processes.
          std::string fullPath = sk_savedGamesDir + std::string("/") + m->getText();

          setWorldFile(fullPath);
          g.reset(getWorldFile());

          // Move to the game screen.
          setScreen(game::Screen::Game);
        }
      );
      m->enable(false);

      m_loadGameScreen->addMenu(m);
      m_savedGames.games.push_back(m);
    }

    m_savedGames.next = generateScreenOption("Next page", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m_savedGames.next->setSimpleAction(
      [this](Game& /*g*/) {
        // Move to the next page if possible.
        if (m_savedGames.index + m_savedGames.gamesPerPage < m_savedGames.saves.size()) {
          m_savedGames.index += m_savedGames.gamesPerPage;
          updateSavedGamesDisplay();
        }

      }
    );
    m_savedGames.next->enable(false);
    m_loadGameScreen->addMenu(m_savedGames.next);

    GameMenuShPtr m = generateScreenOption("Back to main screen", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        // Assign the correct screen and terminate the game.
        setScreen(game::Screen::Home);
      }
    );
    m_loadGameScreen->addMenu(m);
  }

  void
  GameState::generatePauseScreen(const olc::vi2d& dims) {
    // Generate base menu.
    m_pauseScreen = generateDefaultScreen(dims, sk_screenBGColor);

    // Create each option.
    SimpleMenuShPtr m = generateScreenOption("Resume", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        setScreen(game::Screen::Game);
        g.togglePause(false);
      }
    );
    m_pauseScreen->addMenu(m);

    m = generateScreenOption("Save game", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        setScreen(game::Screen::Game);
        g.save(getWorldFile());
        g.togglePause(false);
      }
    );
    m_pauseScreen->addMenu(m);

    m = generateScreenOption("Back to main screen", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(game::Screen::Home);
      }
    );
    m_pauseScreen->addMenu(m);

    m = generateScreenOption("Exit", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        // Assign the correct screen and terminate the game.
        setScreen(game::Screen::Exit);
        g.terminate();
      }
    );
    m_pauseScreen->addMenu(m);
  }

  void
  GameState::generateGameOverScreen(const olc::vi2d& dims) {
    // Generate base menu.
    m_gameOverScreen = generateDefaultScreen(dims, sk_screenBGColor);

    // Create each option.
    SimpleMenuShPtr m = generateScreenOption("Back to main screen", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(game::Screen::Home);
      }
    );
    m_gameOverScreen->addMenu(m);

    m = generateScreenOption("Restart", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        // Assign the correct screen and reset the game with
        // the previously loaded one. If no game has been
        // loaded so far a new one will be generated.
        setScreen(game::Screen::Game);
        g.reset(getWorldFile());
      }
    );
    m_gameOverScreen->addMenu(m);

    m = generateScreenOption("Quit", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& g) {
        // Assign the correct screen and terminate the game.
        setScreen(game::Screen::Exit);
        g.terminate();
      }
    );
    m_gameOverScreen->addMenu(m);
  }

  void
  GameState::refreshSavedGames() {
    // Refresh the list of games that can be loaded by
    // re-reading the content of the save directory.
    // Note that we do not update the displayed values
    // within this method.
    std::string dir = sk_savedGamesDir;

    DirIt end;
    DirIt it(dir);

    m_savedGames.saves.clear();

    log("Scanning directory \"" + dir + "\" for saved games", utils::Level::Debug);
    for (; it != end ; ++it) {
      std::filesystem::directory_entry sg = *it;

      // Only keep the base path in the display: the
      // path to the save directory would just clutter
      // the output.
      std::string path = sg.path();
      std::string name = path.substr(dir.size() + 1);

      if (name.empty()) {
        log("Failed to interpret saved game \"" + path + "\"", utils::Level::Error);
        continue;
      }

      m_savedGames.saves.push_back(name);
    }

    // Sort the games in alphabetical order to ease
    // finding a particular game.
    std::sort(m_savedGames.saves.begin(), m_savedGames.saves.end());

    for (unsigned id = 0u ; id < m_savedGames.saves.size() ; ++id) {
      log("Found saved game \"" + m_savedGames.saves[id] + "\"", utils::Level::Debug);
    }
  }

  void
  GameState::updateSavedGamesDisplay() {
    // Update the text of the display menus with
    // the name of the games starting from the one
    // pointed at by the virtual cursor.
    unsigned max = std::min(
      static_cast<unsigned>(m_savedGames.saves.size()) - m_savedGames.index,
      m_savedGames.gamesPerPage
    );

    unsigned id = 0u;
    for (; id < max ; ++id) {
      m_savedGames.games[id]->setText(m_savedGames.saves[m_savedGames.index + id]);
      m_savedGames.games[id]->enable(true);
    }

    // Fill the rest of the cells with blank spaces.
    if (id < m_savedGames.gamesPerPage) {
      for (; id < m_savedGames.gamesPerPage ; ++id) {
        m_savedGames.games[id]->setText("");
        m_savedGames.games[id]->enable(false);
      }
    }

    // Update the next/previous page buttons.
    m_savedGames.previous->enable(m_savedGames.index > 0);
    m_savedGames.next->enable(
      m_savedGames.index + m_savedGames.gamesPerPage < m_savedGames.saves.size()
    );
  }

}
