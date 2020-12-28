
# include "TDefApp.hh"
# include "TowerMenu.hh"

namespace {

  olc::Pixel
  colorFromMobType(const tdef::world::MobType& type) noexcept {
    switch (type) {
      case tdef::world::MobType::Regular:
        return olc::APPLE_GREEN;
      case tdef::world::MobType::Fast:
        return olc::YELLOW;
      case tdef::world::MobType::Strong:
        return olc::PALE_DARK_RED;
      case tdef::world::MobType::Air:
        return olc::CORNFLOWER_BLUE;
      default:
        break;
    }

    return olc::RED;
  }

}

namespace tdef {

  TDefApp::TDefApp(const AppDesc& desc):
    PGEApp(desc),

    m_world(nullptr),
    m_loc(nullptr),

    m_sMenu(nullptr),
    m_tMenu(nullptr),
    m_uMenu(nullptr),

    m_packs(std::make_shared<TexturePack>()),
    m_tPackID(0u),
    m_wPackID(0u)
  {}

  void
  TDefApp::loadWorld() {
// # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100, 10, 5);
# endif

    m_loc = m_world->locator();
  }

  void
  TDefApp::loadMenuResources() {
    // Create the menu: we want it to have a fixed
    // height and be at the bottom of the screen.
    int w = ScreenWidth();
    int h = ScreenHeight();

    int rgb = 20;
    int mHeight = 20;

    olc::vi2d mPos(0, 0);
    olc::vf2d mSize(w, mHeight);
    olc::Pixel c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);
    menu::BackgroundDesc bg = menu::newColoredBackground(c);
    menu::MenuContentDesc fg = menu::newTextContent("");

    m_sMenu = std::make_shared<Menu>(mPos, mSize, "sMenu", bg, fg);

