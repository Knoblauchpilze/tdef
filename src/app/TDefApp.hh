#ifndef    TDEF_APP_HH
# define   TDEF_APP_HH

# include "PGEApp.hh"
# include "World.hh"
# include "Locator.hh"
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

      void
      onInputs(const controls::State& c,
               const CoordinateFrame& cf) override;

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
       * @brief - Definition of the height of the game menu in
       *          pixels. Allow to tailor some other display so
       *          that they don't overlap the menu.
       */
      static constexpr int MENU_HEIGHT = 50;

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

      /**
       * @brief - Defines the menu allowing to display information
       *          on the status of the player.
       */
      MenuShPtr m_sMenu;

      /**
       * @brief - Defines the menu allowing to purchase towers for
       *          the game.
       */
      MenuShPtr m_tMenu;

      /**
       * @brief - Defines the menu allowing to upgrade and modify
       *          a selected tower.
       */
      MenuShPtr m_uMenu;

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
