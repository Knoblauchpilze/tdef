#ifndef    SIMPLE_MENU_HH
# define   SIMPLE_MENU_HH

# include <memory>
# include "Menu.hh"

namespace tdef {
  namespace menu {

    /**
     * @brief - Defines a common function called whenever
     *          the menu is clicked upon. This allows to
     *          specialize the menu class easily when not
     *          much is needed.
     */
    using RegisterAction = std::function<void(std::vector<ActionShPtr>&)>;

  }

  class SimpleMenu: public Menu {
    public:

      /**
       * @brief - Create a new simple menu allowing to interact
       *          (typically by creating or removing) towers in
       *          the world. Upon clicking on this menu a call
       *          to the provided callback will be triggered.
       * @param pos - the position of the menu in the parent.
       * @param size - the dimensions of the menu.
       * @param bg - the background for this menu.
       * @param fg - the foreground for this menu.
       * @param cb - the callback to trigger when the menu is
       *             clicked upon.
       * @param parent - the parent menu for this element. Specify
       *                 `null` in case the menu is the root of the
       *                 subsystem.
       */
      SimpleMenu(const olc::vi2d& pos,
                 const olc::vf2d& size,
                 const menu::BackgroundDesc& bg,
                 const menu::MenuContentDesc& fg,
                 const menu::RegisterAction& cb,
                 Menu* parent = nullptr);

      ~SimpleMenu() = default;

      /**
       * @brief - Assigns a new action attached to this menu.
       * @param action - the action attached to the menu.
       */
      void
      setAction(menu::RegisterAction action);

    protected:

      virtual
      void
      onClick(std::vector<ActionShPtr>& actions) const;

    private:

      /**
       * @brief - The callback to trigger whenver this menu is
       *          clicked upon.
       */
      menu::RegisterAction m_callback;
  };

}

#endif    /* SIMPLE_MENU_HH */
