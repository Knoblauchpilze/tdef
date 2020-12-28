#ifndef    TOWER_MENU_HH
# define   TOWER_MENU_HH

# include <memory>
# include "Menu.hh"
# include "Tower.hh"

namespace tdef {

  class TowerMenu: public Menu {
    public:

      /**
       * @brief - Create a new tower menu allowing to interact
       *          (typically by creating or removing) towers in
       *          the world. Upon clicking on this menu a new
       *          action to register or remove a tower will be
       *          created.
       * @param pos - the position of the menu in the parent.
       * @param size - the dimensions of the menu.
       * @param type - the type of the tower to create from this
       *               menu.
       * @param bg - the background for this menu.
       * @param fg - the foreground for this menu.
       * @param parent - the parent menu for this element. Specify
       *                 `null` in case the menu is the root of the
       *                 subsystem.
       */
      TowerMenu(const olc::vi2d& pos,
                const olc::vf2d& size,
                const towers::Type& type,
                const menu::BackgroundDesc& bg,
                const menu::MenuContentDesc& fg,
                Menu* parent = nullptr);

      ~TowerMenu() = default;

    protected:

      /**
       * @brief - Interface method allowing inheriting classes
       *          to perform the creation of their own actions
       *          whenver a click is detected on this menu.
       *          For now this method is only triggered when a
       *          click witht he left mouse button is detected.
       *          The default implementation does nothing.
       * @param actions - output vector to register actions if
       *                  needed.
       */
      virtual
      void
      onClick(std::vector<ActionShPtr>& actions) const;

    private:

      /**
       * @brief - The description of the type of tower to create
       *          from this menu.
       */
      towers::Type m_type;
  };

}

#endif    /* TOWER_MENU_HH */
