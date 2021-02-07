#ifndef    PGE_APP_HH
# define   PGE_APP_HH

# include <core_utils/CoreObject.hh>
# include "olcEngine.hh"
# include "AppDesc.hh"
# include "CoordinateFrame.hh"
# include "Controls.hh"
# include "World.hh"

namespace tdef {

  class PGEApp: public utils::CoreObject, public olc::PixelGameEngine {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param desc - contains all the needed information to
       *               create the canvas needed by the app and
       *               set up base properties.
       */
      PGEApp(const AppDesc& desc);

      /**
       * @brief - Desctruction of the object.
       */
      ~PGEApp() = default;

      /**
       * @brief - Implementation of the interface method called
       *          during the creation of the application.
       * @return - `true` if the initialization succeeded.
       */
      bool
      OnUserCreate() override;

      /**
       * @brief - Override of the main update function called at
       *          each frame.
       * @param fElapsedTime - the duration elapsed since last
       *                       frame.
       * @return - `true` if the update succeeded.
       */
      bool
      OnUserUpdate(float fElapsedTime) override;

      /**
       * @brief - Override of the destroy function which allows
       *          to release resources before the OpenGL context
       *          gets destroyed.
       * @return - `true` if the release of resources succeeded.
       */
      bool
      OnUserDestroy() override;

    protected:

      /**
       * @brief - Convenience define refering to a drawing layer.
       */
      enum class Layer {
        Draw,
        DrawDecal,
        UI,
        Debug
      };

      /**
       * @brief - Convenience structure defining the resources
       *          that can be displayed in any app. It contains
       *          pointers to the world's data, to the frames
       *          allowing to change from screen coordinates to
       *          world coordinates and the UI.
       */
      struct RenderDesc {
        CoordinateFrame& cf;
      };

      bool
      isFirstFrame() const noexcept;

      bool
      hasDebug() const noexcept;

      bool
      hasUI() const noexcept;

      /**
       * @brief - Used to assign a certain tint to the layer
       *          defined by the input descriptor.
       * @param layer - the layer for which a tint should be
       *                assigned.
       * @param tint - the tint to associate to the layer.
       */
      void
      setLayerTint(const Layer& layer, const olc::Pixel& tint);

      /**
       * @brief - Another interface method allowing to clear
       *          a rendering layer when it's disabled. This
       *          allows to make sure that we won't keep old
       *          frames displayed on top of some content if
       *          an option is toggled.
       */
      virtual void
      clearLayer();

      /**
       * @brief - Interface method allowing to load the data
       *          needed for the world displayed by this app.
       *          Called before any call to `draw` is issued.
       */
      virtual void
      loadWorld() = 0;

      /**
       * @brief - Interface method allowing inheriting classes
       *          to get a chance to load resources needed for
       *          display. This method is guaranteed to be
       *          called before the first call to `draw` is
       *          issued.
       */
      virtual void
      loadResources() = 0;

      /**
       * @brief - Interface method allowing to load and initialize
       *          menu resources to use for the UI of this app.
       *          This method will be called before any call to the
       *          `drawUI` method is issued.
       */
      virtual void
      loadMenuResources() = 0;

      /**
       * @brief - Interface method allowing inheriting classes
       *          to be notified when the app is going to be
       *          destroyed so that resources can be cleaned.
       *          The default implementation does nothing.
       */
      virtual void
      cleanResources() = 0;

      /**
       * @brief - Interface method allowing inheriting classes
       *          to be notified when the app is going to be
       *          destroyed so that menu resources can be
       *          cleaned.
       *          The default implementation does nothing.
       */
      virtual void
      cleanMenuResources() = 0;

