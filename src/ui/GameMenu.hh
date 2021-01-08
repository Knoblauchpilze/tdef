#ifndef    GAME_MENU_HH
# define   GAME_MENU_HH

# include "SimpleMenu.hh"

namespace tdef {

  class GameMenu: public SimpleMenu {
    public:

      /**
       * @brief - Create a new game menu which builds upon the
       *          simple menu in order to add the possibility
       *          to enable/disable it.
       * @param text - the text to assign to this menu.
       * @param cb - the callback to trigger when the menu is
       *             clicked upon.
       * @param parent - the parent menu for this element. Specify
       *                 `null` in case the menu is the root of the
       *                 subsystem.
       */
      GameMenu(const std::string& text,
               const menu::RegisterAction& cb,
               Menu* parent = nullptr);

      ~GameMenu() = default;

    private:

      /**
       * @brief - Defines the default background color for an active
       *          menu.
       */
      static const olc::Pixel sk_activeBGColor;

      /**
       * @brief - Defines the default text color for an active menu.
       */
      static const olc::Pixel sk_activeTextColor;

      /**
       * @brief - Defines the default background color for a disabled
       *          menu.
       */
      static const olc::Pixel sk_disabledBGColor;

      /**
       * @brief - Defines the default text color for a disabled menu.
       */
      static const olc::Pixel sk_disabledTextColor;
  };

}

#endif    /* GAME_MENU_HH */
