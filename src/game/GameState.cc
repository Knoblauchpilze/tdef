
# include "GameState.hh"
# include "SimpleMenu.hh"
# include "SimpleAction.hh"
# include "Game.hh"

namespace tdef {

  const olc::Pixel GameState::sk_screenBGColor(20, 20, 20, alpha::Opaque);
  const olc::Pixel GameState::sk_menuBGColor(90, 90, 90, alpha::Opaque);
  const olc::Pixel GameState::sk_menuTextColor(180, 180, 180, alpha::Opaque);
  const olc::Pixel GameState::sk_menuTextColorHighlight(30, 30, 30, alpha::Opaque);

  GameState::GameState(const olc::vi2d& dims,
                       const game::Screen& screen):
    utils::CoreObject("state"),

    m_screen(screen),
    m_worldFile(),

    m_gameOverMenu(nullptr)
  {
    setService("game");

    // Generate each screen.
    generateGameOverScreen(dims);
  }

  void
  GameState::setScreen(const game::Screen& screen) {
    if (m_screen == screen) {
      return;
    }

    // Assign the state.
    m_screen = screen;

    // Update screens' visibility.
    m_gameOverMenu->setVisible(m_screen == game::Screen::GameOver);
  }

  void
  GameState::render(olc::PixelGameEngine* pge) const {
    // Render each screen.
    m_gameOverMenu->render(pge);
  }

  menu::InputHandle
  GameState::processUserInput(const controls::State& c,
                              std::vector<ActionShPtr>& actions)
  {
    menu::InputHandle res{false, false};

    // Propagate the user input to each screen.
    menu::InputHandle cur = m_gameOverMenu->processUserInput(c, actions);
    res.relevant = (res.relevant || cur.relevant);
    res.selected = (res.selected || cur.selected);

    return res;
  }

  void
  GameState::generateGameOverScreen(const olc::vi2d& dims) {
    const olc::vf2d size(std::min(dims.x, 350), std::min(dims.y, 200));
    const olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

    menu::BackgroundDesc bg = menu::newColoredBackground(sk_screenBGColor);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_gameOverMenu = std::make_shared<Menu>(
      pos,
      size,
      "goMenu",
      bg,
      fg,
      menu::Layout::Vertical
    );

    // Create each option.
    bg = menu::newColoredBackground(sk_menuBGColor);
    fg = menu::newTextContent("Back to main screen", sk_menuTextColor, menu::Alignment::Center);
    fg.hColor = sk_menuTextColorHighlight;
    m_gameOverMenu->addMenu(
      std::make_shared<SimpleMenu>(
        pos,
        size,
        bg,
        fg,
        [this](std::vector<ActionShPtr>& actions) {
          actions.push_back(
            std::make_shared<SimpleAction>(
              [this](Game& /*g*/) {
                setScreen(game::Screen::Home);
              }
            )
          );
        }
      )
    );

    bg = menu::newColoredBackground(sk_menuBGColor);
    fg = menu::newTextContent("Restart", sk_menuTextColor, menu::Alignment::Center);
    fg.hColor = sk_menuTextColorHighlight;
    m_gameOverMenu->addMenu(
      std::make_shared<SimpleMenu>(
        pos,
        size,
        bg,
        fg,
        [this](std::vector<ActionShPtr>& actions) {
          actions.push_back(
            std::make_shared<SimpleAction>(
              [this](Game& g) {
                // Assign the correct screen and reset the game.
                setScreen(game::Screen::Game);
                g.reset();
              }
            )
          );
        }
      )
    );

    bg = menu::newColoredBackground(sk_menuBGColor);
    fg = menu::newTextContent("Quit", sk_menuTextColor, menu::Alignment::Center);
    fg.hColor = sk_menuTextColorHighlight;
    m_gameOverMenu->addMenu(
      std::make_shared<SimpleMenu>(
        pos,
        size,
        bg,
        fg,
        [this](std::vector<ActionShPtr>& actions) {
          actions.push_back(
            std::make_shared<SimpleAction>(
              [this](Game& g) {
                // Assign the correct screen and terminate the game.
                setScreen(game::Screen::Exit);
                g.terminate();
              }
            )
          );
        }
      )
    );

    // Disable the menu when we start the app.
    m_gameOverMenu->setVisible(false);
  }

}
