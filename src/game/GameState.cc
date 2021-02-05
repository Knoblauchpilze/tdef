
# include "GameState.hh"
# include "SimpleMenu.hh"
# include "SimpleAction.hh"
# include "Game.hh"

namespace {

  tdef::MenuShPtr
  generateDefaultScreen(const olc::vi2d& dims,
                        const olc::Pixel& color)
  {
    const olc::vf2d size(std::min(dims.x, 350), std::min(dims.y, 200));
    const olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

    tdef::menu::BackgroundDesc bg = tdef::menu::newColoredBackground(color);
    tdef::menu::MenuContentDesc fg = tdef::menu::newTextContent("");

    return std::make_shared<tdef::Menu>(
      pos,
      size,
      "goMenu",
      bg,
      fg,
      tdef::menu::Layout::Vertical
    );
  }

  tdef::SimpleMenuShPtr
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

    return std::make_shared<tdef::SimpleMenu>(
      olc::vi2d(),
      olc::vf2d(),
      bg,
      fg,
      [](std::vector<tdef::ActionShPtr>& /*actions*/) {
        // Dummy action.
      }
    );
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
      [this](Game& /*g*/) {
        setScreen(game::Screen::Game);
      }
    );
    m_homeScreen->addMenu(m);

    m = generateScreenOption("Load game", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
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
    // Generate base menu.
    m_loadGameScreen = generateDefaultScreen(dims, sk_screenBGColor);
  }

  void
  GameState::generatePauseScreen(const olc::vi2d& dims) {
    // Generate base menu.
    m_pauseScreen = generateDefaultScreen(dims, sk_screenBGColor);

    // Create each option.
    SimpleMenuShPtr m = generateScreenOption("Resume", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(game::Screen::Game);
      }
    );
    m_pauseScreen->addMenu(m);

    m = generateScreenOption("Save game", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        setScreen(game::Screen::Game);
      }
    );
    m_pauseScreen->addMenu(m);

    m = generateScreenOption("Back to main screen", sk_menuBGColor, sk_menuTextColor, sk_menuTextColorHighlight);
    m->setSimpleAction(
      [this](Game& /*g*/) {
        // Assign the correct screen and terminate the game.
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
        // Assign the correct screen and reset the game.
        setScreen(game::Screen::Game);
        g.reset();
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

}
