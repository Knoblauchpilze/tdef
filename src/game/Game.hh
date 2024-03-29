#ifndef    GAME_HH
# define   GAME_HH

# include <vector>
# include <memory>
# include <core_utils/CoreObject.hh>
# include "World.hh"
# include "Tower.hh"
# include "Mob.hh"
# include "Spawner.hh"
# include "Wall.hh"
# include "Menu.hh"
# include "GameMenu.hh"
# include "SimpleMenu.hh"

namespace tdef {

  class Game: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new game with default parameters.
       */
      Game();

      ~Game();

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
       * @brief - Used to trigger the upgrade of the tower
       *          currently displayed for the input upgrade
       *          to the next level.
       * @param upgrade - the upgrade to perform on the
       *                  current tower.
       */
      void
      upgradeTower(const towers::Upgrade& upgrade);

      /**
       * @brief - Used to trigger a modification of the tower's
       *          target mode to a new value.
       */
      void
      toggleTowerTargetMode();

      /**
       * @brief - Used to trigger the selling of the tower
       *          currently displayed in the upgrade menu.
       *          The tower will then be removed from the
       *          game.
       */
      void
      sellTower();

      /**
       * @brief - Returns the total remaining lives for all
       *          the portals registered in the world.
       * @return - the remaining lives in any portal of the
       *           world.
       */
      float
      lives() const noexcept;

      /**
       * @brief - Used to perform a reset of the data defined
       *          by this game, typically to allow a new run
       *          to be generated.
       * @param file - the name of the saved game to load for
       *               the reset operation. In case the file
       *               is empty, a new randomy generated game
       *               will be created.
       */
      void
      reset(const std::string& file = std::string());

      /**
       * @brief - Requests the game to be terminated. This is
       *          applied to the next iteration of the game
       *          loop.
       */
      void
      terminate() noexcept;

      /**
       * @brief - Returns whether or not the game has been
       *          terminated. The game is terminated when
       *          the user wants to exit the app (usually).
       */
      bool
      terminated() const noexcept;

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
       * @brief - Forward the call to the locator to fetch the
       *          projectile with the specified index.
       * @param id - the index of the entity to get.
       * @return - the corresponding entity.
       */
      world::Projectile
      projectile(int id) const noexcept;

      /**
       * @brief - Forward the call to step one step ahead
       *          in time to the internal world.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       * @param bool - `true` in case the game continues,
       *               and `false` otherwise (i.e. if the
       *               game is ended).
       */
      bool
      step(float tDelta);

      /**
       * @brief - Performs the needed operation to handle
       *          the pause and resume operation for this
       *          game. It will automatically disable the
       *          menu if needed or make it visible again.
       * @param paused - `true` in case the desired state
       *                 after executing this method is
       *                 paused and `false` otherwise (i.e.
       *                 if the desired state is actually
       *                 running).
       */
      void
      togglePause(bool paused);

      /**
       * @brief - Used to indicate that the world should be
       *          paused. Time based entities and actions
       *          should take actions to correctly resume at
       *          a later time.
       */
      void
      pause();

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       */
      void
      resume();

      /**
       * @brief - Used to perform a save operation on this world's
       *          data to the specified file.
       * @param file - the name of the file into which the world's
       *               data should be saved.
       */
      void
      save(const std::string& file) const;

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
      generateTowersMenu(const olc::vi2d& dims);

      /**
       * @brief - Generate the menu displaying the upgrade
       *          options for a tower.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateUpgradeMenu(const olc::vi2d& dims);

      /**
       * @brief - Generate the menu displaying the values
       *          for a mob.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateMobMenu(const olc::vi2d& dims);

      /**
       * @brief - Generate the menu displaying the values
       *          for a spawner.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateSpawnerMenu(const olc::vi2d& dims);

      /**
       * @brief - Generate the menu displaying the values
       *          for a wall.
       * @param dims - the dimensions of the window holding
       *               this menu.
       * @return - the generated menu.
       */
      MenuShPtr
      generateWallMenu(const olc::vi2d& dims);

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
       * @brief - Used to setup the menu allowing to display
       *          the properties of a mob with the instance
       *          provided in input.
       * @param m - the mob whose props should be displayed.
       */
      void
      displayMob(MobShPtr m) noexcept;

