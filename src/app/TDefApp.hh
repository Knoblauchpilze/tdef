#ifndef    TDEF_APP_HH
# define   TDEF_APP_HH

# include "PGEApp.hh"
# include "world/World.hh"
# include "world/Locator.hh"

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

    private:

      /**
       * @brief - Convenience structure regrouping needed props
       *          to draw a sprite.
       */
      struct SpriteDesc {
        float x;
        float y;

        float radius;

        olc::Pixel color;
      };

      void
      loadWorld() override;

      void
      loadResources() override;

      void
      loadMenuResources() override;

      virtual void
      cleanResources() override;

      void
      draw(const RenderDesc& res) override;

      void
      drawUI(const RenderDesc& res) override;

      void
      drawDebug(const RenderDesc& res) override;

      void
      onStep(float elapsed) override;

      void
      onPause(float elapsed) override;

      void
      onResume(float elapsed) override;

      /**
       * @brief - Used to draw the tile referenced by the input
       *          struct to the screen using the corresponding
       *          visual representation.
       * @param tile - the description of the tile to draw.
       * @param cf - the coordinate frame to use to perform the
       *             conversion from tile position to pixels.
       */
      void
      drawSprite(const SpriteDesc& tile, const CoordinateFrame& cf);

    private:

      /**
       * @brief - The world managed by this application.
       */
      WorldShPtr m_world;

      /**
       * @brief - An element to use to interact with elements from
       *          the world. This object provide convenience methods
       *          to access in an efficient way elements that are
       *          within a particular region or in a way that allow
       *          efficient drawing on screen (such as in ascending
       *          `z` order).
       */
      LocatorShPtr m_loc;
  };

}

# include "TDefApp.hxx"

#endif    /* TDEF_APP_HH */
