
# include "TDefApp.hh"

namespace {

  olc::Pixel
  colorFromBlockType(const tdef::world::BlockType& type) noexcept {
    switch (type) {
      case tdef::world::BlockType::Spawner:
        return olc::ORANGE;
      case tdef::world::BlockType::Wall:
        return olc::GREY;
      case tdef::world::BlockType::Portal:
        return olc::GREEN;
      default:
        break;
    }

    return olc::RED;
  }

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
    m_loc(nullptr)
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
      world::Sort::ZOrder
    );

    SpriteDesc sd;
    sd.loc = RelativePosition::BottomRight;

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

        sd.color = olc::DARK_GREY;

        drawSprite(sd, res.cf);
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
        sd.loc = RelativePosition::BottomRight;

        sd.color = colorFromBlockType(t.type);

        drawSprite(sd, res.cf);
        drawHealthBar(sd, t.health, res.cf, Orientation::Vertical);
      }

      // Case of a mob.
      if (ie.type == world::ItemType::Mob) {
        world::Mob t = m_loc->mob(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();
        sd.radius = t.radius;
        sd.loc = RelativePosition::CenterTop;

        sd.color = colorFromMobType(t.type);

        drawSprite(sd, res.cf);
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

    sd.color = olc::DARK_COBALT_BLUE;
    sd.color.a = ALPHA_SEMI_OPAQUE;

    drawSprite(sd, res.cf);

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
