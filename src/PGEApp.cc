
# include "PGEApp.hh"
# include "utils.hh"

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
    m_first(true)
  {
    // Initialize the application settings.
    sAppName = desc.name;
    setService("app");

    // Generate and construct the window.
    initialize(desc.dims, desc.pixRatio);
  }

  bool
  PGEApp::OnUserCreate() {
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
    // Nothing to do yet.
  }

  void
  PGEApp::drawUI() {
    // Nothing to do yet.
  }

  void
  PGEApp::drawDebug() {
    // Nothing to do yet.
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
