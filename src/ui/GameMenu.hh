#ifndef    GAME_MENU_HH
# define   GAME_MENU_HH

# include <memory>
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

      /**
       * @brief - Create a new game menu which builds upon the
       *          simple menu in order to add the possibility
       *          to enable/disable it but still allows to
       *          specify the background and foreground color.
       * @param bg - the background description for this menu.
       * @param fg - the content description for this menu.
       * @param cb - the callback to trigger when the menu is
       *             clicked upon.
       * @param parent - the parent menu for this element. Specify
       *                 `null` in case the menu is the root of the
       *                 subsystem.
       * @param enabled - `true` if this menu is enabled at first.
       */
      GameMenu(const menu::BackgroundDesc& bg,
               const menu::MenuContentDesc& fg,
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

      /**
       * @brief - The color of the background if this menu is active.
       */
      olc::Pixel m_activeBGColor;

      /**
       * @brief - The color of the background if this menu is disabled.
       */
      olc::Pixel m_disabledBGColor;

      /**
       * @brief - The color of the text if this menu is active.
       */
      olc::Pixel m_activeTextColor;

      /**
       * @brief - The color of the text is this menu is disabled.
       */
      olc::Pixel m_disabledTextColor;
  };

  using GameMenuShPtr = std::shared_ptr<GameMenu>;
}

#endif    /* GAME_MENU_HH */
