
# include "TDefApp.hh"

namespace tdef {

  TDefApp::TDefApp(const AppDesc& desc):
    PGEApp(desc),

    m_game(nullptr),

    m_menus(),

    m_packs(std::make_shared<TexturePack>()),
    m_tPackID(0u),
    m_mPackID(0u),
    m_wPackID(0u)
  {}

  void
  TDefApp::loadWorld() {
    m_game = std::make_shared<Game>();
  }

  void
  TDefApp::loadMenuResources() {
    olc::vi2d dims(ScreenWidth(), ScreenHeight());

    // Generate menus and register them.
    m_menus = m_game->generateMenus(dims);
  }

  void
  TDefApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::VERY_DARK_GREY);

    // Fetch elements to display.
    Viewport v = res.cf.cellsViewport();
    std::vector<world::ItemEntry> items = m_game->getVisible(
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
    int yMin = std::floor(v.p.y);
    int xMax = std::ceil(v.p.x + v.dims.x);
    int yMax = std::ceil(v.p.y + v.dims.y);

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
        world::Block t = m_game->block(ie.index);

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
        world::Mob t = m_game->mob(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();
        sd.radius = t.radius;
        sd.loc = RelativePosition::Center;

        sd.sprite.pack = m_mPackID;
        sd.sprite.sprite = t.id;
        sd.sprite.tint = olc::WHITE;

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
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // Draw the cursor.
    olc::vi2d mp = GetMousePos();
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp);

    SpriteDesc sd;
    sd.x = mtp.x;
    sd.y = mtp.y;

    sd.radius = 1.0f;

    sd.sprite.tint = olc::DARK_COBALT_BLUE;
    sd.sprite.tint.a = alpha::SemiOpaque;

    drawRect(sd, res.cf);

    // Render the game menus.
    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      m_menus[id]->render(this);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  TDefApp::drawDebug(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // Draw cursor's position.
    olc::vi2d mp = GetMousePos();
    olc::vf2d it;
    olc::vi2d mtp = res.cf.pixelCoordsToTiles(mp, &it);

    int h = GetDrawTargetHeight();
    int dOffset = 15;
    DrawString(olc::vi2d(0, h / 2), "Mouse coords      : " + toString(mp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 1 * dOffset), "World cell coords : " + toString(mtp), olc::CYAN);
    DrawString(olc::vi2d(0, h / 2 + 2 * dOffset), "Intra cell        : " + toString(it), olc::CYAN);

    // Render entities' path and position.
    Viewport v = res.cf.cellsViewport();
    world::ItemType ie = world::ItemType::Mob;
    std::vector<world::ItemEntry> items = m_game->getVisible(
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

      world::Mob md = m_game->mob(ie.index);

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
