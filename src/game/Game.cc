
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

    m_tType(nullptr),
    m_wallBuilding(false),

    m_lives(BASE_LIVES),
    m_mLives(nullptr),

    m_gold(BASE_GOLD),
    m_mGold(nullptr),
    m_goldEarnedSlot(-1),

    m_tDisplay(),
    m_mDisplay(),
    m_sDisplay(),
    m_wDisplay(),

    m_tMenus()
  {
    setService("game");

    // # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100);
# endif

    m_loc = m_world->locator();

    // Register this item as a listener of the gold
    // earned signal.
    m_goldEarnedSlot = m_world->onGoldEarned.connect_member<Game>(this, &Game::updateGold);
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
    if (m_tType == nullptr && !m_wallBuilding) {
      return;
    }

    // Spawn a tower or a wall.
    if (m_tType != nullptr) {
      spawnTower(p);
      return;
    }

    if (m_wallBuilding) {
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
    if (m_gold < cost) {
      log(
        "Upgrading " + towers::toString(upgrade) + " costs " + std::to_string(cost) +
        " but only " + std::to_string(m_gold) + "available, aborting",
        utils::Level::Error
      );

      return;
    }

    m_tDisplay.tower->upgrade(upgrade, level + 1);
    m_gold -= cost;
    log("Gold is now " + std::to_string(m_gold) + " due to cost " + std::to_string(cost));

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
    m_gold += cost;

    log(
      "Selling tower " + towers::toString(m_tDisplay.tower->getType()) +
      " for " + std::to_string(cost) +
      ", " + std::to_string(m_gold) + " now available"
    );

    m_tDisplay.tower->markForDeletion(true);
    m_world->forceDelete();

    // Hide the upgrade menu.
    m_tDisplay.main->setVisible(false);
    m_tDisplay.tower = nullptr;

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
  Game::step(float tDelta) {
    // Step the world.
    m_world->step(tDelta);

    // Update lives.
    m_lives = lives();

    // Disable the mob display in case it is either
    // dead or not in this world anymore.
    if (m_mDisplay.mob != nullptr && m_mDisplay.mob->isDeleted()) {
      m_mDisplay.main->setVisible(false);
      m_mDisplay.mob = nullptr;
    }

    // And redraw the UI.
    updateUI();
  }

  MenuShPtr
  Game::generateStatusMenu(const olc::vi2d& dims) {
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
    fg = menu::newTextContent("Gold: " + std::to_string(m_gold));
    m_mGold = std::make_shared<Menu>(pos, size, "gold", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(m_mGold);

    // Lives status.
    fg = menu::newTextContent("Lives: " + std::to_string(m_lives));
    m_mLives = std::make_shared<Menu>(pos, size, "lives", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(m_mLives);

    // Wave count.
    fg = menu::newTextContent("Wave: 1");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "wave", bg, fg, menu::Layout::Horizontal, false);
    m->addMenu(sm);

    return m;
  }

  MenuShPtr
  Game::generateTowersMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::Transparent);
    const olc::vi2d pos(0, 20);
    const olc::vf2d size(50, dims.y - 20);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    MenuShPtr m = std::make_shared<Menu>(pos, size, "tMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    GameMenuShPtr tm = generateTowerMenu(towers::Type::Basic);
    m_tMenus[towers::Type::Basic] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Sniper);
    m_tMenus[towers::Type::Sniper] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Cannon);
    m_tMenus[towers::Type::Cannon] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Freezing);
    m_tMenus[towers::Type::Freezing] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Venom);
    m_tMenus[towers::Type::Venom] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Splash);
    m_tMenus[towers::Type::Splash] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Blast);
    m_tMenus[towers::Type::Blast] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Multishot);
    m_tMenus[towers::Type::Multishot] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Minigun);
    m_tMenus[towers::Type::Minigun] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Antiair);
    m_tMenus[towers::Type::Antiair] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Tesla);
    m_tMenus[towers::Type::Tesla] = tm;
    m->addMenu(tm);

    tm = generateTowerMenu(towers::Type::Missile);
    m_tMenus[towers::Type::Missile] = tm;
    m->addMenu(tm);

    m->addMenu(
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

    return m;
  }

  MenuShPtr
  Game::generateUpgradeMenu(const olc::vi2d& dims) {
    // Constants.
    const olc::Pixel bgc(20, 20, 20, alpha::SemiOpaque);
    const olc::vi2d pos(dims.x - 250, 20);
    const olc::vf2d size(250, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_tDisplay.main = std::make_shared<Menu>(pos, size, "uMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Type: Regular");
    m_tDisplay.type = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
    m_tDisplay.main->addMenu(m_tDisplay.type);

    for (unsigned id = 0u ; id < UPGRADE_COUNT ; ++id) {
      // Generate a `Range` menu for now, it will be updated
      // when a tower is selected.
      m_tDisplay.props.push_back(generateTowerUpgradeMenu(towers::Upgrade::Range));
      m_tDisplay.main->addMenu(m_tDisplay.props.back());
    }

    fg = menu::newTextContent("Sell");
    m_tDisplay.main->addMenu(
      std::make_shared<SimpleMenu>(
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
      )
    );

    fg = menu::newTextContent("Target mode");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "prop6", bg, fg);
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
    const olc::vf2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_mDisplay.main = std::make_shared<Menu>(pos, size, "mMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Type: Regular");
    m_mDisplay.type = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
    m_mDisplay.main->addMenu(m_mDisplay.type);

    fg = menu::newTextContent("Health: 10");
    m_mDisplay.health = std::make_shared<Menu>(pos, size, "prop2", bg, fg);
    m_mDisplay.main->addMenu(m_mDisplay.health);

    fg = menu::newTextContent("Speed: 10");
    m_mDisplay.speed = std::make_shared<Menu>(pos, size, "prop3", bg, fg);
    m_mDisplay.main->addMenu(m_mDisplay.speed);

    fg = menu::newTextContent("Bounty: 10");
    m_mDisplay.bounty = std::make_shared<Menu>(pos, size, "prop4", bg, fg);
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
    const olc::vf2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_sDisplay.main = std::make_shared<Menu>(pos, size, "sMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Health: 10");
    m_sDisplay.health = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
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
    const olc::vf2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_wDisplay.main = std::make_shared<Menu>(pos, size, "wMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Health: 10");
    m_wDisplay.health = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
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
    float c = towers::getCost(*m_tType);
    if (m_gold < c) {
      log(
        "Can't afford tower " + towers::toString(*m_tType) + " costing " +
        std::to_string(c) + " with only " +
        std::to_string(m_gold) + " gold available",
        utils::Level::Error
      );

      return;
    }

    m_gold -= c;

    Tower::TProps pp = towers::generateProps(*m_tType, p);
    towers::Processes td = towers::generateData(*m_tType);

    log(
      "Generated tower " + towers::toString(*m_tType) + " at " + p.toString(),
      utils::Level::Info
    );

    TowerShPtr t = std::make_shared<Tower>(pp, td);
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

    // Register the wall as the one being followed.
    m_wDisplay.wall = w;
  }

  void
  Game::updateUI() {
    // Update status menu.
    int v = static_cast<int>(m_lives);
    m_mLives->setText("Lives: " + std::to_string(v));

    v = static_cast<int>(m_gold);
    m_mGold->setText("Gold: " + std::to_string(v));

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

        m_tDisplay.props[id]->setText(msg);
        m_tDisplay.props[id]->enable(
          m_gold >= towers::getUpgradeCost(
            m_tDisplay.tower->getType(),
            it->first,
            it->second
          )
        );

        ++id;
      }

      if (id < ug.size()) {
        log(
          "Only interpreted " + std::to_string(id) + " among " + std::to_string(ug.size()) + " available",
          utils::Level::Error
        );
      }
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
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Sniper;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Cannon;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Freezing;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Venom;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Splash;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Blast;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Multishot;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Minigun;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Antiair;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Tesla;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
    t = towers::Type::Missile;
    m_tMenus[t]->enable(m_gold >= towers::getCost(t));
  }

}
