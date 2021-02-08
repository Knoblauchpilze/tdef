#ifndef    TDEF_APP_HH
# define   TDEF_APP_HH

# include "PGEApp.hh"
# include "Game.hh"
# include "GameState.hh"
# include "Menu.hh"
# include "TexturePack.hh"

namespace tdef {

  class TDefApp: public PGEApp {
    public:

      /**
       * @brief - Create a new default pixel game engine app.
       * @param desc - contains all the needed information to
       *               create the canvas needed by the app and
       *               set up base properties.
       */
      TDefApp(const AppDesc& desc);

      /**
       * @brief - Desctruction of the object.
       */
      ~TDefApp() = default;

    protected:

      bool
      onFrame(float fElapsed) override;

      void
      onInputs(const controls::State& c,
               const CoordinateFrame& cf) override;

    private:

      /**
       * @brief - Convenience structure regrouping needed props
       *          to draw a sprite.
       */
      struct SpriteDesc {
        float x;
        float y;

        float radius;
        RelativePosition loc;

        sprites::Sprite sprite;
      };

      /**
       * @brief - Describe a possible orientation for a graphic
       *          component (e.g. a healthbar, etc.).
       */
      enum class Orientation {
        Horizontal,
        Vertical
      };

      void
      loadWorld() override;

      void
      loadResources() override;

      void
      loadMenuResources() override;

      void
      cleanResources() override;

      void
      cleanMenuResources() override;

      void
      drawDecal(const RenderDesc& res) override;

      void
      draw(const RenderDesc& res) override;

      void
      drawUI(const RenderDesc& res) override;

      void
      drawDebug(const RenderDesc& res) override;

      /**
       * @brief - Interface method called at each step of the
       *          application and while the pause has not been
       *          activated.
       * @param elapsed - the elapsed time since the last call
       *                  to this method.
       * @return - `true` in case the program should continue
       *           running.
       */
      bool
      onStep(float elapsed);

      /**
       * @brief - Interface method called at each frame when
       *          the pause has been activated.
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program should continue
       *           running.
       */
      bool
      onPause(float elapsed);

      /**
       * @brief - Interface method called at each frame when
       *          the game is paused. This is usually the case
       *          right after the `onPause` method has been
       *          called (only once).
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program shoudl continue
       *           running.
       */
      bool
      onPaused(float elapsed);

      /**
       * @brief - Interface method called at each frame when
       *          the resume method has been called.
       * @param elapsed - the elapsed time since the last call.
       * @return - `true` in case the program should continue
       *           running.
       */
      bool
      onResume(float elapsed);

      /**
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param t - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawSprite(const SpriteDesc& t, const CoordinateFrame& cf);

      /**
       * @brief - Used to draw a simple rect at the specified
       *          location. Note that we reuse the sprite desc
       *          but don't actually use the sprite.
       * @param t - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawRect(const SpriteDesc& t,
               const CoordinateFrame& cf);

      /**
       * @brief - Used to draw a minimalistic health bar for an entity
       *          or block at position (x, y) in cells coordinates. It
       *          is displayed with the specified alpha transparency.
       * @param t - the description of the tile to draw.
       * @param ratio - the ratio of the healthbar that is still full.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       * @param o - the orientation of the healtbar.
       */
      void
      drawHealthBar(const SpriteDesc& t,
                    float ratio,
                    const CoordinateFrame& cf,
                    const Orientation& o = Orientation::Horizontal);

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
       * @brief - The game managed by this application.
       */
      GameShPtr m_game;

      /**
       * @brief - The current state of the game: this is used
       *          to adapt what's displayed.
       */
      GameStateShPtr m_gameUI;

      /**
       * @brief - Indicatesthe current state of the simulation
       *          supported by this app.
       */
      State m_state;

      /**
       * @brief - Defines the list of menus available for
       *          this app.
       */
      std::vector<MenuShPtr> m_menus;

      /**
       * @brief - A description of the textures used to represent
       *          the elements of the game.
       */
      TexturePackShPtr m_packs;

      /**
       * @brief - A description of the identifier used to represent
       *          the texture pack for towers.
       */
      unsigned m_tPackID;

      /**
       * @brief - A description of the identifier used to represent
       *          the texture pack for mobs.
       */
      unsigned m_mPackID;

      /**
       * @brief - A description of the identifier used to represent
       *          the texture pack for walls.
       */
      unsigned m_wPackID;
  };

}

# include "TDefApp.hxx"

#endif    /* TDEF_APP_HH */
