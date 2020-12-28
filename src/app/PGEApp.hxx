#ifndef    PGE_APP_HXX
# define   PGE_APP_HXX

# include "PGEApp.hh"

namespace tdef {

  inline
  bool
  PGEApp::OnUserUpdate(float fElapsedTime) {
    // Handle inputs.
    InputChanges ic = handleInputs();

    // Handle game logic if needed.
    switch (m_state) {
      case State::Running:
       onStep(fElapsedTime);
        break;
      case State::Pausing:
        onPause(fElapsedTime);
        m_state = State::Paused;
        break;
      case State::Resuming:
        onResume(fElapsedTime);
        m_state = State::Running;
        break;
      case State::Paused:
      default:
        break;
    }

    // Handle user inputs.
    onInputs(m_controls, *m_frame);

    // Handle rendering: for each function
    // we will assign the draw target first
    // so that the function does not have
    // to handle it. We want to save the
    // current draw target to restore it
    // once the process is done.
    olc::Sprite* base = GetDrawTarget();

    RenderDesc res{
      *m_frame, // Coordinate frame
    };

    SetDrawTarget(m_mLayer);
    draw(res);

    if (hasUI()) {
      SetDrawTarget(m_uiLayer);
      drawUI(res);
    }
    if (!hasUI() && isFirstFrame()) {
      SetDrawTarget(m_uiLayer);
      clearLayer();
    }

    // Draw the debug layer. As it is saved
    // in the layer `0` we need to clear it
    // when it is not displayed as it will
    // be rendered on top of any other layer
    // and thus we would still display the
    // last frame when it is inactive.
    // Note that we also clear it in case
    // the debug is set to `false` from the
    // beginning of the rendering: if we
    // don't do this nothing will be visible
    // as the `0`-th layer would never be
    // updated.
    if (hasDebug()) {
      SetDrawTarget(m_dLayer);
      drawDebug(res);
    }
    if (!hasDebug() && (ic.debugLayerToggled || isFirstFrame())) {
      SetDrawTarget(m_dLayer);
      clearLayer();
    }

    // Restore the target.
    SetDrawTarget(base);

    // Not the first frame anymore.
    m_first = false;

    return !ic.quit;
  }

  inline
  bool
  PGEApp::OnUserDestroy() {
    cleanResources();
    cleanMenuResources();

    return true;
  }

  inline
  void
  PGEApp::loadResources() {}

  inline
  void
  PGEApp::cleanResources() {}

  inline
  bool
  PGEApp::isFirstFrame() const noexcept {
    return m_first;
  }

  inline
  bool
  PGEApp::hasDebug() const noexcept {
    return m_debugOn;
  }

  inline
  bool
  PGEApp::hasUI() const noexcept {
    return m_uiOn;
  }

  inline
  void
  PGEApp::clearLayer() {
    // Clear the canvas with a neutral fully transparent color.
    SetPixelMode(olc::Pixel::ALPHA);
    Clear(olc::Pixel(255, 255, 255, ALPHA_TRANSPARENT));
    SetPixelMode(olc::Pixel::NORMAL);
  }

  inline
  void
  PGEApp::initialize(const olc::vi2d& dims, const olc::vi2d& pixRatio) {
    // Construct the window. Note that we use a pixel size
    // to screen size ratio of `1` (meaning that each pixel
    // of the viewport will be represented by a pixel on
    // the screen).
    olc::rcode c = Construct(dims.x, dims.y, pixRatio.x, pixRatio.y);

    if (c != olc::OK) {
      throw utils::CoreException(
        std::string("Could not build new frontiers application"),
        std::string("Initialization failed")
      );
    }
  }

}

#endif    /* PGE_APP_HXX */