    // Gold amount.
    rgb *= 2;
    c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);
    bg = menu::newColoredBackground(c);
    fg = menu::newTextContent("Gold: 50");
    MenuShPtr sm = std::make_shared<Menu>(mPos, mSize, "gold", bg, fg, menu::Layout::Horizontal, false);
    m_sMenu->addMenu(sm);

    // Lives status.
    // TODO: Make this dynamic.
    fg = menu::newTextContent("Lives: 15");
    sm = std::make_shared<Menu>(mPos, mSize, "lives", bg, fg, menu::Layout::Horizontal, false);
    m_sMenu->addMenu(sm);

    // Wave count.
    fg = menu::newTextContent("Wave: 1");
    sm = std::make_shared<Menu>(mPos, mSize, "wave", bg, fg, menu::Layout::Horizontal, false);
    m_sMenu->addMenu(sm);

    // Bottom menu.
    mHeight = MENU_HEIGHT;
    mPos = olc::vi2d(0, h - mHeight);
    mSize.y = MENU_HEIGHT;
    rgb /= 2;
    c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);

    bg = menu::newColoredBackground(c);
    fg = menu::newTextContent("");
    m_tMenu = std::make_shared<Menu>(mPos, mSize, "tMenu", bg, fg);

    rgb *= 2;
    c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);
    bg = menu::newColoredBackground(c);

    fg = menu::newTextContent("Regular");
    sm = std::make_shared<TowerMenu>(mPos, mSize, towers::Type::Regular, bg, fg);
    m_tMenu->addMenu(sm);

    fg = menu::newTextContent("Snipe");
    sm = std::make_shared<TowerMenu>(mPos, mSize, towers::Type::Snipe, bg, fg);
    m_tMenu->addMenu(sm);

    fg = menu::newTextContent("Slow");
    sm = std::make_shared<TowerMenu>(mPos, mSize, towers::Type::Slow, bg, fg);
    m_tMenu->addMenu(sm);

    fg = menu::newTextContent("Cannon");
    sm = std::make_shared<TowerMenu>(mPos, mSize, towers::Type::Cannon, bg, fg);
    m_tMenu->addMenu(sm);

    // Upgrade menu.
    int mWidth = 120;
    mPos = olc::vi2d(w - mWidth, 20);
    mSize = olc::vi2d(mWidth, h - 20 - mHeight);
    rgb /= 2;
    c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);

    bg = menu::newColoredBackground(c);
    fg = menu::newTextContent("");
    m_uMenu = std::make_shared<Menu>(mPos, mSize, "menu", bg, fg, menu::Layout::Vertical);

    rgb *= 2;
    c = olc::Pixel(rgb, rgb, rgb, ALPHA_SEMI_OPAQUE);
    bg = menu::newColoredBackground(c);

    fg = menu::newTextContent("Range: 10");
    sm = std::make_shared<Menu>(mPos, mSize, "prop1", bg, fg);
    m_uMenu->addMenu(sm);

    fg = menu::newTextContent("Strength: 10");
    sm = std::make_shared<Menu>(mPos, mSize, "prop1", bg, fg);
    m_uMenu->addMenu(sm);

    fg = menu::newTextContent("Attack speed: 10");
    sm = std::make_shared<Menu>(mPos, mSize, "prop1", bg, fg);
    m_uMenu->addMenu(sm);

    fg = menu::newTextContent("Reload time: 10");
    sm = std::make_shared<Menu>(mPos, mSize, "prop1", bg, fg);
    m_uMenu->addMenu(sm);
    // TODO: Implement this.
  }

  void
  TDefApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::VERY_DARK_GREY);

    // Fetch elements to display.
    Viewport v = res.cf.cellsViewport();
    std::vector<world::ItemEntry> items = m_loc->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      nullptr,
      nullptr,
      world::Sort::None
    );

    SpriteDesc sd;
    sd.loc = RelativePosition::BottomRight;
    sd.sprite.id = 0;

    // Render background.
    int xMin = std::floor(v.p.x);
    xMin = std::max(xMin, 0);
    int yMin = std::floor(v.p.y);
    yMin = std::max(yMin, 0);
    int xMax = std::floor(v.p.x + v.dims.x);
    xMax = std::min(xMax, m_loc->w());
    int yMax = std::floor(v.p.y + v.dims.y);
    yMax = std::min(yMax, m_loc->h());

    for (int y = yMin ; y <= yMax ; ++y) {
      for (int x = xMin ; x <= xMax ; ++x) {
        sd.x = x;
        sd.y = y;

        sd.radius = 1.0f;

        sd.sprite.sprite = 0;
        sd.sprite.tint = olc::DARK_GREY;

        drawRect(sd, res.cf);
      }
    }

    // Render each element.
    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const world::ItemEntry& ie = items[id];

      // Case of a block.
      if (ie.type == world::ItemType::Block) {
        world::Block t = m_loc->block(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();
        sd.radius = t.radius;
        sd.loc = RelativePosition::Center;

        bool pack = true;
        switch (t.type) {
          case world::BlockType::Spawner:
            pack = false;
            sd.sprite.tint = olc::DARK_GREEN;
            break;
          case world::BlockType::Wall:
            sd.sprite.pack = m_wPackID;
            sd.sprite.sprite = 0;
            sd.sprite.tint = olc::WHITE;
            break;
          case world::BlockType::Portal:
            pack = false;
            sd.sprite.tint = olc::DARK_RED;
            break;
          case world::BlockType::Tower:
            sd.sprite.pack = m_tPackID;
            sd.sprite.sprite = t.id;
            sd.sprite.tint = olc::WHITE;
            break;
        }

        if (pack) {
          drawSprite(sd, res.cf);
        }
        else {
          drawRect(sd, res.cf);
        }

        if (t.type == world::BlockType::Tower) {
          drawHealthBar(sd, t.health, res.cf, Orientation::Vertical);
        }
      }

      // Case of a mob.
      if (ie.type == world::ItemType::Mob) {
        world::Mob t = m_loc->mob(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();
        sd.radius = t.radius;
        sd.loc = RelativePosition::CenterTop;

        // TODO: Change this ?
        sd.sprite.sprite = 0;
        sd.sprite.tint = colorFromMobType(t.type);

        drawRect(sd, res.cf);
        drawHealthBar(sd, t.health, res.cf);
      }

    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  TDefApp::drawUI(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Draw the cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp);

    SpriteDesc sd;
    sd.x = mtp.x;
    sd.y = mtp.y;

    sd.radius = 1.0f;

    sd.sprite.tint = olc::DARK_COBALT_BLUE;
    sd.sprite.tint.a = ALPHA_SEMI_OPAQUE;

    drawRect(sd, res.cf);

    // Render the game menus.
    if (m_sMenu != nullptr) {
      m_sMenu->render(this);
    }
    if (m_tMenu != nullptr) {
      m_tMenu->render(this);
    }
    if (m_uMenu != nullptr) {
      m_uMenu->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  TDefApp::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Draw cursor's position.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 3 * dOffset), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 2 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h - MENU_HEIGHT - 1 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    // Render entities' path and position.
    Viewport v = res.cf.cellsViewport();
    world::ItemType ie = world::ItemType::Mob;
    std::vector<world::ItemEntry> items = m_loc->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      &ie,
      nullptr,
      world::Sort::ZOrder
    );

    for (unsigned i = 0 ; i < items.size() ; ++i) {
      const world::ItemEntry& ie = items[i];

      // Be on the safe side.
      if (ie.type != world::ItemType::Mob) {
        log(
          "Fetched item with type " + std::to_string(static_cast<int>(ie.type)) + " while requesting only entities",
          utils::Level::Warning
        );

        continue;
      }

      world::Mob md = m_loc->mob(ie.index);

      // Draw the path of this entity if any.
      if (md.path.valid()) {
        olc::vf2d old;
        for (unsigned id = 0u ; id < md.path.cPoints.size() ; ++id) {
          olc::vf2d p = res.cf.tileCoordsToPixels(md.path.cPoints[id].x(), md.path.cPoints[id].y());
          FillCircle(p, 3, olc::CYAN);

          if (id > 0u) {
            DrawLine(old, p, olc::WHITE);
          }
          old = p;
        }
      }
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

}
