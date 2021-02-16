#ifndef    GAME_STATE_HH
# define   GAME_STATE_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "olcEngine.hh"
# include "Menu.hh"
# include "GameMenu.hh"

namespace tdef {
  namespace game {

    /**
     * @brief - Convenience enumeration defining the current state
     *          of the application: each value roughly corresponds
     *          to a distinct menu in the application.
     */
    enum class Screen {
      Home,
      LoadGame,
      Game,
      Pause,
      GameOver,
      Exit
    };

  }

  class GameState: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new game state with the specified
       *          state.
       * @param dims - the dimensions of the menu: this might
       *               correspond to the size of the window
       *               where the menu will be displayed.
       * @param screen - the current screen.
       */
      GameState(const olc::vi2d& dims,
                const game::Screen& screen = game::Screen::Home);

      /**
       * @brief - Retrieves the currently selected screen.
       * @return - the currently selected screen.
       */
      const game::Screen&
      getScreen() const noexcept;

      /**
       * @brief - Retrieves the currently selected path to
       *          reach the world's data. In case the data
       *          is empty it usually indicates that a new
       *          world should be created.
       * @return - a string to the world's data file.
       */
      const std::string&
      getWorldFile() const noexcept;

      /**
       * @brief - Used to define a new current state for
       *          the game. This method will also update
       *          the visibility of each menu to make it
       *          easy to render the corresponding screen
       *          once it's selected.
       * @param screen - the new screen to apply.
       */
      void
      setScreen(const game::Screen& screen);

      /**
       * @brief - Defines a new value for the selected world
       *          to load. This will overrides the file path
       *          to reach the saved world's data.
       * @param file - the path to the file describing the
       *               world's to load data.
       */
      void
      setWorldFile(const std::string& file);

      /**
       * @brief - Used to render the screen corresponding to
       *          the current state using the provided game
       *          renderer.
       * @param pge - the engine to use to render the game
       *              state.
       */
      void
      render(olc::PixelGameEngine* pge) const;

      /**
       * @brief - Performs the interpretation of the controls
       *          provided as input to update the selected
       *          screen. Actions may be generated through
       *          this mechanism.
       * @param c - the controls and user input for this
       *            frame.
       * @param actions - the list of actions produced by the
       *                  menu while processing the events.
       * @return - the description of what happened when the
       *           inputs has been processed.
       */
      menu::InputHandle
      processUserInput(const controls::State& c,
                       std::vector<ActionShPtr>& actions);

    private:

      /**
       * @brief - Create a valid home screen menu.
       * @param dims - the dimensions of the parent window.
       */
      void
      generateHomeScreen(const olc::vi2d& dims);

      /**
       * @brief - Create a valid load game screen menu.
       * @param dims - the dimensions of the parent window.
       */
      void
      generateLoadGameScreen(const olc::vi2d& dims);

      /**
       * @brief - Create a valid pause screen menu.
       * @param dims - the dimensions of the parent window.
       */
      void
      generatePauseScreen(const olc::vi2d& dims);

      /**
       * @brief - Create a valid game over screen menu.
       * @param dims - the dimensions of the parent window.
       */
      void
      generateGameOverScreen(const olc::vi2d& dims);

      /**
       * @brief - Used to update the list of saved games. It
       *          is typically used in case the load game menu
       *          is being displayed to ensure that we have
       *          up to date information in it.
       */
      void
      refreshSavedGames();

      /**
       * @brief - Used to update the display menus representing
       *          the list of saved games available for loading.
       *          This assumes the list of games is up to date
       *          and will use the current index to display all
       *          subsequent games in the limit of the allowed
       *          number per page.
       *          It will also handle the de/activation of the
       *          navigation buttons if needed.
       */
      void
      updateSavedGamesDisplay();

      /**
       * @brief - Used to generate a new default name for a new
       *          world when none is defined.
       *          Note that use the base name for the world and
       *          append an integer to differentiate between
       *          saves.
       * @return - the generated file name to save the world.
       */
      std::string
      generateSaveFileName() const noexcept;

    private:

      /**
       * @brief - Defines the background color for a screen.
       */
      static const olc::Pixel sk_screenBGColor;

      /**
       * @brief - Defines the background color for a menu on
       *          a particular screen.
       */
      static const olc::Pixel sk_menuBGColor;

      /**
       * @brief - Defines the text color for a menu on any
       *          screen.
       */
      static const olc::Pixel sk_menuTextColor;

      /**
       * @brief - Defines the text color for the text of a
       *          highlighted menu.
       */
      static const olc::Pixel sk_menuTextColorHighlight;

      /**
       * @brief - Defines the number of saved games that can
       *          be displayed on a single page of the load
       *          game menu.
       */
      static constexpr const unsigned sk_savedGamesPerPage = 10u;

      /**
       * @brief - Defines the path to the directory where the
       *          saved games are registered.
       */
      static constexpr const char* sk_savedGamesDir = "data/worlds";

      /**
       * @brief - Defines a base string to be used when a new
       *          game should be saved.
       */
      static constexpr const char* sk_baseWorldName = "new_world_";

      /**
       * @brief - Defines the default extension for a world save.
       */
      static constexpr const char* sk_saveGameExtension = ".lvl";

      /**
       * @brief - Convenience structure regrouping the available
       *          saved games as fetched from the saves directory.
       */
      struct SavedGamesData {
        // The list of saved games as listed in the directory where
        // games are dumped.
        std::vector<std::string> saves;

        // The index of the first element displayed in the load game
        // screen. Starts at `0` and can range until all the games
        // are displayed (assuming we're displaying a certain number
        // per page).
        unsigned index;

        // The number of saved games displayed per page.
        unsigned gamesPerPage;

        // The menus allowing to display the saved games names.
        std::vector<GameMenuShPtr> games;

        // The menu representing the previous page option.
        GameMenuShPtr previous;

        // The menu representing the next page option.
        GameMenuShPtr next;
      };

      /**
       * @brief - Defines the current screen selected in this
       *          game. Updated whenever the user takes action
       *          to change it.
       */
      game::Screen m_screen;

      /**
       * @brief - Defines the path to the file where the world
       *          associated to the current game is to be saved
       *          or loaded from.
       */
      std::string m_worldFile;

      /**
       * @brief - Defines the screen to display when the game is
       *          on the home screen.
       */
      MenuShPtr m_homeScreen;

      /**
       * @brief - Defines the screen to display when the game is
       *          on the loading game screen.
       */
      MenuShPtr m_loadGameScreen;

      /**
       * @brief - The data needed to represent the list of games
       *          available for loading.
       */
      SavedGamesData m_savedGames;

      /**
       * @brief - Defines the screen to display when the game is
       *          in pause.
       */
      MenuShPtr m_pauseScreen;

      /**
       * @brief - Defines the menu to display in case
       *          the game is over.
       */
      MenuShPtr m_gameOverScreen;
  };

  using GameStateShPtr = std::shared_ptr<GameState>;
}

# include "GameState.hxx"

#endif    /* GAME_STATE_HH */
