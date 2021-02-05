#ifndef    SIMPLE_MENU_HH
# define   SIMPLE_MENU_HH

# include <memory>
# include "Menu.hh"
# include "SimpleAction.hh"

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
      
      /**
       * @brief - Used to define a new simple action where the
       *          process would be to push the input callback
       *          to the provided vector of actions.
       * @param process - the callback to be pushed to the list
       *                  of action whenever the callback linked
       *                  to this menu is triggered.
       */
      void
      setSimpleAction(action::Process process);

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

  using SimpleMenuShPtr = std::shared_ptr<SimpleMenu>;
}

#endif    /* SIMPLE_MENU_HH */
