
# include "Game.hh"
# include <cxxabi.h>
# include <maths_utils/AngleUtils.hh>
# include "TowerFactory.hh"
# include "GameMenu.hh"
# include "SimpleAction.hh"
# include "Mob.hh"
# include "Tower.hh"
# include "Spawner.hh"
# include "Wall.hh"

namespace {

  tdef::GameMenuShPtr
  generateTowerMenu(const tdef::towers::Type& type) {
    return std::make_shared<tdef::GameMenu>(
      tdef::towers::toString(type),
      [type](std::vector<tdef::ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<tdef::SimpleAction>(
            [type](tdef::Game& g) {
              g.setTowerType(type);
            }
          )
        );
      }
    );
  }

  tdef::GameMenuShPtr
  generateTowerUpgradeMenu(const tdef::towers::Upgrade& upg) {
    return std::make_shared<tdef::GameMenu>(
      tdef::towers::toString(upg),
      [upg](std::vector<tdef::ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<tdef::SimpleAction>(
            [upg](tdef::Game& g) {
              g.upgradeTower(upg);
            }
          )
        );
      }
    );
  }

}

namespace tdef {

  Game::Game():
    utils::CoreObject("game"),

    m_world(nullptr),
    m_loc(nullptr),
    m_state(
      State{
        true,                  // paused
        false,                 // disabled
        false,                 // terminated
        InfoPanelStatus::None, // infoState
        false,                 // wallBuilding
        nullptr,               // tType
        BASE_LIVES,            // lives
        BASE_GOLD              // gold
      }
    ),

    m_statusDisplay(StatusDisplay{
      nullptr, // main

      nullptr, // lives
      nullptr, // gold

      nullptr, // play
      nullptr, // pause

      -1       // goldEarnedSlot
    }),

    m_tDisplay(),
    m_mDisplay(),
    m_sDisplay(),
    m_wDisplay(),

    m_buildings(nullptr),
    m_tMenus()
  {
    setService("game");

    # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>("data/worlds/level_1.lvl");
# else
    m_world = std::make_shared<World>(100);
# endif

    m_loc = m_world->locator();

    // Register this item as a listener of the gold
    // earned signal.
    m_statusDisplay.goldEarnedSlot = m_world->onGoldEarned.connect_member<Game>(this, &Game::updateGold);
  }

  std::vector<MenuShPtr>
  Game::generateMenus(const olc::vi2d& dims) {
    // Generate each menu.
    std::vector<MenuShPtr> menus;

    menus.push_back(generateStatusMenu(dims));
    menus.push_back(generateTowersMenu(dims));
    menus.push_back(generateUpgradeMenu(dims));
    menus.push_back(generateMobMenu(dims));
    menus.push_back(generateSpawnerMenu(dims));
    menus.push_back(generateWallMenu(dims));

    return menus;
  }

  void
  Game::performAction(float x, float y) {
    // Only handle actions when the game is not disabled.
    if (m_state.disabled) {
      log("Ignoring action while menu is disabled");
      return;
    }

    // Assume the point is at the center of the
    // cell corresponding to the input position.
    utils::Point2f p(std::floor(x) + 0.5f, std::floor(y) + 0.5f);

    // Check whether this position is obstructed.
    WorldElementShPtr we = m_loc->itemAt(p, true);
    if (we != nullptr) {
      // Select this element to be displayed in
      // the side panel.
      MobShPtr m = std::dynamic_pointer_cast<Mob>(we);
      if (m != nullptr) {
        displayMob(m);
        updateUI();
        return;
      }

      TowerShPtr t = std::dynamic_pointer_cast<Tower>(we);
      if (t != nullptr) {
        displayTower(t);
        updateUI();
        return;
      }

      SpawnerShPtr s = std::dynamic_pointer_cast<Spawner>(we);
      if (s != nullptr) {
        displaySpawner(s);
        updateUI();
        return;
      }

      WallShPtr w = std::dynamic_pointer_cast<Wall>(we);
      if (w != nullptr) {
        displayWall(w);
        updateUI();
        return;
      }

      // Unknown element to display.
      int status;
      std::string et = abi::__cxa_demangle(typeid(*we).name(), 0, 0, &status);
      log(
        "Failed to display element at " + we->getPos().toString() +
        " with type \"" + et + "\"",
        utils::Level::Error
      );

      return;
    }

    // As no world element is located at the
    // position of the action, we can attempt
    // to build a tower or a wall, assuming
    // there is one defined.
    if (m_state.tType == nullptr && !m_state.wallBuilding) {
      return;
    }

    // Spawn a tower or a wall.
    if (m_state.tType != nullptr) {
      spawnTower(p);
      return;
    }

    if (m_state.wallBuilding) {
      spawnWall(p);
      return;
    }
  }