      /**
       * @brief - Used to setup the menu allowing to display
       *          the properties of a tower with the instance
       *          provided in input.
       * @param t - the tower whose props should be displayed.
       */
      void
      displayTower(TowerShPtr t) noexcept;

      /**
       * @brief - Used to setup the menu allowing to display
       *          the properties of a spawner with the input
       *          instance.
       * @param s - the spawner whose props will be displayed.
       */
      void
      displaySpawner(SpawnerShPtr s) noexcept;

      /**
       * @brief - Used to setup the menu allowing to display
       *          the properties of a wall with the instance
       *          provided in input.
       * @param w - the wall whose props should be displayed.
       */
      void
      displayWall(WallShPtr w) noexcept;

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

      /**
       * @brief - Used to enable or disable the menus that
       *          compose the game. This allows to easily
       *          hide any game related component.
       * @param enable - `true` if the menus are enabled.
       */
      void
      enable(bool enable);

      /**
       * @brief - Used during the step function and by any process
       *          that needs to update the UI and the text content
       *          of menus.
       */
      void
      updateUI();

    private:

      /**
       * @brief - The number of gold that the player is starting
       *          with.
       */
      static constexpr int BASE_GOLD = 5000;

      /**
       * @brief - The base number of lives available before the
       *          game is lost.
       */
      static constexpr int BASE_LIVES = 15;

      /**
       * @brief - Used to define how many upgrades are available
       *          for towers.
       */
      static constexpr unsigned UPGRADE_COUNT = 5;

      /**
       * @brief - Used to define the cost of building a wall in
       *          the game. This value is hard-coded.
       */
      static constexpr int WALL_COST = 45;

      /**
       * @brief - Convenience structure regrouping all the info
       *          on the status display.
       */
      struct StatusDisplay {
        // The main menu representing the status. Allows to easily
        // de/activate the whole display.
        MenuShPtr main;

        // The menu indicating the number of remaining lives
        // in the game.
        MenuShPtr lives;

        // The menu indicating the amount of gold available
        // for this game.
        MenuShPtr gold;

        // The menu representing the play action (to start or
        // resume the game).
        SimpleMenuShPtr play;

        // The menu representing the pause action.
        SimpleMenuShPtr pause;

        // Holds the slot index returned by the world when
        // registering for gold earned update.
        int goldEarnedSlot;
      };

      /**
       * @brief - Convenience structure regrouping all the info
       *          on a tower display.
       */
      struct TowerDisplay {
        // `main` defines the main menu that controls the display
        // of tower's info. Set it to visible or hidden according
        // to whether a tower is selected.
        MenuShPtr main;

        // The following items define the menus displaying the
        // various props of a tower.
        MenuShPtr type;
        std::vector<GameMenuShPtr> props;
        MenuShPtr sell;
        MenuShPtr targetMode;

        // `tower` defines a pointer to the tower being displayed.
        // It allows to continuously update the values displayed
        // in the menus.
        TowerShPtr tower;
      };

      /**
       * @brief - Convenience structure regrouping all the info
       *          on a mob display.
       */
      struct MobDisplay {
        // `main` defines the main menu that controls the display
        // of mob's info. Set it to visible or hidden according
        // to whether a mob is selected.
        MenuShPtr main;

        // The following items define the menus displaying the
        // various props of a mob.
        MenuShPtr type;
        MenuShPtr health;
        MenuShPtr speed;
        MenuShPtr bounty;

        // `mob` defines a pointer to the mob being displayed. It
        // allows to continuously update the values displayed in
        // the menus.
        MobShPtr mob;
      };

      /**
       * @brief - Convenience structure regrouping all the info
       *          on a wall display.
       */
      struct SpawnerDisplay {
        // `main` defines the main menu that controls the display
        // of spawner's info. Set it to visible or hidden according
        // to whether a spawner is selected.
        MenuShPtr main;

