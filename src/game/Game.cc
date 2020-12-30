
# include "Game.hh"
# include "TowerFactory.hh"
# include "SimpleMenu.hh"
# include "SimpleAction.hh"

namespace tdef {

  Game::Game():
    utils::CoreObject("game"),

    m_world(nullptr),
    m_loc(nullptr),

    m_tType(nullptr),
    m_wallBuilding(false)
  {
    setService("game");

    // # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100, 10, 5);
# endif

    m_loc = m_world->locator();
  }

  std::vector<MenuShPtr>
  Game::generateMenus(const olc::vi2d& dims) const {
    // Generate each menu.
    std::vector<MenuShPtr> menus;
    
    menus.push_back(generateStatusMenu(dims));
    menus.push_back(generateTowersMenu(dims));
    menus.push_back(generateUpgradeMenu(dims));

    return menus;
  }

  void
  Game::performAction(float x, float y) {
    // Make sure that a tower type is defined.
    if (m_tType == nullptr && !m_wallBuilding) {
      return;
    }

    // Check whether this position is obstructed.
    if (m_loc->obstructed(x, y)) {
      log("Can't place element at " + std::to_string(x) + "x" + std::to_string(y));
      return;
    }

    // Spawn a tower or a wall.
    if (m_tType != nullptr) {
      spawnTower(x, y);
      return;
    }

    if (m_wallBuilding) {
      spawnWall(x, y);
      return;
    }
  }

  float
  Game::lives() const noexcept {
    float l = 0.0f;

    std::vector<BlockShPtr> bs = m_loc->getVisibleBlocks(utils::Point2f(), -1.0f, world::BlockType::Portal);

    for (unsigned id = 0u ; id < bs.size() ; ++id) {
      PortalShPtr p = std::dynamic_pointer_cast<Portal>(bs[id]);
      if (p != nullptr) {
        l += p->getLives();
      }
    }

    return l;
  }

  MenuShPtr
  Game::generateStatusMenu(const olc::vi2d& dims) const {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos;
    const olc::vf2d size(dims.x, 20);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    MenuShPtr m = std::make_shared<Menu>(pos, size, "sMenu", bg, fg);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    // Gold amount.
    fg = menu::newTextContent("Gold: " + std::to_string(BASE_GOLD));
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "gold", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(sm);

    // Lives status.
    fg = menu::newTextContent("Lives: " + std::to_string(BASE_LIVES));
    sm = std::make_shared<Menu>(pos, size, "lives", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(sm);

    // Wave count.
    fg = menu::newTextContent("Wave: 1");
    sm = std::make_shared<Menu>(pos, size, "wave", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(sm);

    return m;
  }

  MenuShPtr
  Game::generateTowersMenu(const olc::vi2d& dims) const {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(0, dims.y - 50);
    const olc::vf2d size(dims.x, 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    MenuShPtr m = std::make_shared<Menu>(pos, size, "tMenu", bg, fg);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Basic");
    MenuShPtr sm = std::make_shared<SimpleMenu>(
      pos,
      size,
      bg,
      fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.setTowerType(towers::Type::Basic);
            }
          )
        );
      }
    );
    m->addMenu(sm);

    fg = menu::newTextContent("Sniper");
    sm = std::make_shared<SimpleMenu>(
      pos,
      size,
      bg,
      fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.setTowerType(towers::Type::Sniper);
            }
          )
        );
      }
    );
    m->addMenu(sm);

    fg = menu::newTextContent("Freezing");
    sm = std::make_shared<SimpleMenu>(
      pos,
      size,
      bg,
      fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.setTowerType(towers::Type::Freezing);
            }
          )
        );
      }
    );
    m->addMenu(sm);

    fg = menu::newTextContent("Cannon");
    sm = std::make_shared<SimpleMenu>(
      pos,
      size,
      bg,
      fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.setTowerType(towers::Type::Cannon);
            }
          )
        );
      }
    );
    m->addMenu(sm);

    fg = menu::newTextContent("Wall");
    sm = std::make_shared<SimpleMenu>(
      pos,
      size,
      bg,
      fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.allowWallBuilding();
            }
          )
        );
      }
    );
    m->addMenu(sm);

    return m;
  }

  MenuShPtr
  Game::generateUpgradeMenu(const olc::vi2d& dims) const {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 120, 20);
    const olc::vf2d size(120, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    MenuShPtr m = std::make_shared<Menu>(pos, size, "uMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Range: 10");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
    m->addMenu(sm);

    fg = menu::newTextContent("Strength: 10");
    sm = std::make_shared<Menu>(pos, size, "prop2", bg, fg);
    m->addMenu(sm);

    fg = menu::newTextContent("Attack speed: 10");
    sm = std::make_shared<Menu>(pos, size, "prop3", bg, fg);
    m->addMenu(sm);

    fg = menu::newTextContent("Reload time: 10");
    sm = std::make_shared<Menu>(pos, size, "prop4", bg, fg);
    m->addMenu(sm);

    fg = menu::newTextContent("Sell");
    sm = std::make_shared<Menu>(pos, size, "prop5", bg, fg);
    m->addMenu(sm);

    fg = menu::newTextContent("Target mode");
    sm = std::make_shared<Menu>(pos, size, "prop6", bg, fg);
    m->addMenu(sm);

    return m;
  }

  void
  Game::spawnTower(float x, float y) {
    // Generate the tower.
    Tower::TProps pp;
    towers::Data td;
    utils::Point2f p(std::floor(x) + 0.5f, std::floor(y) + 0.5f);

    bool valid = true;
    switch (*m_tType) {
      case towers::Type::Basic:
        pp = TowerFactory::generateBasicTowerProps(p);
        td = TowerFactory::generateBasicTowerData();
        break;
      case towers::Type::Sniper:
        pp = TowerFactory::generateSniperTowerProps(p);
        td = TowerFactory::generateSniperTowerData();
        break;
      case towers::Type::Freezing:
        pp = TowerFactory::generateSlowTowerProps(p);
        td = TowerFactory::generateSlowTowerData();
        break;
      case towers::Type::Cannon:
        pp = TowerFactory::generateCannonTowerProps(p);
        td = TowerFactory::generateCannonTowerData();
        break;
      default:
        valid = false;
        break;
    }

    if (!valid) {
      log(
        "Unknown tower type " + std::to_string(static_cast<int>(*m_tType)) +
        " to generate",
        utils::Level::Warning
      );

      return;
    }

    log(
      "Generated tower " + std::to_string(static_cast<int>(*m_tType)) +
      " at " + std::to_string(x) + "x" + std::to_string(y)
    );

    TowerShPtr t = std::make_shared<Tower>(pp, td);
    m_world->spawn(t);
  }

  void
  Game::spawnWall(float x, float y) {
    log("Generated wall at " + std::to_string(x) + "x" + std::to_string(y));

    utils::Point2f p(std::floor(x) + 0.5f, std::floor(y) + 0.5f);
    Wall::WProps pp = Wall::newProps(p);

    WallShPtr w = std::make_shared<Wall>(pp);
    m_world->spawn(w);
  }

}
