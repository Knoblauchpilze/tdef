#ifndef    GAME_STATE_HH
# define   GAME_STATE_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "olcEngine.hh"
# include "Menu.hh"

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
