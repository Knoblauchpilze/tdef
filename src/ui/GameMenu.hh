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
       * @param enabled - `true` if this menu is enabled at first.
       */
      GameMenu(const std::string& text,
               const menu::RegisterAction& cb,
               Menu* parent = nullptr,
               bool enabled = true);

      ~GameMenu() = default;

      /**
       * @brief - Used to turn this menu either active or inactive
       *          based on the input boolean.
       * @param enabled - `true` if the menu is enabled.
       */
      void
      enable(bool enabled);

    protected:

      /**
       * @brief - Reimplementation of the interface method so that
       *          we can prevent the menu from being selected in
       *          case it is disabled.
       * @return - `true` only if the menu is enabled.
       */
      bool
      onHighlight() const override;

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

      /**
       * @brief - Holds the status of this menu: `true` if the menu
       *          is enabled and `false` otherwise.
       */
      bool m_enabled;
  };

}

#endif    /* GAME_MENU_HH */
