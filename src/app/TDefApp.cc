
# include "TDefApp.hh"
# include <maths_utils/ComparisonUtils.hh>
# include "SimpleAction.hh"

namespace tdef {

  TDefApp::TDefApp(const AppDesc& desc):
    PGEApp(desc),

    m_game(nullptr),
    m_gameUI(nullptr),

    m_menus(),

    m_packs(std::make_shared<TexturePack>()),
    m_tPackID(0u),
    m_mPackID(0u),
    m_wPackID(0u)
  {}

  bool
  TDefApp::onFrame(float fElapsed) {
    if (m_gameUI->getScreen() == game::Screen::Game) {
      bool gameOver = !m_game->step(fElapsed);

      // Display the game over menu if needed.
      if (gameOver) {
        m_gameUI->setScreen(game::Screen::GameOver);
      }
    }

    return m_game->terminated();
  }

  void
  TDefApp::onInputs(const controls::State& c,
                    const CoordinateFrame& cf)
  {
    // Handle menus update and process the
    // corresponding actions.
    std::vector<ActionShPtr> actions;
    bool relevant = false;

    for (unsigned id = 0u ; id < m_menus.size() ; ++id) {
      menu::InputHandle ih = m_menus[id]->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    if (m_gameUI != nullptr) {
      menu::InputHandle ih = m_gameUI->processUserInput(c, actions);
      relevant = (relevant || ih.relevant);
    }

    for (unsigned id = 0u ; id < actions.size() ; ++id) {
      actions[id]->apply(*m_game);
    }

    bool lClick = (c.buttons[controls::mouse::Left] == controls::ButtonState::Released);
    if (lClick && !relevant) {
      olc::vf2d it;
      olc::vi2d tp = cf.pixelCoordsToTiles(olc::vi2d(c.mPosX, c.mPosY), &it);

      m_game->performAction(tp.x + it.x, tp.y + it.y);
    }

    if (c.keys[controls::keys::P]) {
      // Switch screen to pause or back to game
      // screen.
      bool paused = (m_gameUI->getScreen() == game::Screen::Pause);
      m_game->togglePause(!paused);
      m_gameUI->setScreen(paused ? game::Screen::Game : game::Screen::Pause);
    }
  }

  void
  TDefApp::loadWorld() {
    m_game = std::make_shared<Game>();
  }

  void
  TDefApp::loadMenuResources() {
    olc::vi2d dims(ScreenWidth(), ScreenHeight());

    // Generate menus and register them.
    m_menus = m_game->generateMenus(dims);
    m_gameUI = std::make_shared<GameState>(dims, game::Screen::Home);
  }

  void
  TDefApp::drawDecal(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::VERY_DARK_GREY);

    // In case we're not in the game screen, do nothing.
    if (m_gameUI->getScreen() != game::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

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
  TDefApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // In case we're not in the game screen, do nothing.
    if (m_gameUI->getScreen() != game::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

    // Fetch elements to display.
    Viewport v = res.cf.cellsViewport();
    world::ItemType ie = world::ItemType::Block;
    std::vector<world::ItemEntry> items = m_game->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      &ie,
      nullptr,
      world::Sort::None
    );

    for (unsigned i = 0 ; i < items.size() ; ++i) {
      const world::ItemEntry& wi = items[i];
      world::Block bd = m_game->block(wi.index);

      // Represent the orientation as a small line
      // centered on the block and oriented with a
      // direction similar to the parent. We only
      // want to display it for towers.
      if (bd.type != world::BlockType::Tower) {
        continue;
      }

      static const float len = 50.0f;

      olc::vf2d p = res.cf.tileCoordsToPixels(bd.p.x(), bd.p.y(), RelativePosition::BottomRight, bd.radius);
      olc::vf2d e;

      e.x = p.x + len * std::cos(bd.orientation);
      e.y = p.y + len * std::sin(bd.orientation);
      DrawLine(p, e, olc::RED);

      // Draw the aiming cone if it is not `0` rad wide.
      if (!utils::fuzzyEqual(bd.cone, 0.0f)) {
        float min = bd.orientation - bd.cone / 2.0f;
        float max = bd.orientation + bd.cone / 2.0f;

        olc::vf2d eMin(p.x + len * std::cos(min), p.y + len * std::sin(min));
        olc::vf2d eMax(p.x + len * std::cos(max), p.y + len * std::sin(max));

        FillTriangle(p, eMin, eMax, olc::GREEN);
      }
    }

    // Fetch mobs to display.
    ie = world::ItemType::Mob;
    items = m_game->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      &ie,
      nullptr,
      world::Sort::ZOrder
    );

    olc::Pixel fColor = olc::CYAN;
    olc::Pixel pColor = olc::DARK_GREEN;
    olc::Pixel sColor = olc::DARK_GREY;

    for (unsigned i = 0 ; i < items.size() ; ++i) {
      const world::ItemEntry& wi = items[i];
      world::Mob md = m_game->mob(wi.index);

      // Represent the effects currently applied to
      // the mob as small circles with an appropriate
      // color.
      if (md.freezed) {
        olc::vf2d p = res.cf.tileCoordsToPixels(md.p.x() - 0.3f, md.p.y());
        FillCircle(p, 3, fColor);
      }
      if (md.poisoned) {
        olc::vf2d p = res.cf.tileCoordsToPixels(md.p.x() + 0.0f, md.p.y());
        FillCircle(p, 3, pColor);
      }
      if (md.stunned) {
        olc::vf2d p = res.cf.tileCoordsToPixels(md.p.x() + 0.3f, md.p.y());
        FillCircle(p, 3, sColor);
      }
    }

    // Fetch projectiles to display.
    ie = world::ItemType::Projectile;
    items = m_game->getVisible(
      v.p.x,
      v.p.y,
      v.p.x + v.dims.x,
      v.p.y + v.dims.y,
      &ie,
      nullptr,
      world::Sort::None
    );

    for (unsigned i = 0 ; i < items.size() ; ++i) {
      const world::ItemEntry& wi = items[i];
      world::Projectile pd = m_game->projectile(wi.index);

      olc::vf2d p = res.cf.tileCoordsToPixels(pd.p.x(), pd.p.y(), RelativePosition::BottomRight, 2.0f);

      FillCircle(p, 4, olc::PINK);
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  TDefApp::drawUI(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, alpha::Transparent));

    // Draw the correct screen based on the current
    // state of the game.
    const game::Screen& s = m_gameUI->getScreen();
    switch (s) {
      case game::Screen::Home:
      case game::Screen::LoadGame:
      case game::Screen::Pause:
      case game::Screen::GameOver:
      case game::Screen::Exit:
        m_gameUI->render(this);
        break;
      case game::Screen::Game:
      default:
        // Continue to the rest of the function to
        // draw the game's data.
        break;
    }
    if (s != game::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

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

    if (m_gameUI->getScreen() != game::Screen::Game) {
      SetPixelMode(olc::Pixel::NORMAL);
      return;
    }

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
      const world::ItemEntry& wi = items[i];

      // Be on the safe side.
      if (wi.type != world::ItemType::Mob) {
        log(
          "Fetched item with type " + std::to_string(static_cast<int>(wi.type)) + " while requesting only entities",
          utils::Level::Warning
        );

        continue;
      }

      world::Mob md = m_game->mob(wi.index);

      // Draw the path of this entity if any.
      if (md.path.valid()) {
        olc::vf2d old;
        const std::vector<utils::Point2f>& cPoints = md.path.getPassagePoints();
        for (unsigned id = 0u ; id < cPoints.size() ; ++id) {
          olc::vf2d p = res.cf.tileCoordsToPixels(cPoints[id].x(), cPoints[id].y());
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
