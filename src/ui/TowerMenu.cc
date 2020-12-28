
# include "TowerMenu.hh"
# include "TowerAction.hh"

namespace tdef {

  TowerMenu::TowerMenu(const olc::vi2d& pos,
                       const olc::vf2d& size,
                       const towers::Type& type,
                       const menu::BackgroundDesc& bg,
                       const menu::MenuContentDesc& fg,
                       Menu* parent):
    Menu(pos, size, "tower_menu", bg, fg, menu::Layout::Horizontal, true, parent),

    m_type(type)
  {}

  void
  TowerMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Push a new action with the corresponding tower
    // type to be executed.
    actions.push_back(std::make_shared<TowerAction>(m_type));
  }

}
