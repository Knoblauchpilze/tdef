#ifndef    GAME_HH
# define   GAME_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "World.hh"
# include "Tower.hh"
# include "Menu.hh"

namespace tdef {

  class Game: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new game with default parameters.
       */
      Game();

      ~Game();

      int
      w() const noexcept;

      int
      h() const noexcept;

      /**
       * @brief - Used to perform the creation of the menus
       *          allowing to control the world wrapped by
       *          this game.
       * @param dims - the dimensions of the window in which
       *               menus will be displayed.
       * @return - the list of menus representing this game.
       */
      std::vector<MenuShPtr>
      generateMenus(const olc::vi2d& dims);

      /**
       * @brief - Defines the type of tower to spawn upon
       *          invoking the `performAction` method.
       * @param type - the type of tower to spawn.
       */
      void
      setTowerType(const towers::Type& type);

      /**
       * @brief - Defines that a wall should be prepared to
       *          be spawned. This overrides any kind of
       *          tower that might have been defined.
       */
      void
      allowWallBuilding();

      /**
       * @brief - Used to create a tower with the specified
       *          type (as defined by the `setTowerType`)
       *          method at the specified position. Note
       *          that we will only perform the action if
       *          no elements exist here.
       * @param x - the abscissa of the position at which the
       *            action should be taking place.
       * @param y - the ordinate of the position at which the
       *            action will be taking place.
       */
      void
      performAction(float x, float y);

      /**
       * @brief - Returns the total remaining lives for all
       *          the portals registered in the world.
       * @return - the remaining lives in any portal of the
       *           world.
       */
      float
      lives() const noexcept;

      /**
       * @brief - Forward the call to the internal world so
       *          that one can fetch the list of visible
       *          elements.
       * @param xMin - the abscissa of the top left corner
       *               of the view frustum.
       * @param yMin - the ordinate of the top left corner
       *               of the view frustum.
       * @param xMax - the abscissa of the bottom right
       *               corner of the view frustum.
       * @param yMax - the ordinate of the bottom right
       *               corner of the view frustum.
       * @param type - the type of elements to consider. If
       *               this value is set to `null` all the
       *               item types will be included.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @param sort - the algorithm to use when performing the
       *               sorting operation (none by default).
       * @return - the list of items that are visible in the
       *           input view frustum.
       */
      std::vector<world::ItemEntry>
      getVisible(float xMin,
                 float yMin,
                 float xMax,
                 float yMax,
                 const world::ItemType* type = nullptr,
                 const world::Filter* filters = nullptr,
                 world::Sort sort = world::Sort::None) const noexcept;

      /**
       * @brief - Forward the call to the locator to fetch the
       *          block with the specified index.
       * @param id - index of the tile to access.
       * @return - the tile at the specified index.
       */
      world::Block
      block(int id) const noexcept;

      /**
       * @brief - Forward the call to the locator to fetch the
       *          mob with the specified index.
       * @param id - the index of the entity to get.
       * @return - the corresponding entity.
       */
      world::Mob
      mob(int id) const noexcept;

      /**
       * @brief - Forward the call to step one step ahead
       *          in time to the internal world.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      step(float tDelta);

      /**
       * @brief - Pause the simulation of the world wrapped
       *          by this game.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      pause(float tDelta);

      /**
       * @brief - Resume the simulation of the world wrapped
       *          by the game.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      resume(float tDelta);

    private:

      /**
       * @brief - Generate the menu displaying a status for
       *          the game.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateStatusMenu(const olc::vi2d& dims);

      /**
       * @brief - Generate the menu displaying a status for
       *          the towers.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateTowersMenu(const olc::vi2d& dims) const;

      /**
       * @brief - Generate the menu displaying the upgrade
       *          options for a tower.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateUpgradeMenu(const olc::vi2d& dims) const;

      /**
       * @brief - Spawn a new tower at the specified position
       *          assuming that 1. a tower is actually set to
       *          be spawned and 2. the coordinates are valid.
       * @param p - the position of the tower.
       */
      void
      spawnTower(const utils::Point2f& p);

      /**
       * @brief - Spawn a new wall at the specified position
       *          assuming that a wall is able to be spawned
       *          and that the coordinates are valid.
       * @param p - the position of the tower.
       */
      void
      spawnWall(const utils::Point2f& p);

      /**
       * @brief - Updates the amount of gold available for the
       *          game with the provided amount. Note that we
       *          consider that this method will not be called
       *          in a concurrent context as it is not thread
       *          safe.
       * @param earned - the amount of gold earned.
       */
      void
      updateGold(float earned);

    private:

      /**
       * @brief - Definition of the height of the game menu in
       *          pixels. Allow to tailor some other display so
       *          that they don't overlap the menu.
       */
      static constexpr int MENU_HEIGHT = 50;

      /**
       * @brief - The number of gold that the player is starting
       *          with.
       */
      static constexpr int BASE_GOLD = 50;

      /**
       * @brief - The base number of lives available before the
       *          game is lost.
       */
      static constexpr int BASE_LIVES = 15;

      /**
       * @brief - The world for this game.
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
       * @brief - The type of tower to spawn if needed.
       */
      std::shared_ptr<towers::Type> m_tType;

      /**
       * @brief - Used to define that building a wall is
       *          allowed. This will usually be used to
       *          override any information about a tower
       *          to build.
       */
      bool m_wallBuilding;

      /**
       * @brief - The available lives for this game.
       */
      float m_lives;

      /**
       * @brief - The menu indicating the number of remaining lives
       *          in the game.
       */
      MenuShPtr m_mLives;

      /**
       * @brief - The available gold for this game.
       */
      float m_gold;

      /**
       * @brief - The menu indicating the amount of gold available
       *          for this game.
       */
      MenuShPtr m_mGold;

      /**
       * @brief - Holds the slot index returned by the world when
       *          registering for gold earned update.
       */
      int m_goldEarnedSlot;
  };

  using GameShPtr = std::shared_ptr<Game>;
}

# include "Game.hxx"

#endif    /* GAME_HH */
