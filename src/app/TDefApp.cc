
# include "TDefApp.hh"

namespace {

  olc::Pixel
  colorFromBlockType(const tdef::world::BlockType& type) noexcept {
    switch (type) {
      case tdef::world::BlockType::Spawner:
        return olc::Pixel(255, 128, 0);
      case tdef::world::BlockType::Wall:
        return olc::GREY;
      case tdef::world::BlockType::Portal:
        return olc::GREEN;
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
    m_world = std::make_shared<World>(100, 100, 50);
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

        // TODO: Restore health.
        sd.color = colorFromBlockType(t.type);

        drawSprite(sd, res.cf);
      }

      // Case of a mob.
      if (ie.type == world::ItemType::Mob) {
        world::Mob t = m_loc->mob(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();

        sd.radius = t.radius;

        // TODO: Restore health.
        // TODO: Restore color.
        sd.color = olc::BLUE;

        drawSprite(sd, res.cf);
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

    sd.color = olc::PINK;

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

    SetPixelMode(olc::Pixel::NORMAL);
  }

}