  void
  Game::upgradeTower(const towers::Upgrade& upgrade) {
    // Make sure there's a tower to upgrade.
    if (m_tDisplay.tower == nullptr) {
      log(
        "Attempting to upgrade " + towers::toString(upgrade) + " with no active tower",
        utils::Level::Error
      );

      return;
    }

    // Make sure there's enough money.
    towers::Type t = m_tDisplay.tower->getType();
    int level = m_tDisplay.tower->getUpgradeLevel(upgrade);
    float cost = towers::getUpgradeCost(t, upgrade, level);
    if (m_state.gold < cost) {
      log(
        "Upgrading " + towers::toString(upgrade) + " costs " + std::to_string(cost) +
        " but only " + std::to_string(m_state.gold) + "available, aborting",
        utils::Level::Error
      );

      return;
    }

    m_tDisplay.tower->upgrade(upgrade, level + 1);
    m_state.gold -= cost;
    log("Gold is now " + std::to_string(m_state.gold) + " due to cost " + std::to_string(cost));

    updateUI();
  }

  void
  Game::sellTower() {
    // Make sure there's a tower to upgrade.
    if (m_tDisplay.tower == nullptr) {
      log(
        "Attempting to sell tower while none is selected",
        utils::Level::Error
      );

      return;
    }

    float cost = m_tDisplay.tower->getTotalCost();
    m_state.gold += cost;

    log(
      "Selling tower " + towers::toString(m_tDisplay.tower->getType()) +
      " for " + std::to_string(cost) +
      ", " + std::to_string(m_state.gold) + " now available"
    );

    m_tDisplay.tower->markForDeletion(true);
    m_world->forceDelete();

    // Hide the upgrade menu.
    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;

    // Should be the case as we don't allow selling a tower
    // outside of the corresponding menu.
    if (m_state.infoState == InfoPanelStatus::Tower) {
      m_state.infoState = InfoPanelStatus::None;
    }

    updateUI();
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

  void
  Game::reset(const std::string& file) {
    // Reset game state.
    m_state.paused = true;
    m_state.disabled = false;
    // Note that the `terminated` is only set to `true` if the
    // user wants to exit the game so the next line is probably
    // redundant.
    m_state.terminated = false;
    m_state.infoState = InfoPanelStatus::None;
    m_state.wallBuilding = false;
    m_state.tType = nullptr;
    m_state.lives = BASE_LIVES;
    m_state.gold = BASE_GOLD;

    m_world->reset(file);

    // And reset menus.
    m_statusDisplay.main->setVisible(true);
    m_buildings->setVisible(true);

    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;

    m_mDisplay.main->setVisible(false);
    m_mDisplay.mob = nullptr;

    m_sDisplay.main->setVisible(false);
    m_sDisplay.spawner = nullptr;

    m_wDisplay.main->setVisible(false);
    m_wDisplay.wall = nullptr;
  }

  bool
  Game::step(float tDelta) {
    // When the game is paused it is not over yet.
    if (m_state.paused) {
      return true;
    }

    // Step the world.
    m_world->step(tDelta);

    // Update lives.
    m_state.lives = lives();

    // Disable the mob display in case it is either
    // dead or not in this world anymore.
    if (m_mDisplay.mob != nullptr && m_mDisplay.mob->isDeleted()) {
      m_mDisplay.main->setVisible(false);
      m_mDisplay.mob = nullptr;
    }

    // And redraw the UI.
    updateUI();

    // The game continues as long as there are some
    // lives left. In case the game is over, disable
    // the menu.
    bool alive = (m_state.lives > 0);
    if (!alive) {
      enable(false);
    }

    return alive;
  }

  void
  Game::togglePause(bool forceUI) {
    // Update paused status.
    m_state.paused = !m_state.paused;

    // Propagate info to the world.
    if (m_state.paused) {
      m_world->pause();
    }
    else {
      m_world->resume();
    }

    if (forceUI) {
      enable(!m_state.paused);
    }
  }

  MenuShPtr
  Game::generateStatusMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos;
    const olc::vi2d size(dims.x, 20);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_statusDisplay.main = std::make_shared<Menu>(pos, size, "sMenu", bg, fg, menu::Layout::Horizontal, false, false);

    // Adapt color for the sub menus background.
    olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    // Gold amount.
    fg = menu::newTextContent("Gold: " + std::to_string(m_state.gold));
    m_statusDisplay.gold = std::make_shared<Menu>(pos, size, "gold", bg, fg, menu::Layout::Horizontal, false, false);
    m_statusDisplay.main->addMenu(m_statusDisplay.gold);

    // Lives status.
    fg = menu::newTextContent("Lives: " + std::to_string(m_state.lives));
    m_statusDisplay.lives = std::make_shared<Menu>(pos, size, "lives", bg, fg, menu::Layout::Horizontal, false, false);
    m_statusDisplay.main->addMenu(m_statusDisplay.lives);

    // Wave count.
    fg = menu::newTextContent("Wave: 1");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "wave", bg, fg, menu::Layout::Horizontal, false, false);
    m_statusDisplay.main->addMenu(sm);

