
# include "PGEApp.hh"
# include "ColorUtils.hh"
# include "utils.hh"

namespace {

  olc::Pixel
  colorFromState(tdef::world::State s) {
    switch (s) {
      case tdef::world::State::Empty:
        return olc::VERY_DARK_GREY;
      case tdef::world::State::Colony:
        return olc::DARK_GREEN;
      case tdef::world::State::Deposit:
        return olc::DARK_YELLOW;
      case tdef::world::State::Worker:
        return olc::DARK_BLUE;
      case tdef::world::State::Warrior:
        return olc::VERY_DARK_BLUE;
      default:
        return olc::RED;
    }
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
    m_world = std::make_shared<World>(100, 15, 15);
# endif

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
  PGEApp::draw() {
    // Clear rendering target.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::BLACK);

    // Fetch the visible cells.
    Viewport v = m_frame->cellsViewport();

    int xMin = std::floor(v.p.x);
    int yMin = std::floor(v.p.y);
    int xMax = std::floor(v.p.x + v.dims.x);
    int yMax = std::floor(v.p.y + v.dims.y);

    SpriteDesc desc;
    bool valid;

    for (int y = yMin ; y <= yMax ; ++y) {
      for (int x = xMin ; x <= xMax ; ++x) {
        world::State s = m_world->cell(x, y, valid);

        if (!valid) {
          continue;
        }

        desc.x = x;
        desc.y = y;
        desc.color = colorFromState(s);

        drawSprite(desc, *m_frame);
      }
    }

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  PGEApp::drawUI() {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Nothing to do yet.

    SetPixelMode(olc::Pixel::NORMAL);
  }

  void
  PGEApp::drawDebug() {
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));

    // Nothing to do yet.

    SetPixelMode(olc::Pixel::NORMAL);
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
