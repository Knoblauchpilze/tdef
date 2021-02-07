
# include "SimpleMenu.hh"

namespace tdef {

  SimpleMenu::SimpleMenu(const olc::vi2d& pos,
                        const olc::vf2d& size,
                        const menu::BackgroundDesc& bg,
                        const menu::MenuContentDesc& fg,
                        const menu::RegisterAction& cb,
                        Menu* parent):
    Menu(pos, size, "simple_menu", bg, fg, menu::Layout::Horizontal, true, true, parent),

    m_callback(cb)
  {}

  void
  SimpleMenu::setAction(menu::RegisterAction action) {
    m_callback = action;
  }

  void
  SimpleMenu::setSimpleAction(action::Process process) {
    m_callback = [process](std::vector<ActionShPtr>& actions) {
      actions.push_back(std::make_shared<SimpleAction>(process));
    };
  }

  void
  SimpleMenu::onClick(std::vector<ActionShPtr>& actions) const {
    // Trigger the callback.
    m_callback(actions);
  }

}
