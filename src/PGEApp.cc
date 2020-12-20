
# include "PGEApp.hh"
# include "ColorUtils.hh"
# include "utils.hh"

namespace {

  olc::Pixel
  colorFromBlockType(const tdef::world::BlockType& type) noexcept {
    switch (type) {
      case tdef::world::BlockType::Spawner:
        return olc::Pixel(255, 128, 0);
      case tdef::world::BlockType::Wall:
        return olc::DARK_GREY;
      default:
        break;
    }

    return olc::RED;
  }

}

namespace tdef {

  PGEApp::PGEApp(const AppDesc& desc):
    utils::CoreObject(desc.name),
    olc::PixelGameEngine(),

    m_mLayer(0u),
    m_dLayer(0u),
    m_uiLayer(0u),

    m_debugOn(false),
    m_uiOn(true),

    m_state(State::Running),
    m_first(true),

    m_frame(desc.frame)
  {
    // Initialize the application settings.
    sAppName = desc.name;
    setService("app");

    // Make sure that a coordinate frame is provided.
    if (m_frame == nullptr) {
      error(
        std::string("Unable to create PGE application"),
        std::string("Invalid null coordinate frame provided")
      );
    }

    // Generate and construct the window.
    initialize(desc.dims, desc.pixRatio);
  }

  bool
  PGEApp::OnUserCreate() {
// # define WORLD_FROM_FILE
# ifdef WORLD_FROM_FILE
    m_world = std::make_shared<World>(100, std::string("data/worlds/level_1.lvl"));
# else
    m_world = std::make_shared<World>(100, 100, 50);
# endif

    m_loc = m_world->locator();

    // Load the menu resources.
    loadMenuResources();

    // And other resources.
    loadResources();

    // The debug layer is the default layer: it is always
    // provided by the pixel game engine.
    m_dLayer = 0u;

    // Create a layer for the UI elements and enable it.
    m_uiLayer = CreateLayer();
    EnableLayer(m_uiLayer, true);

    // And finally create a layer for the main content: as
    // the pixel game engine draws layers from back to front
    // the main content should be at the back so that all
    // other elements are displayed on top of it.
    m_mLayer = CreateLayer();
    EnableLayer(m_mLayer, true);

    return true;
  }

  void
  PGEApp::draw(const RenderDesc& res) {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // Fetch elements to display.
    Viewport v = res.cf.cellsViewport();
    std::vector<world::ItemEntry> items = res.loc->getVisible(
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
    xMax = std::min(xMax, res.loc->w());
    int yMax = std::floor(v.p.y + v.dims.y);
    yMax = std::min(yMax, res.loc->h());

    for (int y = yMin ; y <= yMax ; ++y) {
      for (int x = xMin ; x <= xMax ; ++x) {
        sd.x = x;
        sd.y = y;

        sd.radius = 1.0f;

        sd.color = olc::VERY_DARK_GREY;

        drawSprite(sd, res.cf);
      }
    }

    // Render each element.
    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const world::ItemEntry& ie = items[id];

      // Case of a block.
      if (ie.type == world::ItemType::Block) {
        world::Block t = res.loc->block(ie.index);

        sd.x = t.p.x();
        sd.y = t.p.y();

        sd.radius = t.radius;

        // TODO: Restore health.
        sd.color = colorFromBlockType(t.type);

        drawSprite(sd, res.cf);
      }

      // Case of a mob.
      if (ie.type == world::ItemType::Mob) {
        world::Mob t = res.loc->mob(ie.index);

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
  PGEApp::drawUI(const RenderDesc& /*res*/) {
    clearLayer();
  }

  void
  PGEApp::drawDebug(const RenderDesc& /*res*/) {
    clearLayer();
  }

  void
  PGEApp::loadMenuResources() {
    // Nothing to do here.
  }

  PGEApp::InputChanges
  PGEApp::handleInputs() {
    InputChanges ic{false, false};

    // Detect press on `Escape` key to shutdown the app.
    olc::HWButton esc = GetKey(olc::ESCAPE);
    if (esc.bReleased) {
      ic.quit = true;
      return ic;
    }

    // In case we're dragging the right mouse button we
    // will update the world's position (panning). What
    // we want is to remember the position at the moment
    // of the click and then continuously move the world
    // to match the current displacement.
    if (GetMouse(1).bPressed) {
      m_frame->beginTranslation(GetMousePos());
    }
    if (GetMouse(1).bHeld) {
      m_frame->translate(GetMousePos());
    }

    int scroll = GetMouseWheel();
    if (scroll > 0) {
      m_frame->zoomIn(GetMousePos());
    }
    if (scroll < 0) {
      m_frame->zoomOut(GetMousePos());
    }

    // De/activate the debug mode if needed and
    // handle general simulation control options.
    if (GetKey(olc::D).bReleased) {
      m_debugOn = !m_debugOn;
      ic.debugLayerToggled = true;
    }
    if (GetKey(olc::U).bReleased) {
      m_uiOn = !m_uiOn;
    }

    if (GetKey(olc::P).bReleased) {
      switch (m_state) {
        case State::Running:
        case State::Resuming:
          m_state = State::Pausing;
          break;
        case State::Paused:
          m_state = State::Resuming;
          break;
        case State::Pausing:
        default:
          break;
      }
    }

    return ic;
  }

}