      /**
       * @brief - Interface method to display the main content
       *          of the app. This method only handles the
       *          decal objects and is called first during the
       *          rendering step. It means that the content
       *          will be overriden by all UI and debug info.
       * @param res - the resources to help the drawing.
       */
      virtual void
      drawDecal(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method to display the main content
       *          of the app. This method is called to draw the
       *          non-decal instances of the content and is
       *          triggered right after the `drawDecal` one.
       *          It will still get overriden by UI and debug.
       * @param res - the resources that can be drawn.
       */
      virtual void
      draw(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method allowing to draw the UI of
       *          the application. This regroups menu and all
       *          needed elements that are not game elements.
       * @param res - the resources that can be drawn.
       */
      virtual void
      drawUI(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method allowing inheriting classes
       *          to perform their own drawing calls to show
       *          debug info.
       * @param res - the resources that can be drawn.
       */
      virtual void
      drawDebug(const RenderDesc& res) = 0;

      /**
       * @brief - Interface method called at each step of the
       *          application and while the pause has not been
       *          activated.
       *          Inheriting classes should implement this to
       *          provide their own logic.
       * @param elapsed - the elapsed time since the last call
       *                  to this method.
       * @return - `true` in case the program should continue
       *           running.
       */
      virtual bool
      onStep(float elapsed) = 0;

      /**
       * @brief - Interface method called at each frame when
       *          the pause has been activated.
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program should continue
       *           running.
       */
      virtual bool
      onPause(float elapsed) = 0;

      /**
       * @brief - Interface method called at each frame when
       *          the game is paused. This is usually the case
       *          right after the `onPause` method has been
       *          called (only once).
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program shoudl continue
       *           running.
       */
      virtual bool
      onPaused(float elapsed) = 0;

      /**
       * @brief - Interface method called at each frame when
       *          the resume method has been called.
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program should continue
       *           running.
       */
      virtual bool
      onResume(float elapsed) = 0;

      /**
       * @brief - Interface method called at each frame when
       *          the inputs should be handled.
       * @param c - the current state of the controls.
       * @param cf - the coordinate frame associated to this
       *             app.
       */
      virtual void
      onInputs(const controls::State& c,
               const CoordinateFrame& cf) = 0;

    private:

      /**
       * @brief - Convenience enumeration describing the state of the
       *          game. It allows to correctly handle transitions.
       */
      enum class State {
        Running,
        Pausing,
        Paused,
        Resuming
      };

      /**
       * @brief - Used to keep track of the changes in the input
       *          that were processed during a frame. It helps
       *          determining whether some unique processes need
       *          to be triggered, such as cleaning of rendering
       *          layers that will not be updated anymore.
       */
      struct InputChanges {
        bool quit;
        bool debugLayerToggled;
      };

      /**
       * @brief - Performs the initialization of the engine to make
       *          it suits our needs.
       * @param dims - a vector describing the dimensions of the
       *               canvas for this app in pixels.
       * @param pixRatio - the ratio of a pixel in the app compared
       *                   to a pixel on screen. If this value is set
       *                   set to `2` it means that each pixel in the
       *                   app's canvas will be 2x2 pixels on screen.
       */
      void
      initialize(const olc::vi2d& dims, const olc::vi2d& pixRatio);

      /**
       * @brief - Used to perform the necessary update based on
       *          the controls that the user might have used in
       *          the game.
       * @return - a state describing the changes processed in
       *           this method. It includes any exit request of
       *           the user and changes to the UI.
       */
      InputChanges
      handleInputs();

    private:

      /**
       * @brief - The index representing the main layer for this
       *          app. Given how the pixel game engine is designed
       *          we display layers with the highest order first
       *          and then in descending order.
       *          As we want the debug and UI layers to be on top
       *          of the base layer, we need to give it the largest
       *          index so that it is displayed first, and then the
       *          rest on top.
       */
      uint32_t m_mDecalLayer;

      /**
       * @brief - The index representing the main layer but where
       *          only non-decal objects can be drawn. This layer
       *          will be displayed on top of the `m_mLayer` so
       *          it limits a bit how we can intertwine the items
       *          in the rendering.
       */
      uint32_t m_mLayer;

      /**
       * @brief - The index of the layer allowing to display debug
       *          information. This layer will be assigned to the
       *          default layer created by the pixel game engine.
       *          It is needed in order to initialize the last of
       *          the layers (and thus the one that will be set on
       *          top of all the others) with meaningful data.
       */
      uint32_t m_dLayer;

      /**
       * @brief - A layer used to represent all the UI elements of
       *          the application (menu, etc).
       */
      uint32_t m_uiLayer;

      /**
       * @brief - Used to determine whether debug display is needed
       *          for this app.
       */
      bool m_debugOn;

      /**
       * @brief - Similar to the `m_debugOn` but controls whether
       *          the UI is active or not.
       */
      bool m_uiOn;

      /**
       * @brief - Indicatesthe current state of the simulation
       *          supported by this app.
       */
      State m_state;

      /**
       * @brief - A map to keep track of the state of the controls
       *          to be transmitted to the world's entities for
       *          the simulation.
       */
      controls::State m_controls;

      /**
       * @brief - Boolean allowing to display logs only on the
       *          first frame. Or do any other process a single
       *          time upon rendering the first frame.
       */
      bool m_first;

      /**
       * @brief - Holds an object allowing to convert between the
       *          various coordinate frames handled by the app. It
       *          handles conversion between cells coordinate and
       *          screen coordinates and conversely.
       */
      CoordinateFrameShPtr m_frame;
  };

}

# include "PGEApp.hxx"

#endif    /* PGE_APP_HH */