    // Pause button.
    olc::vi2d fSize(25, size.y);
    bg = menu::newColoredBackground(olc::WHITE);
    fg = menu::newImageContent("data/img/pause.png", fSize);
    fg.expand = false;
    m_statusDisplay.pause = std::make_shared<SimpleMenu>(
      pos, fSize, bg, fg,
      [this](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [this](Game& g) {
              g.pause();
            }
          )
        );
      }
    );
    m_statusDisplay.main->addMenu(m_statusDisplay.pause);

    // Play button.
    fg = menu::newImageContent("data/img/play.png", fSize);
    fg.expand = false;
    m_statusDisplay.play = std::make_shared<SimpleMenu>(
      pos, fSize, bg, fg,
      [this](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [this](Game& g) {
              g.resume();
            }
          )
        );
      }
    );
    m_statusDisplay.main->addMenu(m_statusDisplay.play);

    return m_statusDisplay.main;
  }

  MenuShPtr
  Game::generateTowersMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::Transparent);
    const olc::vi2d pos(0, 20);
    const olc::vi2d size(50, dims.y - 20);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_buildings = std::make_shared<Menu>(pos, size, "tMenu", bg, fg, menu::Layout::Vertical, false, false);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    GameMenuShPtr tm = generateTowerMenu(towers::Type::Basic);
    m_tMenus[towers::Type::Basic] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Sniper);
    m_tMenus[towers::Type::Sniper] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Cannon);
    m_tMenus[towers::Type::Cannon] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Freezing);
    m_tMenus[towers::Type::Freezing] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Venom);
    m_tMenus[towers::Type::Venom] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Splash);
    m_tMenus[towers::Type::Splash] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Blast);
    m_tMenus[towers::Type::Blast] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Multishot);
    m_tMenus[towers::Type::Multishot] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Minigun);
    m_tMenus[towers::Type::Minigun] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Antiair);
    m_tMenus[towers::Type::Antiair] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Tesla);
    m_tMenus[towers::Type::Tesla] = tm;
    m_buildings->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Missile);
    m_tMenus[towers::Type::Missile] = tm;
    m_buildings->addMenu(tm);

    m_buildings->addMenu(
      std::make_shared<GameMenu>(
        "Wall",
        [](std::vector<ActionShPtr>& actions) {
          actions.push_back(
            std::make_shared<SimpleAction>(
              [](Game& g) {
                g.allowWallBuilding();
              }
            )
          );
        },
        nullptr,
        false
      )
    );

    return m_buildings;
  }

  MenuShPtr
  Game::generateUpgradeMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 250, 20);
    const olc::vi2d size(250, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_tDisplay.main = std::make_shared<Menu>(pos, size, "uMenu", bg, fg, menu::Layout::Vertical, false, false);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Type: Regular");
    m_tDisplay.type = std::make_shared<Menu>(pos, size, "prop1", bg, fg, menu::Layout::Horizontal, false, false);
    m_tDisplay.main->addMenu(m_tDisplay.type);

    for (unsigned id = 0u ; id < UPGRADE_COUNT ; ++id) {
      // Generate a `Range` menu for now, it will be updated
      // when a tower is selected.
      m_tDisplay.props.push_back(generateTowerUpgradeMenu(towers::Upgrade::Range));
      m_tDisplay.main->addMenu(m_tDisplay.props.back());
    }

    fg = menu::newTextContent("Sell");
    m_tDisplay.sell = std::make_shared<SimpleMenu>(
      pos, size,
      bg, fg,
      [](std::vector<ActionShPtr>& actions) {
        actions.push_back(
          std::make_shared<SimpleAction>(
            [](Game& g) {
              g.sellTower();
            }
          )
        );
      }
    );
    m_tDisplay.main->addMenu(m_tDisplay.sell);

    fg = menu::newTextContent("Target mode");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "prop2", bg, fg);
    m_tDisplay.main->addMenu(sm);

    // This menu is hidden until the user clicks on
    // a tower.
    m_tDisplay.main->setVisible(false);

    // No mob being tracked for now.
    m_tDisplay.tower = nullptr;

    return m_tDisplay.main;
  }

  MenuShPtr
  Game::generateMobMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 150, 20);
    const olc::vi2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_mDisplay.main = std::make_shared<Menu>(pos, size, "mMenu", bg, fg, menu::Layout::Vertical, false, false);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Type: Regular");
    m_mDisplay.type = std::make_shared<Menu>(pos, size, "prop1", bg, fg, menu::Layout::Horizontal, false, false);
    m_mDisplay.main->addMenu(m_mDisplay.type);

    fg = menu::newTextContent("Health: 10");
    m_mDisplay.health = std::make_shared<Menu>(pos, size, "prop2", bg, fg, menu::Layout::Horizontal, false, false);
    m_mDisplay.main->addMenu(m_mDisplay.health);

    fg = menu::newTextContent("Speed: 10");
    m_mDisplay.speed = std::make_shared<Menu>(pos, size, "prop3", bg, fg, menu::Layout::Horizontal, false, false);
    m_mDisplay.main->addMenu(m_mDisplay.speed);

    fg = menu::newTextContent("Bounty: 10");
    m_mDisplay.bounty = std::make_shared<Menu>(pos, size, "prop4", bg, fg, menu::Layout::Horizontal, false, false);
    m_mDisplay.main->addMenu(m_mDisplay.bounty);

    // This menu is hidden until the user clicks on
    // a mob.
    m_mDisplay.main->setVisible(false);

    // No mob being tracked for now.
    m_mDisplay.mob = nullptr;

    return m_mDisplay.main;
  }

  MenuShPtr
  Game::generateSpawnerMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 150, 20);
    const olc::vi2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_sDisplay.main = std::make_shared<Menu>(pos, size, "sMenu", bg, fg, menu::Layout::Vertical, false, false);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Health: 10");
    m_sDisplay.health = std::make_shared<Menu>(pos, size, "prop1", bg, fg, menu::Layout::Horizontal, false, false);
    m_sDisplay.main->addMenu(m_sDisplay.health);

    // This menu is hidden until the user clicks on
    // a spawner.
    m_sDisplay.main->setVisible(false);

    // No spawner being tracked for now.
    m_sDisplay.spawner = nullptr;

    return m_sDisplay.main;
  }

  MenuShPtr
  Game::generateWallMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 150, 20);
    const olc::vi2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_wDisplay.main = std::make_shared<Menu>(pos, size, "wMenu", bg, fg, menu::Layout::Vertical, false, false);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Health: 10");
    m_wDisplay.health = std::make_shared<Menu>(pos, size, "prop1", bg, fg, menu::Layout::Horizontal, false, false);
    m_wDisplay.main->addMenu(m_wDisplay.health);

    // This menu is hidden until the user clicks on
    // a wall.
    m_wDisplay.main->setVisible(false);

    // No wall being tracked for now.
    m_wDisplay.wall = nullptr;

    return m_wDisplay.main;
  }

  void
  Game::spawnTower(const utils::Point2f& p) {
    // Generate the tower. Also make sure that the
    // player owns enough gold to do so.
    float c = towers::getCost(*m_state.tType);
    if (m_state.gold < c) {
      log(
        "Can't afford tower " + towers::toString(*m_state.tType) + " costing " +
        std::to_string(c) + " with only " +
        std::to_string(m_state.gold) + " gold available",
        utils::Level::Error
      );

      return;
    }

    m_state.gold -= c;

    Tower::TProps pp = towers::generateProps(*m_state.tType, p);

    log(
      "Generated tower " + towers::toString(*m_state.tType) + " at " + p.toString(),
      utils::Level::Info
    );

    TowerShPtr t = std::make_shared<Tower>(pp);
    m_world->spawn(t);
  }

  void
  Game::spawnWall(const utils::Point2f& p) {
    log("Generated wall at " + p.toString());

    Wall::WProps pp = Wall::newProps(p);

    WallShPtr w = std::make_shared<Wall>(pp);
    m_world->spawn(w);
  }

  void
  Game::displayMob(MobShPtr m) noexcept {
    // Deactivate any other menu and activate the
    // one corresponding to mob props.
    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;
    m_mDisplay.main->setVisible(true);
    m_sDisplay.main->setVisible(false);
    m_sDisplay.spawner = nullptr;
    m_wDisplay.main->setVisible(false);
    m_wDisplay.wall = nullptr;

    m_state.infoState = InfoPanelStatus::Mob;

    // Register the mob as the one being followed.
    m_mDisplay.mob = m;
  }

  void
  Game::displayTower(TowerShPtr t) noexcept {
    // Deactivate any other menu and activate the
    // one corresponding to mob props.
    m_tDisplay.main->setVisible(true);
    m_mDisplay.main->setVisible(false);
    m_mDisplay.mob = nullptr;
    m_sDisplay.main->setVisible(false);
    m_sDisplay.spawner = nullptr;
    m_wDisplay.main->setVisible(false);
    m_wDisplay.wall = nullptr;

    m_state.infoState = InfoPanelStatus::Tower;

    // Register the tower as the one being followed.
    m_tDisplay.tower = t;

    // Update the menus to display the props of the
    // tower.
    towers::Upgrades ugs = t->getUpgrades();

    unsigned id = 0u;
    for (towers::Upgrades::const_iterator it = ugs.cbegin() ;
         it != ugs.cend() && id < m_tDisplay.props.size();
         ++it)
    {
      towers::Upgrade u = it->first;

      m_tDisplay.props[id]->setAction(
        [u](std::vector<tdef::ActionShPtr>& actions) {
          actions.push_back(
            std::make_shared<tdef::SimpleAction>(
              [u](tdef::Game& g) {
                g.upgradeTower(u);
              }
            )
          );
        }
      );
      ++id;
    }
  }

  void
  Game::displaySpawner(SpawnerShPtr s) noexcept {
    // Deactivate any other menu and activate the
    // one corresponding to mob props.
    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;
    m_mDisplay.main->setVisible(false);
    m_mDisplay.mob = nullptr;
    m_sDisplay.main->setVisible(true);
    m_wDisplay.main->setVisible(false);
    m_wDisplay.wall = nullptr;

    m_state.infoState = InfoPanelStatus::Spawner;

    // Register the spawner as the one being followed.
    m_sDisplay.spawner = s;
  }

  void
  Game::displayWall(WallShPtr w) noexcept {
    // Deactivate any other menu and activate the
    // one corresponding to mob props.
    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;
    m_mDisplay.main->setVisible(false);
    m_mDisplay.mob = nullptr;
    m_sDisplay.main->setVisible(false);
    m_sDisplay.spawner = nullptr;
    m_wDisplay.main->setVisible(true);

    m_state.infoState = InfoPanelStatus::Wall;

    // Register the wall as the one being followed.
    m_wDisplay.wall = w;
  }

  void
  Game::enable(bool enable) {
    if (m_statusDisplay.main != nullptr) {
      m_statusDisplay.main->setVisible(enable);
    }

    m_state.disabled = !enable;

    // Only reenable menus (i.e. `enable = true`) in
    // case the information status indicates that it
    // was indeed displayed.
    bool st = (enable && !m_state.disabled);
    if (m_tDisplay.main != nullptr) {
      m_tDisplay.main->setVisible(st && m_state.infoState == InfoPanelStatus::Tower);
    }
    if (m_mDisplay.main != nullptr) {
      m_mDisplay.main->setVisible(st && m_state.infoState == InfoPanelStatus::Mob);
    }
    if (m_sDisplay.main != nullptr) {
      m_sDisplay.main->setVisible(st && m_state.infoState == InfoPanelStatus::Spawner);
    }
    if (m_wDisplay.main != nullptr) {
      m_wDisplay.main->setVisible(st && m_state.infoState == InfoPanelStatus::Wall);
    }

    if (m_buildings != nullptr) {
      m_buildings->setVisible(st);
    }
  }

  void
  Game::updateUI() {
    // Update status menu.
    int v = static_cast<int>(m_state.lives);
    m_statusDisplay.lives->setText("Lives: " + std::to_string(v));

    v = static_cast<int>(m_state.gold);
    m_statusDisplay.gold->setText("Gold: " + std::to_string(v));

    // Update display values for visible menus.
    if (m_tDisplay.tower != nullptr) {
      std::string t = towers::toString(m_tDisplay.tower->getType());
      m_tDisplay.type->setText("Type: " + t);

      towers::Upgrades ug = m_tDisplay.tower->getUpgrades();

      unsigned id = 0u;
      for (towers::Upgrades::const_iterator it = ug.cbegin() ;
           it != ug.cend() && id < m_tDisplay.props.size() ;
           ++it)
      {
        float v;
        bool intVal = false;
        std::string unit;

        switch (it->first) {
          case towers::Upgrade::Range:
            v = m_tDisplay.tower->getRange();
            break;
          case towers::Upgrade::Damage:
            v = m_tDisplay.tower->getAttack();
            intVal = true;
            break;
          case towers::Upgrade::RotationSpeed:
            v = utils::radToDeg(m_tDisplay.tower->getRotationSpeed());
            intVal = true;
            unit = "deg";
            break;
          case towers::Upgrade::AttackSpeed:
            v = m_tDisplay.tower->getAttackSpeed();
            break;
          case towers::Upgrade::ProjectileSpeed:
            v = m_tDisplay.tower->getProjectileSpeed();
            intVal = true;
            break;
          case towers::Upgrade::FreezingPower:
            v = m_tDisplay.tower->getFreezingPower();
            intVal = true;
            unit = "%";
            break;
          case towers::Upgrade::FreezingSpeed:
            v = m_tDisplay.tower->getFreezingSpeed();
            intVal = true;
            unit = "%";
            break;
          case towers::Upgrade::FreezingDuration:
            // Convert milliseconds to seconds.
            v = m_tDisplay.tower->getFreezingDuration() / 1000.0f;
            intVal = true;
            unit = "s";
            break;
          case towers::Upgrade::PoisonDuration:
            // Convert milliseconds to seconds.
            v = m_tDisplay.tower->getPoisonDuration() / 1000.0f;
            intVal = true;
            unit = "s";
            break;
          case towers::Upgrade::StunChance:
            v = m_tDisplay.tower->getStunChance();
            intVal = true;
            unit = "%";
            break;
          case towers::Upgrade::StunDuration:
            // Convert milliseconds to seconds.
            v = m_tDisplay.tower->getStunDuration() / 1000.0f;
            intVal = true;
            unit = "s";
            break;
          default:
            // Unhandled for now.
            log("Unhandled props " + towers::toString(it->first), utils::Level::Error);
            v = -1.0f;
            break;
        }

        std::string msg = towers::toString(it->first);
        msg += "(";
        msg += std::to_string(it->second);
        msg += "):";
        if (intVal) {
          msg += std::to_string(static_cast<int>(std::round(v)));
        }
        else {
          // Only display 2 decimals.
          std::stringstream out;
          out << std::fixed << std::setprecision(2) << v;
          msg += out.str();
        }
        if (!unit.empty()) {
          msg += " ";
          msg += unit;
        }

        float cost = towers::getUpgradeCost(m_tDisplay.tower->getType(), it->first, it->second);
        msg += " (cost: ";
        msg += std::to_string(static_cast<int>(std::round(cost)));
        msg += ")";

        m_tDisplay.props[id]->setText(msg);
        m_tDisplay.props[id]->enable(m_state.gold >= cost);

        ++id;
      }

      if (id < ug.size()) {
        log(
          "Only interpreted " + std::to_string(id) + " among " + std::to_string(ug.size()) + " available",
          utils::Level::Error
        );
      }

      std::string msg = "Sell (";
      float tc = m_tDisplay.tower->getTotalCost();
      msg += std::to_string(static_cast<int>(std::round(tc)));
      msg += " gold)";
      m_tDisplay.sell->setText(msg);
    }

    if (m_mDisplay.mob != nullptr) {
      std::string t = mobs::toString(m_mDisplay.mob->getType());
      m_mDisplay.type->setText("Type: " + t);

      float v = m_mDisplay.mob->getHealth();
      m_mDisplay.health->setText("Health: " + std::to_string(v));

      v = m_mDisplay.mob->getSpeed();
      m_mDisplay.speed->setText("Speed: " + std::to_string(v));

      v = m_mDisplay.mob->getBounty();
      m_mDisplay.bounty->setText("Bounty: " + std::to_string(v));
    }

    if (m_sDisplay.spawner != nullptr) {
      float v = m_sDisplay.spawner->getHealth();
      m_sDisplay.health->setText("Health: " + std::to_string(v));
    }

    if (m_wDisplay.wall != nullptr) {
      float v = m_wDisplay.wall->getHealth();
      m_wDisplay.health->setText("Health: " + std::to_string(v));
    }

    // Update status for tower's construction.
    // Note that we assume all the menus are
    // already registered in the map. If this
    // is not the case UB will arise.
    towers::Type t = towers::Type::Basic;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Sniper;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Cannon;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Freezing;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Venom;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Splash;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Blast;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Multishot;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Minigun;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Antiair;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Tesla;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
    t = towers::Type::Missile;
    m_tMenus[t]->enable(m_state.gold >= towers::getCost(t));
  }

}
