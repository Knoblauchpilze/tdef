
# include "Game.hh"
# include <cxxabi.h>
# include "TowerFactory.hh"
# include "GameMenu.hh"
# include "SimpleAction.hh"
# include "Mob.hh"
# include "Tower.hh"
# include "Spawner.hh"
# include "Wall.hh"

namespace {

  tdef::MenuShPtr
  generateTowerMenu(const tdef::towers::Type& type)
  {
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
    m_wDisplay()
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
        return;
      }

      TowerShPtr t = std::dynamic_pointer_cast<Tower>(we);
      if (t != nullptr) {
        displayTower(t);
        return;
      }

      SpawnerShPtr s = std::dynamic_pointer_cast<Spawner>(we);
      if (s != nullptr) {
        displaySpawner(s);
        return;
      }

      WallShPtr w = std::dynamic_pointer_cast<Wall>(we);
      if (w != nullptr) {
        displayWall(w);
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
    m_world->step(tDelta);

    // Update the status based on the game data.
    m_lives = lives();

    int v = static_cast<int>(m_lives);
    menu::MenuContentDesc fg = menu::newTextContent("Lives: " + std::to_string(v));
    m_mLives->setContent(fg);

    v = static_cast<int>(m_gold);
    fg = menu::newTextContent("Gold: " + std::to_string(v));
    m_mGold->setContent(fg);

    // Update display values for visible menus.
    if (m_tDisplay.tower != nullptr) {
      std::string t = towers::toString(m_tDisplay.tower->getType());
      fg = menu::newTextContent("Type: " + t);
      m_tDisplay.type->setContent(fg);

      float v = m_tDisplay.tower->getRange();
      fg = menu::newTextContent("Range: " + std::to_string(v));
      m_tDisplay.range->setContent(fg);

      v = m_tDisplay.tower->getAttack();
      fg = menu::newTextContent("Damage: " + std::to_string(v));
      m_tDisplay.damage->setContent(fg);

      v = m_tDisplay.tower->getAttackSpeed();
      fg = menu::newTextContent("Attack speed: " + std::to_string(v));
      m_tDisplay.attackSpeed->setContent(fg);
    }

    if (m_mDisplay.mob != nullptr) {
      std::string t = mobs::toString(m_mDisplay.mob->getType());
      fg = menu::newTextContent("Type: " + t);
      m_mDisplay.type->setContent(fg);

      float v = m_mDisplay.mob->getHealth();
      fg = menu::newTextContent("Health: " + std::to_string(v));
      m_mDisplay.health->setContent(fg);

      v = m_mDisplay.mob->getSpeed();
      fg = menu::newTextContent("Speed: " + std::to_string(v));
      m_mDisplay.speed->setContent(fg);

      v = m_mDisplay.mob->getBounty();
      fg = menu::newTextContent("Bounty: " + std::to_string(v));
      m_mDisplay.bounty->setContent(fg);
    }

    if (m_sDisplay.spawner != nullptr) {
      float v = m_sDisplay.spawner->getHealth();
      fg = menu::newTextContent("Health: " + std::to_string(v));
      m_sDisplay.health->setContent(fg);
    }
    
    if (m_wDisplay.wall != nullptr) {
      float v = m_wDisplay.wall->getHealth();
      fg = menu::newTextContent("Health: " + std::to_string(v));
      m_wDisplay.health->setContent(fg);
    }
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
  Game::generateTowersMenu(const olc::vi2d& dims) const {
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

    m->addMenu(generateTowerMenu(towers::Type::Basic));
    m->addMenu(generateTowerMenu(towers::Type::Sniper));
    m->addMenu(generateTowerMenu(towers::Type::Cannon));
    m->addMenu(generateTowerMenu(towers::Type::Freezing));
    m->addMenu(generateTowerMenu(towers::Type::Venom));
    m->addMenu(generateTowerMenu(towers::Type::Splash));
    m->addMenu(generateTowerMenu(towers::Type::Blast));
    m->addMenu(generateTowerMenu(towers::Type::Multishot));
    m->addMenu(generateTowerMenu(towers::Type::Minigun));
    m->addMenu(generateTowerMenu(towers::Type::Antiair));
    m->addMenu(generateTowerMenu(towers::Type::Tesla));
    m->addMenu(generateTowerMenu(towers::Type::Missile));

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
    const olc::vi2d pos(dims.x - 150, 20);
    const olc::vf2d size(150, dims.y - 20 - 50);

    menu::BackgroundDesc bg = menu::newColoredBackground(bgc);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_tDisplay.main = std::make_shared<Menu>(pos, size, "uMenu", bg, fg, menu::Layout::Vertical);

    // Adapt color for the sub menus background.
    const olc::Pixel smbgc(20, 20, 20, alpha::SemiOpaque);
    bg = menu::newColoredBackground(smbgc);

    fg = menu::newTextContent("Type: Regular");
    m_tDisplay.type = std::make_shared<Menu>(pos, size, "prop1", bg, fg);
    m_tDisplay.main->addMenu(m_tDisplay.type);

    fg = menu::newTextContent("Range: 10");
    m_tDisplay.range = std::make_shared<Menu>(pos, size, "prop2", bg, fg);
    m_tDisplay.main->addMenu(m_tDisplay.range);

    fg = menu::newTextContent("Damage: 10");
    m_tDisplay.damage = std::make_shared<Menu>(pos, size, "prop3", bg, fg);
    m_tDisplay.main->addMenu(m_tDisplay.damage);

    fg = menu::newTextContent("Attack speed: 10");
    m_tDisplay.attackSpeed = std::make_shared<Menu>(pos, size, "prop4", bg, fg);
    m_tDisplay.main->addMenu(m_tDisplay.attackSpeed);

    fg = menu::newTextContent("Sell");
    MenuShPtr sm = std::make_shared<Menu>(pos, size, "prop5", bg, fg);
    m_tDisplay.main->addMenu(sm);

    fg = menu::newTextContent("Target mode");
    sm = std::make_shared<Menu>(pos, size, "prop6", bg, fg);
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
    // Generate the tower.
    Tower::TProps pp;
    towers::Data td;

    bool valid = true;
    switch (*m_tType) {
      case towers::Type::Basic:
        pp = towers::generateProps(towers::Type::Basic, p);
        td = towers::generateData(towers::Type::Basic);
        break;
      case towers::Type::Sniper:
        pp = towers::generateProps(towers::Type::Sniper, p);
        td = towers::generateData(towers::Type::Sniper);
        break;
      case towers::Type::Freezing:
        pp = towers::generateProps(towers::Type::Freezing, p);
        td = towers::generateData(towers::Type::Freezing);
        break;
      case towers::Type::Cannon:
        pp = towers::generateProps(towers::Type::Cannon, p);
        td = towers::generateData(towers::Type::Cannon);
        break;
      case towers::Type::Venom:
        pp = towers::generateProps(towers::Type::Venom, p);
        td = towers::generateData(towers::Type::Venom);
        break;
      case towers::Type::Splash:
        pp = towers::generateProps(towers::Type::Splash, p);
        td = towers::generateData(towers::Type::Splash);
        break;
      case towers::Type::Blast:
        pp = towers::generateProps(towers::Type::Blast, p);
        td = towers::generateData(towers::Type::Blast);
        break;
      case towers::Type::Multishot:
        pp = towers::generateProps(towers::Type::Multishot, p);
        td = towers::generateData(towers::Type::Multishot);
        break;
      case towers::Type::Minigun:
        pp = towers::generateProps(towers::Type::Minigun, p);
        td = towers::generateData(towers::Type::Minigun);
        break;
      case towers::Type::Antiair:
        pp = towers::generateProps(towers::Type::Antiair, p);
        td = towers::generateData(towers::Type::Antiair);
        break;
      case towers::Type::Tesla:
        pp = towers::generateProps(towers::Type::Tesla, p);
        td = towers::generateData(towers::Type::Tesla);
        break;
      case towers::Type::Missile:
        pp = towers::generateProps(towers::Type::Missile, p);
        td = towers::generateData(towers::Type::Missile);
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

    log("Generated tower " + std::to_string(static_cast<int>(*m_tType)) + " at " + p.toString());

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

}