        // The following items define the menus displaying the
        // various props of a spawner.
        MenuShPtr health;

        // `spawner` defines a pointer to the wall being displayed.
        // It allows to continuously update the value displayed
        // in the menus.
        SpawnerShPtr spawner;
      };

      /**
       * @brief - Convenience structure regrouping all the info
       *          on a wall display.
       */
      struct WallDisplay {
        // `main` defines the main menu that controls the display
        // of wall's info. Set it to visible or hidden according
        // to whether a wall is selected.
        MenuShPtr main;

        // The following items define the menus displaying the
        // various props of a spawner.
        MenuShPtr health;

        // `wall` defines a pointer to the wall being displayed.
        // It allows to continuously update the value displayed
        // in the menus.
        WallShPtr wall;
      };

      /**
       * @brief - Convenience enumeration allowing to save the
       *          current information being displayed in the
       *          lateral info panel.
       */
      enum class InfoPanelStatus {
        Tower,
        Mob,
        Spawner,
        Wall,
        None
      };

      /**
       * @brief - Convenience information defining the state of the
       *          game. It includes information about whether the
       *          menus should be displayed and if the user actions
       *          should be interpreted or not.
       */
      struct State {
        // Defines whether this world is paused (i.e.
        // internal attributes of the mobs/blocks/etc
        // have already been updated to reflect the
        // pause status) or not. This allows to react
        // to consecutive pause requests and prevent
        // weird behaviors to occur.
        bool paused;

        // Whether or not the UI is disabled.
        bool disabled;

        // Used to hold whether or not the game has been shut
        // down. It usually indicates that no simulation will
        // be performed anymore and usually indicates that a
        // termination request has been received.
        bool terminated;

        // Defines the current information displayed in the
        // lateral information panel. This is updated based
        // on which element of the game is selected and is
        // used in case the menu is disabled to restore the
        // correct info displayed.
        // Default value is set to none as no information is
        // displayed at the beginning of the game.
        InfoPanelStatus infoState;

        // Used to define that building a wall is
        // allowed. This will usually be used to
        // override any information about a tower
        // to build.
        bool wallBuilding;

        // The type of tower to spawn if needed.
        std::shared_ptr<towers::Type> tType;

        // The available lives for this game.
        float lives;

        // The available gold for this game.
        float gold;
      };

      /**
       * @brief - Convenience define to refer to a map of menus
       *          adressed based on the type of tower it defines.
       */
      using TowerMenus = std::unordered_map<towers::Type, GameMenuShPtr>;

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
       * @brief - The definition of the game state.
       */
      State m_state;

      /**
       * @brief - The status menu regrouping both the number of
       *          lives remaining, the amount of gold available
       *          and other information about the game.
       */
      StatusDisplay m_statusDisplay;

      /**
       * @brief - The properties of the tower display menus. This
       *          attribute is only valid once the corresponding
       *          generation method has been called.
       */
      TowerDisplay m_tDisplay;

      /**
       * @brief - The properties of the mob display menus. This
       *          attribute is only valid once the corresponding
       *          generation method has been called.
       */
      MobDisplay m_mDisplay;

      /**
       * @brief - The properties of the spawner display menus. It
       *          is only valid once the corresponding generation
       *          method has been called.
       */
      SpawnerDisplay m_sDisplay;

      /**
       * @brief - The properties of the wall display menus. This
       *          attribute is only valid once the corresponding
       *          generation method has been called.
       */
      WallDisplay m_wDisplay;

      /**
       * @brief - The main menu holding all the information about
       *          towers and other buildings construction.
       */
      MenuShPtr m_buildings;

      /**
       * @brief - Defines the list of menus allowing to build a
       *          new tower in the game. We need to de/activate
       *          some entries based on the amount of gold the
       *          player has at any moment.
       */
      TowerMenus m_tMenus;

      /**
       * @brief - Holds the menu controlling the construction of
       *          a wall.
       */
      GameMenuShPtr m_wMenu;
  };

  using GameShPtr = std::shared_ptr<Game>;
}

# include "Game.hxx"

#endif    /* GAME_HH */
