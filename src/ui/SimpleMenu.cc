
# include "SimpleMenu.hh"

namespace tdef {

  SimpleMenu::SimpleMenu(const olc::vi2d& pos,
                        const olc::vf2d& size,
                        const menu::BackgroundDesc& bg,
                        const menu::MenuContentDesc& fg,
                        const menu::RegisterAction& cb,
                        Menu* parent):
    Menu(pos, size, "simple_menu", bg, fg, menu::Layout::Horizontal, true, parent),

    m_callback(cb)
  {}

  void
  SimpleMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Trigger the callback.
    m_callback(actions);
  }

}
